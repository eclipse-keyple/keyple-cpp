/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

 #pragma once

#include <algorithm> 
#include <string>

#if defined(WIN32) || defined(__MINGW32__) || defined (__MINGW64__)
#  include <winscard.h>
#else
#  include <PCSC/winscard.h>
#  include <PCSC/wintypes.h>
#endif

/* Smartcard I/O */
#include "CardException.h"

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"
#include "System.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace smartcardio {

/* Forward declaration */
namespace org { namespace eclipse { namespace keyple { namespace smartcardio { class Card; }}}}

class CardTerminal {
public:
    
    /** Native SCARDCONTEXT */
    SCARDCONTEXT ctx;

private:
    /** The name of this terminal (native PC/SC name) */
    std::string name;

    /** */
    Card* card;

public:
    /**
     * Returns the unique name of this terminal.
     *
     * @return the unique name of this terminal.
     */
    std::string getName()
    {
        return name;
    }

    /**
     * Establishes a connection to the card.
     * If a connection has previously established using
     * the specified protocol, this method returns the same Card object as
     * the previous call.
     *
     * @param protocol the protocol to use ("T=0", "T=1", or "T=CL"), or "*" to
     *   connect using any available protocol.
     *
     * @throws NullPointerException if protocol is null
     * @throws IllegalArgumentException if protocol is an invalid protocol
     *   specification
     * @throws CardNotPresentException if no card is present in this terminal
     * @throws CardException if a connection could not be established
     *   using the specified protocol or if a connection has previously been
     *   established using a different protocol
     * @throws SecurityException if a SecurityManager exists and the
     *   caller does not have the required
     *   {@linkplain CardPermission permission}
     */
    Card* connect(std::string protocol)
    {
        if (card != NULL) {
            if (card->isValid()) {
                std::string cardProto = card->getProtocol();
                if (!protocol.compare("*") || !protocol.compare(cardProto)) {
                    return card;
                } else {
                    throw new CardException("Cannot connect using " + protocol
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
                throw new CardNotPresentException("No card present");
            } else {
                throw new CardException("connect() failed");
            }
        }
    }

    /**
     * Returns whether a card is present in this terminal.
     *
     * @return whether a card is present in this terminal.
     *
     * @throws CardException if the status could not be determined
     */
    bool isCardPresent()
    {
        try {
            SCARD_READERSTATE state;
            state.szReader = name.c_str();
            state.dwCurrentState =SCARD_STATE_UNAWARE;

            LONG status = SCardGetStatusChange(this->ctx, 0, &state, 1);
            return (state.dwCurrentState & SCARD_STATE_PRESENT) != 0;
        } catch (PCSCException e) {
            throw new CardException("isCardPresent() failed"); //, e);
        }
    }

    /**
     * Waits until a card is present in this terminal or the timeout
     * expires. If the method returns due to an expired timeout, it returns
     * false. Otherwise it return true.
     *
     * <P>If a card is present in this terminal when this
     * method is called, it returns immediately.
     *
     * @param timeout if positive, block for up to <code>timeout</code>
     *   milliseconds; if zero, block indefinitely; must not be negative
     * @return false if the method returns due to an expired timeout,
     *   true otherwise.
     *
     * @throws IllegalArgumentException if timeout is negative
     * @throws CardException if the operation failed
     */
    bool waitForCardPresent(long timeout)
    {
        return waitForCard(true, timeout);
    }

    /**
     * Waits until a card is absent in this terminal or the timeout
     * expires. If the method returns due to an expired timeout, it returns
     * false. Otherwise it return true.
     *
     * <P>If no card is present in this terminal when this
     * method is called, it returns immediately.
     *
     * @param timeout if positive, block for up to <code>timeout</code>
     *   milliseconds; if zero, block indefinitely; must not be negative
     * @return false if the method returns due to an expired timeout,
     *   true otherwise.
     *
     * @throws IllegalArgumentException if timeout is negative
     * @throws CardException if the operation failed
     */
    bool waitForCardAbsent(long timeout)
    {
        return waitForCard(false, timeout);
    }

    std::string toString()
    {
        return "PC/SC terminal " + name;
    }

    /**
     * Constructs a new CardTerminal object.
     *
     * <p>This constructor is called by subclasses only. Application should
     * call {@linkplain CardTerminals#list list()}
     * or {@linkplain CardTerminals#getTerminal getTerminal()}
     * to obtain a CardTerminal object.
     */
    CardTerminal(SCARDCONTEXT ctx, const std::string& name)
    : ctx(ctx), name(name)
    {

    }

private:
    /**
     *
     */
    bool waitForCard(bool wantPresent, long timeout)
    {
        if (timeout < 0)
            throw new IllegalArgumentException("timeout must not be negative");

        if (timeout == 0)
            timeout = INFINITE;

        SCARD_READERSTATE status;
        status.dwCurrentState = SCARD_STATE_UNAWARE;
        status.szReader = name.c_str();

        try {
            /* Check if card status already matches */
            SCardGetStatusChange(this->ctx, 0, &status, 1);
            boolean present = (status.dwCurrentState & SCARD_STATE_PRESENT) != 0;
            if (wantPresent == present) {
                return true;
            }

            /* No match, wait (until timeout expires) */
            long end = System::currentTimeMillis() + timeout;
            while (wantPresent != present && timeout != 0) {
                /* Set remaining timeout */
                if (timeout != INFINITE)
                    timeout = max(end - System::currentTimeMillis(), 0l);

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
};

}
}
}
}

