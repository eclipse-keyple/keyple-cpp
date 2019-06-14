/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#include <algorithm>
#include <cstring>

/* Common */
#include "exceptionhelper.h"
#include "System.h"

/* Smartcard I/O */
#include "Card.h"
#include "CardTerminal.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace smartcardio {

using Card           = org::eclipse::keyple::smartcardio::Card;

std::string CardTerminal::getName()
{
    return name;
}

Card* CardTerminal::connect(std::string protocol)
{
    logger->debug("connect\n");

    if (card != NULL) {
        if (card->isValid()) {
            std::string cardProto = card->getProtocol();
            if (!protocol.compare("*") || !protocol.compare(cardProto)) {
                return card;
            } else {
                throw CardException("Cannot connect using " + protocol
                    + ", connection already established using " + cardProto);
            }
        } else {
            card = NULL;
        }
    }
    try {
        return new Card(this, protocol);
    } catch (PCSCException e) {
        if (e.code == SCARD_W_REMOVED_CARD ||
            e.code == SCARD_E_NO_SMARTCARD) {
            throw CardNotPresentException("No card present");
        } else {
            throw CardException("connect() failed");
        }
    }
}

bool CardTerminal::isCardPresent()
{
    logger->debug("isCardPresent -\n");

    try {
        DWORD state;
        DWORD protocol;
        BYTE pbAtr[261];
        SCARDHANDLE hCard;
        DWORD atrLen = sizeof(pbAtr);
        DWORD chReaderLen = strlen(name.c_str()) + 1;
        LONG rv;

        logger->debug("isCardPresent - connecting to card\n");

        rv = SCardConnect(this->ctx, (LPSTR)name.c_str(), SCARD_SHARE_SHARED,
                          SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &hCard,
                          &protocol);
        if (rv != SCARD_S_SUCCESS) {
                logger->debug("isCardPresent - error connecting to card\n");
                return false;
        }

        logger->debug("isCardPresent - retrieving card status on %s\n",
                      name.c_str());

        rv = SCardStatus(hCard, (LPSTR)name.c_str(), &chReaderLen,
                         &state, &protocol, pbAtr, &atrLen);
        if (rv != SCARD_S_SUCCESS) {
                logger->debug("isCardPresent - error retrieving status (%x)\n",
                              rv);
                return false;
        }

        logger->debug("isCardPresent - current state: %x (vs. %x)\n",
                      state, SCARD_PRESENT);

        logger->debug("isCardPresent - disconnecting\n");

        SCardDisconnect(hCard, SCARD_LEAVE_CARD);

        return (state & SCARD_PRESENT || state & SCARD_POWERED) != 0;
    } catch (PCSCException e) {
        throw CardException("isCardPresent() failed"); //, e);
    }
}

bool CardTerminal::waitForCardPresent(long timeout)
{
    logger->debug("waitForCardPresent - timeout: %d\n", timeout);

    return waitForCard(true, timeout);
}

bool CardTerminal::waitForCardAbsent(long timeout)
{
    return waitForCard(false, timeout);
}

std::string CardTerminal::toString()
{
    return "PC/SC terminal " + name;
}

CardTerminal::CardTerminal(SCARDCONTEXT ctx, const std::string& name)
: ctx(ctx), name(name)
{

}

bool CardTerminal::waitForCard(bool wantPresent, long timeout)
{
    bool present = !wantPresent;
    SCARD_READERSTATE status[1];
    long start = System::currentTimeMillis();
    long current; 

    logger->debug("waitForCard - wantPresent: %d, timeout: %dl\n", wantPresent,
                  timeout);

    if (timeout < 0)
        throw new IllegalArgumentException("timeout must not be negative");

    if (timeout == 0)
        timeout = INFINITE;

    status[0].dwCurrentState = SCARD_STATE_UNAWARE;
    status[0].szReader = this->name.c_str();

    try {
        /*
         * Check if card status already matches
         *
         * Note: if executed properly SCardGetStatusChange blocks until the
         * current availability of the card changes.
         */
        logger->debug("waitForCard - waiting for card status change on %s\n",
                      this->name.c_str());

        while (wantPresent != present && timeout != 0) {
            LONG rv = SCardGetStatusChange(this->ctx, 50, status, 1);
            if (rv != SCARD_S_SUCCESS) {
                logger->debug("waitForCard - error in SCardGetStatusChange " \
                              "(%d)\n", rv);
                return false;
            }

            present = (status[0].dwEventState & SCARD_STATE_PRESENT) != 0;
            if (wantPresent == present)
                return true;

            if (timeout != INFINITE) {
                current = System::currentTimeMillis();
                if (current > (start + timeout)) {
                    logger->debug("waitForCard - timeout\n");
                    return false;
                }
            }

	    Thread::sleep(50);
        }
    } catch (PCSCException e) {
        if (e.code == SCARD_E_TIMEOUT) {
            return false;
        } else {
            throw CardException("waitForCard() failed"); //, e);
        }
    }

    return false;
}

}
}
}
}

