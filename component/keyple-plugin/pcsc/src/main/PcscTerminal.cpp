/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include <cstring>

/* Common */
#include "exceptionhelper.h"
#include "System.h"

/* PC/SC plugin */
#include "PcscTerminal.h"
#include "PcscTerminalException.h"

namespace keyple {
namespace plugin {
namespace pcsc {

#ifdef WIN32
static char* pcsc_stringify_error(LONG rv)
{
    static char out[20];
    sprintf_s(out, sizeof(out), "0x%08X", rv);

    return out;
}
#endif

PcscTerminal::PcscTerminal(const std::string& name)
: context(0), handle(0), state(0), name(name), contextEstablished(false)
{
    memset(&pioSendPCI, 0, sizeof(SCARD_IO_REQUEST));
}

const std::string& PcscTerminal::getName() const
{
    return this->name;
}

const std::vector<std::string>& PcscTerminal::listTerminals()
{
    LONG ret;
    SCARDCONTEXT context;
    char* readers = NULL;
    char* ptr     = NULL;
    DWORD len     = 0;
    static std::vector<std::string> list;

    /* Clear list */
    list.clear();

    ret = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &context);
    if (ret != SCARD_S_SUCCESS) {
        throw PcscTerminalException("SCardEstablishContext failed");
    }

    ret = SCardListReaders(context, NULL, NULL, &len);
    if (ret != SCARD_S_SUCCESS) {
        throw PcscTerminalException("SCardListReaders failed");
    }

    readers = (char*)calloc(len, sizeof(char));

    if (!len || !readers)
        /* No readers to add to list */
        return list;

    ret = SCardListReaders(context, NULL, readers, &len);
    if (ret != SCARD_S_SUCCESS) {
        throw PcscTerminalException("SCardListReaders failed");
    }

    ptr = readers;

    if (!ptr)
        return list;

    while (*ptr) {
        std::string s(ptr);
        list.push_back(s);
        ptr += strlen(ptr) + 1;
    }

    SCardReleaseContext(context);
    free(readers);

    return list;
}

void PcscTerminal::establishContext()
{
    LONG ret;

    if (this->contextEstablished)
        return;

    ret = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &this->context);
    if (ret != SCARD_S_SUCCESS) {
        this->contextEstablished = false;
        throw PcscTerminalException("SCardEstablishContext failed");
    }

