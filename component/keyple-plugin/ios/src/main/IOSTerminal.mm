/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "IOSTerminal.h"

#include <cstring>

/* Common */
#include "IllegalArgumentException.h"
#include "KeypleStd.h"
#include "System.h"
#include "Thread.h"

/* iOS plugin */
#include "IOSTerminalException.h"

namespace keyple {
namespace plugin {
namespace ios {

using namespace keyple::common;
using namespace keyple::common::exception;

IOSTerminal::IOSTerminal(const std::string& name)
: mName(name)
{
    mLogger->debug("initTerminal\n");

    mIOSTerminalImpl = [[IOSTerminalImpl alloc] init];
}

const std::string& IOSTerminal::getName() const
{
    return mName;
}

const std::vector<std::string>& IOSTerminal::listTerminals()
{
    int len = 0;
    static std::vector<std::string> list;

    /* Clear list */
    list.clear();

    /* ... Fetch reader list */
    list.push_back("IOSReader");

    if (!len)
        /* No readers to add to list */
        return list;

    return list;
}

bool IOSTerminal::isCardPresent()
{
    return [mIOSTerminalImpl isCardPresent];
}

bool IOSTerminal::didUserCancelOrTimeout()
{
    return [mIOSTerminalImpl didUserCancelOrTimeout];
}

void IOSTerminal::waitForCardPresent()
{
    mLogger->debug("[%] waitForCardPresent\n", mName);

    [mIOSTerminalImpl waitForCardPresent];
}

void IOSTerminal::stopWaitForCard()
{
    mLogger->debug("[%] stopWaitForCard\n", mName);

    [mIOSTerminalImpl stopWaitForCard];
}

void IOSTerminal::waitForCardAbsent()
{
    mLogger->debug("[%] waitForCardAbsent\n", mName);

    [mIOSTerminalImpl waitForCardAbsent];
}

void IOSTerminal::openAndConnect(const std::string& protocol)
{
    std::vector<uint8_t> atr;

    mLogger->debug("[%] openAndConnect - protocol: %\n", mName, protocol);

    [mIOSTerminalImpl openAndConnect];

    mAtr.clear();
    //mAtr.insert(mAtr.end(), _atr, _atr + atrLen);
}

void IOSTerminal::closeAndDisconnect(bool reset)
{
    mLogger->debug("[%] closeAndDisconnect - reset: %\n", mName, reset ? "yes" : "no");

    [mIOSTerminalImpl closeAndDisconnect];
}

const std::vector<uint8_t>& IOSTerminal::getATR()
{
    return mAtr;
}

std::vector<uint8_t> IOSTerminal::transmitApdu(const std::vector<uint8_t>& apduIn)
{
    if (apduIn.size() == 0)
        throw IllegalArgumentException("command cannot be empty");

    /* Make a copy */
    std::vector<uint8_t> _apduIn = apduIn;

    /* To check */
    bool t0GetResponse = true;
    bool t1GetResponse = true;
    bool t1StripLe     = true;

    /*
     * Note that we modify the 'command' array in some cases, so it must be a copy of the
     * application provided data
     */
    int n   = _apduIn.size();
    bool t0 = 0;//mProtocol == SCARD_PROTOCOL_T0;
    bool t1 = 1;//mProtocol == SCARD_PROTOCOL_T1;

    if (t0 && (n >= 7) && (_apduIn[4] == 0))
        throw IOSTerminalException("Extended len. not supported for T=0");

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
    int k = 0;
    std::vector<uint8_t> result;

    while (true) {
        if (++k >= 32) {
            throw IOSTerminalException("Could not obtain response");
        }

        mLogger->debug("[%] transmitApdu - c-apdu >> %\n", mName, _apduIn);

        /* C++ <-> Obj-C data format convertion */
        NSData *nsCApdu = [NSData dataWithBytes: _apduIn.data() length: _apduIn.size()];
        NSData *nsRApdu = [mIOSTerminalImpl transmitApdu:nsCApdu];
        const size_t count = [nsRApdu length];
        uint8_t *first = (uint8_t *)[nsRApdu bytes];
        uint8_t *last = first + count;
        std::vector<uint8_t> response(first, last);

        if (response.size() < 2)
            throw IOSTerminalException("IOSTerminalImpl_transmitApdu failed");

        mLogger->debug("[%] transmitApdu - r-apdu << %\n", mName, response);

        int rn = response.size();
        if (getresponse && (rn >= 2)) {
            /* See ISO 7816/2005, 5.1.3 */
            if ((rn == 2) && (response[0] == 0x6c)) {
                // Resend command using SW2 as short Le field
                _apduIn[n - 1] = response[1];
                continue;
            }

            if (response[rn - 2] == 0x61) {
                /* Issue a GET RESPONSE command with the same CLA using SW2 as short Le field */
                if (rn > 2)
                    result.insert(result.end(), response.begin(), response.begin() + rn - 2);

                std::vector<uint8_t> getResponse(5);
                getResponse[0] = _apduIn[0];
                getResponse[1] = 0xC0;
                getResponse[2] = 0;
                getResponse[3] = 0;
                getResponse[4] = response[rn - 1];
                n          = 5;
                _apduIn.swap(getResponse);
                continue;
            }
        }

        result.insert(result.end(), response.begin(), response.begin() + rn);
        break;
    }

    return result;
}

void IOSTerminal::beginExclusive()
{
}

void IOSTerminal::endExclusive()
{
}

bool IOSTerminal::operator==(const IOSTerminal& o) const
{
    return !mName.compare(o.mName);
}

bool IOSTerminal::operator!=(const IOSTerminal& o) const
{
	return !(*this == o);
}

std::ostream& operator<<(std::ostream& os, const IOSTerminal& t)
{
    os << "IOSTERMINAL: {"
       << "NAME = " << t.mName
	   << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<IOSTerminal>& vt)
{
    os << "IOSTERMINALS: {";
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
