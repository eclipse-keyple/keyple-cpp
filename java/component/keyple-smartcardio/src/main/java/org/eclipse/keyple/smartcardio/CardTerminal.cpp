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
        DWORD atrLen = sizeof(pbAtr);
        SCARDHANDLE hCard;
        DWORD chReaderLen = 100; //strlen(name.c_str());
        LONG rv;

        logger->debug("isCardPresent - retrieving card status on %s\n",
                      name.c_str());

        rv = SCardConnect(this->ctx, (LPSTR)name.c_str(), SCARD_SHARE_SHARED,
                          SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &hCard,
                          &protocol);
        if (rv != SCARD_S_SUCCESS) {
                logger->debug("isCardPresent - error connecting to card\n");
                return false;
        }

        rv = SCardStatus(hCard, (LPSTR)name.c_str(), &chReaderLen, &state,
                         &protocol, pbAtr, &atrLen);
        if (rv != SCARD_S_SUCCESS) {
                logger->debug("isCardPresent - error retrieving status (%x)\n",
                              rv);
                return false;
        }

        logger->debug("isCardPresent - current state: %x (vs. %x)\n",
                      state, SCARD_PRESENT);

        SCardDisconnect(hCard, SCARD_UNPOWER_CARD);

        return (state & SCARD_PRESENT || state & SCARD_POWERED) != 0;
    } catch (PCSCException e) {
        throw CardException("isCardPresent() failed"); //, e);
    }
}

bool CardTerminal::waitForCardPresent(long timeout)
{
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
    if (timeout < 0)
        throw new IllegalArgumentException("timeout must not be negative");

    if (timeout == 0)
        timeout = INFINITE;

    SCARD_READERSTATE status;
    status.dwCurrentState = SCARD_STATE_UNAWARE;
    status.szReader = this->name.c_str();

    try {
        /* Check if card status already matches */
        SCardGetStatusChange(this->ctx, 0, &status, 1);
        bool present = (status.dwCurrentState & SCARD_STATE_PRESENT) != 0;
        if (wantPresent == present) {
            return true;
        }

        /* No match, wait (until timeout expires) */
        long end = System::currentTimeMillis() + timeout;
        while (wantPresent != present && timeout != 0) {
            /* Set remaining timeout */
            if (timeout != INFINITE)
                timeout = std::max(end - System::currentTimeMillis(), 0l);

            SCardGetStatusChange(this->ctx, timeout, &status, 1);
            present = (status.dwCurrentState & SCARD_STATE_PRESENT) != 0;
        }
        return wantPresent == present;
    } catch (PCSCException e) {
        if (e.code == SCARD_E_TIMEOUT) {
            return false;
        } else {
            throw CardException("waitForCard() failed"); //, e);
        }
    }
}

}
}
}
}

