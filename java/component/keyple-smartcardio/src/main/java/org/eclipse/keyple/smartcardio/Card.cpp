/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

/* Smartcard I/O */
#include "ATR.h"
#include "Card.h"
#include "CardChannel.h"
#include "CardTerminal.h"
#include "PCSCException.h"

/* Common */
#include "exceptionhelper.h"

/* Core */
#include "ByteArrayUtil.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace smartcardio {

using ATR            = org::eclipse::keyple::smartcardio::ATR;
using ByteArrayUtil  = org::eclipse::keyple::core::util::ByteArrayUtil;
using CardTerminal   = org::eclipse::keyple::smartcardio::CardTerminal;
using Logger         = org::eclipse::keyple::common::Logger;
using PCSCException  = org::eclipse::keyple::smartcardio::PCSCException;

std::vector<char> commandOpenChannel = {0, 0x70, 0, 0, 1};

Card::Card(CardTerminal* terminal, std::string protocol) : terminal(terminal)
{
    int sharingMode = SCARD_SHARE_SHARED;
    int connectProtocol;
    DWORD state;
    BYTE _atr[33];
    DWORD atrLen = sizeof(_atr);
    DWORD rLen = strlen(terminal->name.c_str()) + 1;
    LONG rv;

    logger->debug("constructor\n");

    exclusiveThread = NULL;

    if (!protocol.compare("*")) {
        connectProtocol = SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1;
    } else if (!protocol.compare("T=0")) {
        connectProtocol = SCARD_PROTOCOL_T0;
    } else if (!protocol.compare("T=1")) {
        connectProtocol = SCARD_PROTOCOL_T1;
    } else if (!protocol.compare("direct")) {
        // testing
        connectProtocol = 0;
        sharingMode = SCARD_SHARE_DIRECT;
    } else {
        throw IllegalArgumentException("Unsupported protocol " + protocol);
    }

    logger->debug("connecting with protocol: %s and connectProtocol: %d, "\
                  "sharing mode: %d\n", protocol, connectProtocol,
                  sharingMode);

    rv = SCardConnect(terminal->ctx, terminal->name.c_str(), sharingMode,
                      connectProtocol, &this->cardhdl, &this->protocol);
    if (rv != SCARD_S_SUCCESS) {
        logger->debug("constructor - error connecting to reader (%d)\n", rv);
    }

    switch(this->protocol) {
    case SCARD_PROTOCOL_T0:
        this->pioSendPCI = *SCARD_PCI_T0;
        break;
    case SCARD_PROTOCOL_T1:
        this->pioSendPCI = *SCARD_PCI_T1;
        break;
    }

    basicChannel = new CardChannel(this, 0);
    this->state = State::OK;

    rv = SCardStatus(this->cardhdl, (LPSTR)terminal->name.c_str(),
                     &rLen, &state, &this->protocol, _atr, &atrLen);
    if (rv != SCARD_S_SUCCESS) {
        logger->debug("constructor - error retrieving status (%d)\n", rv);
    } else {
        std::vector<char> __atr(_atr, _atr + atrLen);
        logger->debug("constructor - atr: %s\n",
                      ByteArrayUtil::toHex(__atr));
        atr = new ATR(__atr);
    }
}

ATR* Card::getATR()
{
    logger->debug("getATR - returning public member ATR *atr (%p)\n", atr);

    return atr;
}

std::string Card::getProtocol()
{
    logger->debug("getProtocol\n");

    switch (protocol) {
    case SCARD_PROTOCOL_T0:
        return "T=0";
    case SCARD_PROTOCOL_T1:
        return "T=1";
    default:
        // should never occur
        return "Unknown protocol " + protocol;
    }
}

CardChannel* Card::getBasicChannel()
{
    logger->debug("getBasicChannel\n");

    checkSecurity("getBasicChannel");
    checkState();

    return basicChannel;
}

