/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

/* Core */
#include "ByteArrayUtils.h"

/* Common */
#include "exceptionhelper.h"
#include "stringhelper.h"

/* Smartcard I/O */
#include "Card.h"
#include "CardChannel.h"
#include "CardException.h"

#if defined(WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <winscard.h>
#else
#include <PCSC/winscard.h>
#include <PCSC/wintypes.h>
#endif

namespace org {
namespace eclipse {
namespace keyple {
namespace smartcardio {

using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
using Card           = org::eclipse::keyple::smartcardio::Card;
using CardException  = org::eclipse::keyple::smartcardio::CardException;

CardChannel::CardChannel(Card* card, int channel) : card(card), channel(channel)
{
    
}

Card* CardChannel::getCard()
{
    return card;
}

int CardChannel::getChannelNumber()
{
    checkClosed();
    return channel;
}

std::shared_ptr<ResponseAPDU>
CardChannel::transmit(std::shared_ptr<CommandAPDU> command)
{
    checkClosed();
    card->checkExclusive();
    std::vector<char> commandBytes = command->getBytes();
    std::vector<char> responseBytes = doTransmit(commandBytes);
    return std::make_shared<ResponseAPDU>(responseBytes);
}

int CardChannel::transmit(std::vector<char>& command,
                          std::vector<char>& response)
{
    checkClosed();
    card->checkExclusive();

    if (command.size() == 0 || command == response) {
        throw new IllegalArgumentException
                ("command and response must not be the same object");
    }

    std::vector<char> resp = doTransmit(command);
    response.insert(response.end(), resp.begin(), resp.end());

    return resp.size();
}

void CardChannel::close()
{
    if (getChannelNumber() == 0)
        throw IllegalStateException("Cannot close basic logical channel");

    if (this->isClosed)
        return;

    card->checkExclusive();
    try {
        std::vector<char> com = {0x00, 0x70, (char)0x80, 0};
        com[3] = getChannelNumber();
        setChannel(com);

        char r_apdu[261];
        DWORD dwRecv = sizeof(r_apdu);
        SCardTransmit(card->ctx, &card->pioSendPCI, (LPCBYTE)com.data(),
                      com.size(), NULL, (LPBYTE)r_apdu, &dwRecv);
        std::vector<char> res(r_apdu, r_apdu + dwRecv);
        if (isOK(res) == false) {
            throw CardException("close() failed: " +
                                ByteArrayUtils::toHex(res));
        }
    } catch (PCSCException e) {
        card->handleError(e);
        throw new CardException("Could not close channel"); //, e);
    }

    this->isClosed = true;
}

std::string CardChannel::toString()
{
    return StringHelper::formatSimple("PC/SC channel %d", channel);
}

int CardChannel::getSW(std::vector<char> res)
{
    if (res.size() < 2)
        throw CardException("Invalid response length: " + res.size());

    int sw1 = res[res.size() - 2] & 0xff;
    int sw2 = res[res.size() - 1] & 0xff;
    return (sw1 << 8) | sw2;
}

bool CardChannel::isOK(std::vector<char> res)
{
    return (res.size() == 2) && (getSW(res) == 0x9000);
}

void CardChannel::checkClosed()
{
    card->checkState();
    if (isClosed) {
        throw new IllegalStateException("Logical channel has been closed");
    }
}

void CardChannel::checkManageChannel(std::vector<char> b)
{
    if (b.size() < 4) {
        throw IllegalArgumentException("Command APDU must be at least 4 bytes long");
    }
    
    if ((b[0] >= 0) && (b[1] == 0x70)) {
        throw IllegalArgumentException("Manage channel command not allowed, use " \
                                        "openLogicalChannel()");
    }
}

void CardChannel::setChannel(std::vector<char> com)
{
    int cla = com[0];
    if (cla < 0) {
        /*
            * proprietary class format, cannot set or check logical channel
            * for now, just return
            */
        return;
    }

    /* classes 001x xxxx is reserved for future use in ISO, ignore */
    if ((cla & 0xe0) == 0x20) {
        return;
    }

    /* see ISO 7816/2005, table 2 and 3 */
    if (channel <= 3) {
        /*
            *mask of bits 7, 1, 0 (channel number)
            * 0xbc == 1011 1100
            */
        com[0] &= 0xbc;
        com[0] |= channel;
    } else if (channel <= 19) {
        /*
            * mask of bits 7, 3, 2, 1, 0 (channel number)
            * 0xbc == 1011 0000
            */
        com[0] &= 0xb0;
        com[0] |= 0x40;
        com[0] |= (channel - 4);
    } else {
        throw std::runtime_error("Unsupported channel number: " + channel);
    }
}

std::vector<char> CardChannel::concat(std::vector<char> b1,
                                      std::vector<char> b2, int n2)
{
    if ((b1.size() == 0) && (n2 == (int)b2.size()))
        return b2;

    std::vector<char> res(b1.begin(), b1.end());
    res.insert(res.end(), b2.begin(), b2.begin() + n2);

    return res;
}

std::vector<char> CardChannel::doTransmit(std::vector<char> command)
{
    /* To check */
    bool t0GetResponse = true;
    bool t1GetResponse = true;
    bool t1StripLe = true;

    /*
        *note that we modify the 'command' array in some cases, so it must
        * be a copy of the application provided data
        */
    try {
        checkManageChannel(command);
        setChannel(command);
        int n = command.size();
        bool t0 = card->protocol == SCARD_PROTOCOL_T0;
        bool t1 = card->protocol == SCARD_PROTOCOL_T1;
        if (t0 && (n >= 7) && (command[4] == 0)) {
            throw new CardException
                    ("Extended length forms not supported for T=0");
        }
        if ((t0 || (t1 && t1StripLe)) && (n >= 7)) {
            int lc = command[4] & 0xff;
            if (lc != 0) {
                if (n == lc + 6) {
                    n--;
                }
            } else {
                lc = ((command[5] & 0xff) << 8) | (command[6] & 0xff);
                if (n == lc + 9) {
                    n -= 2;
                }
            }
        }

        bool getresponse = (t0 && t0GetResponse) || (t1 && t1GetResponse);
        int k = 0;
        std::vector<char> result;
        while (true) {
            if (++k >= 32) {
                throw CardException("Could not obtain response");
            }
            char r_apdu[261];
            DWORD dwRecv = sizeof(r_apdu);        
            SCardTransmit(card->ctx, &card->pioSendPCI, (LPCBYTE)command.data(),
                          command.size(), NULL, (LPBYTE)r_apdu, &dwRecv);
            std::vector<char> response(r_apdu, r_apdu + dwRecv);
            int rn = response.size();
            if (getresponse && (rn >= 2)) {
                // see ISO 7816/2005, 5.1.3
                if ((rn == 2) && (response[0] == 0x6c)) {
                    // Resend command using SW2 as short Le field
                    command[n - 1] = response[1];
                    continue;
                }
                if (response[rn - 2] == 0x61) {
                    // Issue a GET RESPONSE command with the same CLA
                    // using SW2 as short Le field
                    if (rn > 2) {
                        result.insert(result.end(), response.begin(), response.begin() + rn - 2);
                    }
                    command[1] = static_cast<char>(0xC0);
                    command[2] = 0;
                    command[3] = 0;
                    command[4] = response[rn - 1];
                    n = 5;
                    continue;
                }
            }
            result.insert(result.end(), response.begin(), response.begin() + rn);
            break;
        }
        return result;
    } catch (PCSCException e) {
        card->handleError(e);
        throw CardException(""); //e);
    }
}

}
}
}
}
