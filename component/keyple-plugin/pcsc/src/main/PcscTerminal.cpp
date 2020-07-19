/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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
#include "IllegalArgumentException.h"
#include "System.h"
#include "Thread.h"

/* PC/SC plugin */
#include "PcscTerminal.h"
#include "PcscTerminalException.h"

namespace keyple {
namespace plugin {
namespace pcsc {

#ifdef WIN32
std::string pcsc_stringify_error(LONG rv)
{
    static char out[20];
    sprintf_s(out, sizeof(out), "0x%08X", rv);

    return std::string(out);
}
#endif

PcscTerminal::PcscTerminal(const std::string& name)
: mContext(0), mHandle(0), mState(0), mName(name), mContextEstablished(false)
{
    memset(&mPioSendPCI, 0, sizeof(SCARD_IO_REQUEST));
}

const std::string& PcscTerminal::getName() const
{
    return mName;
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

    if (!len)
        /* No readers to add to list */
        return list;

    readers = (char*)calloc(len, sizeof(char));

    if (!readers)
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

    if (mContextEstablished)
        return;

    ret = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &mContext);
    if (ret != SCARD_S_SUCCESS) {
        mContextEstablished = false;
        mLogger->error("SCardEstablishContext failed with error: %\n",
                      std::string(pcsc_stringify_error(ret)));
        throw PcscTerminalException("SCardEstablishContext failed");
    }