CardChannel* Card::openLogicalChannel()
{
    logger->debug("openLogicalChannel\n");

    checkSecurity("openLogicalChannel");
    checkState();
    checkExclusive();

    try {
        char r_apdu[261];
        DWORD dwRecv = sizeof(r_apdu);

        SCardTransmit(this->cardhdl, &this->pioSendPCI,
                      (LPCBYTE)commandOpenChannel.data(),
                      commandOpenChannel.size(), NULL, (LPBYTE)r_apdu, &dwRecv);

        std::vector<char> response(r_apdu, r_apdu + dwRecv);
        if ((response.size() != 3) || (getSW(response) != 0x9000))
            throw CardException("openLogicalChannel() failed, card response: "
                                + ByteArrayUtil::toHex(response));

        return new CardChannel(this, response[0]);
    } catch (PCSCException e) {
        handleError(e);
        throw CardException("openLogicalChannel() failed"); //, e);
    }
}

void Card::beginExclusive()
{
    checkSecurity("exclusive");
    checkState();

    if (exclusiveThread != NULL)
        throw CardException("Exclusive access has already been assigned to " \
                            "Thread " + exclusiveThread->getName());

    try {
        SCardBeginTransaction(this->cardhdl);
    } catch (PCSCException e) {
        handleError(e);
        throw CardException("beginExclusive() failed"); //, e);
    }

    /* Alex: fix that bit */
    //exclusiveThread->setThread(Thread::currentThreadId());
}

void Card::endExclusive()
{
    checkState();
    if (exclusiveThread->selfId() != Thread::currentThreadId())
        throw IllegalStateException("Exclusive access not assigned to current" \
                                    " Thread");


    try {
        SCardEndTransaction(this->cardhdl, SCARD_LEAVE_CARD);
    } catch (PCSCException e) {
        handleError(e);
        throw CardException("endExclusive() failed"); //, e);
    }

    exclusiveThread = NULL;
}

void Card::disconnect(bool reset)
{
    if (reset)
        checkSecurity("reset");

    if (state != State::OK)
        return;

    checkExclusive();
    try {
        SCardDisconnect(this->cardhdl, (reset ? SCARD_LEAVE_CARD:SCARD_RESET_CARD));
    } catch (PCSCException e) {
        throw CardException("disconnect() failed"); //, e);
    }

    state = State::DISCONNECTED;
    exclusiveThread = NULL;
}

bool Card::isValid()
{
    if (state != State::OK)
        return false;

    /* Ping card via SCardStatus */
    try {
        DWORD state, protocol;
        BYTE attr[32];
        DWORD b = 32;
        DWORD cch = terminal->name.length();
        SCardStatus(this->cardhdl, (LPSTR)terminal->name.c_str(),
                    &cch, &state, &protocol,
                    attr, &b);
        return true;
    } catch (PCSCException e) {
        state = State::REMOVED;
        return false;
    }
}

void Card::checkExclusive()
{
    logger->debug("checkExclusive\n");

    Thread *t = exclusiveThread;
    if (!t) {
        logger->debug("exclusiveThread is null\n");
        return;
    }

    logger->debug("checkExclusive - t: %p\n", t);
    logger->debug("checkExclusive - t->selfId: %d\n", t->selfId());

    if (t->selfId() != Thread::currentThreadId())
        throw new CardException("Exclusive access established by another Thread"); //, e);
}

void Card::checkState()
{
    State s = state;
    if (s == State::DISCONNECTED)
        throw IllegalStateException("Card has been disconnected");
    else if (s == State::REMOVED)
        throw IllegalStateException("Card has been removed");
}

void Card::checkSecurity(std::string action)
{
    (void)action;

    /* Always good */
}

void Card::handleError(PCSCException e)
{
    if (e.code == SCARD_W_REMOVED_CARD)
        this->state = State::REMOVED;
}

int Card::getSW(std::vector<char> b)
{
    if (b.size() < 2)
        return -1;

    int sw1 = b[b.size() - 2] & 0xff;
    int sw2 = b[b.size() - 1] & 0xff;

    return (sw1 << 8) | sw2;
}

}
}
}
}