    this->contextEstablished = true;
}

void PcscTerminal::releaseContext()
{
    if (!this->contextEstablished)
        return;

    SCardReleaseContext(this->context);
    this->contextEstablished = false;
}

bool PcscTerminal::isCardPresent()
{
    DWORD protocol;
    SCARDHANDLE hCard;
    LONG rv;

    try {
        establishContext();
    } catch (PcscTerminalException& e) {
        logger->error("isCardPresent - caught PcscTerminalException (msg: %s"
                      ", cause: %s)\n",
                      e.getMessage().c_str(), e.getCause().what());
        throw e;
    }

    rv = SCardConnect(this->context, (LPCSTR)this->name.c_str(),
                      SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
                      &hCard, &protocol);
    if (rv != SCARD_S_SUCCESS) {
        if (rv != (int)SCARD_E_NO_SMARTCARD)
            logger->debug("isCardPresent - error connecting to card (%s)\n",
                          pcsc_stringify_error(rv));
        releaseContext();
        return false;
    }

    releaseContext();
    return true;
}

bool PcscTerminal::waitForCardPresent(long long timeout)
{
    bool present;
    long current;
    long start = System::currentTimeMillis();

    do {
        present = isCardPresent();
        if (present) {
            logger->debug("waitForCardPresent - card present\n");
            return true;
        }

        if (timeout == INFINITE)
            continue;

        current = System::currentTimeMillis();
        if (current > (start + timeout)) {
            logger->debug("waitForCardPresent - timeout\n");
            return false;
        }

    } while (1);
}

void PcscTerminal::openAndConnect(std::string protocol)
{
    LONG rv;
    DWORD connectProtocol;
    DWORD sharingMode = SCARD_SHARE_SHARED;
    BYTE reader[200];
    DWORD readerLen = sizeof(reader);
    BYTE _atr[33];
    DWORD atrLen = sizeof(_atr);

    logger->debug("openAndConnect\n");

    try {
        establishContext();
    } catch (PcscTerminalException& e) {
        logger->error("isCardPresent - caught PcscTerminalException (msg: %s"
                      ", cause: %s)\n",
                      e.getMessage().c_str(), e.getCause().what());
        throw e;
    }

    if (!protocol.compare("*")) {
        connectProtocol = SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1;
    } else if (!protocol.compare("T=0")) {
        connectProtocol = SCARD_PROTOCOL_T0;
    } else if (!protocol.compare("T=1")) {
        connectProtocol = SCARD_PROTOCOL_T1;
    } else if (!protocol.compare("direct")) {
        connectProtocol = 0;
        sharingMode     = SCARD_SHARE_DIRECT;
    } else {
        throw IllegalArgumentException("Unsupported protocol " + protocol);
    }

    logger->debug("openAndConnect - connecting with protocol: %s, "
                  "connectProtocol: %d and sharingMode: %d\n",
                  protocol.c_str(), connectProtocol, sharingMode);

    rv = SCardConnect(this->context, this->name.c_str(), sharingMode,
                      connectProtocol, &this->handle, &this->protocol);
    if (rv != SCARD_S_SUCCESS) {
        logger->error("openAndConnect - SCardConnect failed (%s)\n",
                      pcsc_stringify_error(rv));
        releaseContext();
        throw PcscTerminalException("openAndConnect failed");
    }

    switch (this->protocol) {
    case SCARD_PROTOCOL_T0:
        this->pioSendPCI = *SCARD_PCI_T0;
        break;
    case SCARD_PROTOCOL_T1:
        this->pioSendPCI = *SCARD_PCI_T1;
        break;
    }

    rv = SCardStatus(this->handle, (LPSTR)reader, &readerLen, &this->state,
                     &this->protocol, _atr, &atrLen);
    if (rv != SCARD_S_SUCCESS) {
        logger->error("openAndConnect - SCardStatus failed (%s)\n",
                      pcsc_stringify_error(rv));
        releaseContext();
        throw PcscTerminalException("openAndConnect failed");
    } else {
        logger->debug("openAndConnect - card state: %d\n", this->state);
    }

    this->atr.clear();
    this->atr.insert(this->atr.end(), _atr, _atr + atrLen);
}

void PcscTerminal::closeAndDisconnect(bool reset)
{
    logger->debug("closeAndDisconnect\n");

    SCardDisconnect(this->context, reset ? SCARD_LEAVE_CARD : SCARD_RESET_CARD);

    releaseContext();
}

bool PcscTerminal::waitForCardAbsent(long long timeout)
{
    bool present;
    long current;
    long start = System::currentTimeMillis();

    do {
        present = isCardPresent();
        if (!present) {
            logger->debug("waitForCardPresent - card absent\n");
            return true;
        }

        if (timeout == INFINITE)
            continue;

        current = System::currentTimeMillis();
        if (current > (start + timeout)) {
            logger->debug("waitForCardPresent - timeout\n");
            return false;
        }

    } while (1);
}

const std::vector<uint8_t>& PcscTerminal::getATR()
{
    return this->atr;
}

std::vector<uint8_t> PcscTerminal::transmitApdu(const std::vector<uint8_t>& apduIn)
{
    if (apduIn.size() == 0)
        throw IllegalArgumentException("command cannot be empty");

    /* Make a copy */
    std::vector<uint8_t> _apduIn = apduIn;

    /* To check */
    bool t0GetResponse = true;
    bool t1GetResponse = true;
    bool t1StripLe     = true;

    logger->debug("doTransmit\n");

    /*
     * Note that we modify the 'command' array in some cases, so it must
     * be a copy of the application provided data
     */
    int n   = _apduIn.size();
    bool t0 = this->protocol == SCARD_PROTOCOL_T0;
    bool t1 = this->protocol == SCARD_PROTOCOL_T1;
    if (t0 && (n >= 7) && (_apduIn[4] == 0)) {
        throw PcscTerminalException("Extended len. not supported for T=0");
    }
    if ((t0 || (t1 && t1StripLe)) && (n >= 7)) {
        int lc = _apduIn[4] & 0xff;
        if (lc != 0) {
            if (n == lc + 6) {
                n--;
            }
        } else {
            lc = ((_apduIn[5] & 0xff) << 8) | (_apduIn[6] & 0xff);
            if (n == lc + 9) {
                n -= 2;
            }
        }
    }

    bool getresponse = (t0 && t0GetResponse) || (t1 && t1GetResponse);
    int k            = 0;
    std::vector<uint8_t> result;

    while (true) {
        if (++k >= 32) {
            throw PcscTerminalException("Could not obtain response");
        }
        char r_apdu[261];
        DWORD dwRecv = sizeof(r_apdu);
        SCardTransmit(this->handle, &this->pioSendPCI, (LPCBYTE)_apduIn.data(),
                      _apduIn.size(), NULL, (LPBYTE)r_apdu, &dwRecv);
        std::vector<uint8_t> response(r_apdu, r_apdu + dwRecv);
        int rn = response.size();
        if (getresponse && (rn >= 2)) {
            // see ISO 7816/2005, 5.1.3
            if ((rn == 2) && (response[0] == 0x6c)) {
                // Resend command using SW2 as short Le field
                _apduIn[n - 1] = response[1];
                continue;
            }
            if (response[rn - 2] == 0x61) {
                // Issue a GET RESPONSE command with the same CLA
                // using SW2 as short Le field
                if (rn > 2) {
                    result.insert(result.end(), response.begin(),
                                  response.begin() + rn - 2);
                }
                _apduIn[1] = 0xC0;
                _apduIn[2] = 0;
                _apduIn[3] = 0;
                _apduIn[4] = response[rn - 1];
                n         = 5;
                continue;
            }
        }
        result.insert(result.end(), response.begin(), response.begin() + rn);
        break;
    }
    return result;
}

void PcscTerminal::beginExclusive()
{
}

void PcscTerminal::endExclusive()
{
}

}
}
}