    mContextEstablished = true;
}

void PcscTerminal::releaseContext()
{
    mLogger->debug("releaseContext - contextEstablished: %\n",
                  mContextEstablished ? "yes" : "no");

    if (!mContextEstablished)
        return;

    mLogger->debug("releaseContext - releasing context\n");

    SCardReleaseContext(mContext);
    mContextEstablished = false;
}

bool PcscTerminal::isCardPresent(bool release)
{
    DWORD protocol;
    SCARDHANDLE hCard;
    LONG rv;

    try {
        establishContext();
    } catch (PcscTerminalException& e) {
        mLogger->error("isCardPresent - caught PcscTerminalException %\n", e);
        throw e;
    }

    rv = SCardConnect(mContext, (LPCSTR)mName.c_str(),
                      SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
                      &hCard, &protocol);
    if (rv != SCARD_S_SUCCESS) {
        if (rv != static_cast<LONG>(SCARD_E_NO_SMARTCARD) &&
            rv != static_cast<LONG>(SCARD_W_REMOVED_CARD))
            mLogger->debug("isCardPresent - error connecting to card (%)\n",
                          std::string(pcsc_stringify_error(rv)));

        if (release)
            releaseContext();

        return false;
    }

    if (release)
        releaseContext();

    return true;
}

bool PcscTerminal::waitForCardPresent(long long timeout)
{
    bool present;
    long long current;
    long long start = System::currentTimeMillis();

    mLogger->trace("waitForCardPresent - waiting for card insertion for % ms\n",
                  timeout);

    do {
        present = isCardPresent(false);
        if (present) {
            mLogger->trace("waitForCardPresent - card present\n");
            return true;
        }

        Thread::sleep(100);

        if (timeout == INFINITE)
            continue;

        current = System::currentTimeMillis();
        if (current > (start + timeout)) {
            mLogger->trace("waitForCardPresent - timeout\n");
            return false;
        }

    } while (1);
}

void PcscTerminal::openAndConnect(const std::string& protocol)
{
    LONG rv;
    DWORD connectProtocol;
    DWORD sharingMode = SCARD_SHARE_SHARED;
    BYTE reader[200];
    DWORD readerLen = sizeof(reader);
    BYTE _atr[33];
    DWORD atrLen = sizeof(_atr);

    mLogger->debug("openAndConnect - protocol: %\n", protocol);

    try {
        establishContext();
    } catch (PcscTerminalException& e) {
        mLogger->error("openAndConnect - caught PcscTerminalException %\n", e);
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

    mLogger->debug("openAndConnect - connecting tp % with protocol: %, "
                  "connectProtocol: % and sharingMode: %\n", mName,
                  protocol, connectProtocol, sharingMode);

    rv = SCardConnect(mContext, mName.c_str(), sharingMode,
                      connectProtocol, &mHandle, &mProtocol);
    if (rv != SCARD_S_SUCCESS) {
        mLogger->error("openAndConnect - SCardConnect failed (%)\n",
                      std::string(pcsc_stringify_error(rv)));
        releaseContext();
        throw PcscTerminalException("openAndConnect failed");
    }

    switch (mProtocol) {
    case SCARD_PROTOCOL_T0:
        mPioSendPCI = *SCARD_PCI_T0;
        break;
    case SCARD_PROTOCOL_T1:
        mPioSendPCI = *SCARD_PCI_T1;
        break;
    }

    rv = SCardStatus(mHandle, (LPSTR)reader, &readerLen, &mState,
                     &mProtocol, _atr, &atrLen);
    if (rv != SCARD_S_SUCCESS) {
        mLogger->error("openAndConnect - SCardStatus failed (s)\n",
                      std::string(pcsc_stringify_error(rv)));
        releaseContext();
        throw PcscTerminalException("openAndConnect failed");
    } else {
        mLogger->debug("openAndConnect - card state: %\n", mState);
    }

    mAtr.clear();
    mAtr.insert(mAtr.end(), _atr, _atr + atrLen);
}

void PcscTerminal::closeAndDisconnect(bool reset)
{
    mLogger->debug("closeAndDisconnect - reset: %\n", reset ? "yes" : "no");

    SCardDisconnect(mContext, reset ? SCARD_LEAVE_CARD : SCARD_RESET_CARD);

    releaseContext();
}

bool PcscTerminal::waitForCardAbsent(long long timeout)
{
    bool present;
    long long current;
    long long start = System::currentTimeMillis();

    mLogger->trace("waitForCardAbsent - waiting for card removal for % ms\n",
                  timeout);

    do {
        present = isCardPresent(false);
        if (!present) {
            mLogger->trace("waitForCardAbsent - card absent\n");
            return true;
        }

        Thread::sleep(100);

        if (timeout == INFINITE)
            continue;

        current = System::currentTimeMillis();
        if (current > (start + timeout)) {
            mLogger->trace("waitForCardAbsent - timeout\n");
            return false;
        }

    } while (1);
}

const std::vector<uint8_t>& PcscTerminal::getATR()
{
    return mAtr;
}

std::vector<uint8_t>
PcscTerminal::transmitApdu(const std::vector<uint8_t>& apduIn)
{
    if (apduIn.size() == 0)
        throw IllegalArgumentException("command cannot be empty");

    /* Make a copy */
    std::vector<uint8_t> _apduIn = apduIn;

    /* To check */
    bool t0GetResponse = true;
    bool t1GetResponse = true;
    bool t1StripLe     = true;

    mLogger->debug("transmitApdu - c-apdu >> %\n", apduIn);

    /*
     * Note that we modify the 'command' array in some cases, so it must
     * be a copy of the application provided data
     */
    int n   = _apduIn.size();
    bool t0 = mProtocol == SCARD_PROTOCOL_T0;
    bool t1 = mProtocol == SCARD_PROTOCOL_T1;
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
        long rv;
        rv = SCardTransmit(mHandle, &mPioSendPCI, (LPCBYTE)_apduIn.data(),
                      _apduIn.size(), NULL, (LPBYTE)r_apdu, &dwRecv);
        if (rv != SCARD_S_SUCCESS) {
            mLogger->error("SCardEstablishContext failed with error: %\n",
                          std::string(pcsc_stringify_error(rv)));
            throw PcscTerminalException("ScardTransmit failed");
        }
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
                n          = 5;
                continue;
            }
        }
        result.insert(result.end(), response.begin(), response.begin() + rn);
        break;
    }

    mLogger->debug("transmitApdu - r-apdu << %\n", result);

    return result;
}

void PcscTerminal::beginExclusive()
{
}

void PcscTerminal::endExclusive()
{
}

bool PcscTerminal::operator==(const PcscTerminal& o) const
{
    return !mName.compare(o.mName);
}

bool PcscTerminal::operator!=(const PcscTerminal& o) const
{
	return !(*this == o);
}

std::ostream& operator<<(std::ostream& os, const PcscTerminal& t)
{
    os << "PCSCTERMINAL: {"
       << "NAME = " << t.mName
	   << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<PcscTerminal>& vt)
{
    os << "PCSCTERMINALS: {";
    for (const auto& t : vt) {
        os << t;
        if (t != vt.back())
            os << ", ";
    }
	os << "}";

    return os;
}

}
}
}
