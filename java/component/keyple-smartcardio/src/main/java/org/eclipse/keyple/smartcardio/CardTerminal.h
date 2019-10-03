/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#pragma once

#if defined(WIN32) || defined(__MINGW32__) || defined (__MINGW64__)
#  include <winscard.h>
#else
#  include <PCSC/winscard.h>
#  include <PCSC/wintypes.h>
#endif

/* Common */
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"

namespace org { namespace eclipse { namespace keyple { namespace smartcardio { class Card; }}}}

namespace org {
namespace eclipse {
namespace keyple {
namespace smartcardio {

using Card          = org::eclipse::keyple::smartcardio::Card;
using Logger        = org::eclipse::keyple::common::Logger;
using LoggerFactory = org::eclipse::keyple::common::LoggerFactory;

class EXPORT CardTerminal {
private:
    /**
     * The name of this terminal (native PC/SC name)
     */
    const std::string name;

    /**
     * 
     */
    Card* card;

    /**
     *
     */
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(CardTerminal));

public:
    /**
     * Native SCARDCONTEXT
     */
    SCARDCONTEXT ctx;

    /**
     * Returns the unique name of this terminal.
     *
     * @return the unique name of this terminal.
     */
    const std::string& getName();

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
    Card* connect(std::string protocol);

    /**
     * Returns whether a card is present in this terminal.
     *
     * @return whether a card is present in this terminal.
     *
     * @throws CardException if the status could not be determined
     */
    bool isCardPresent();

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
    bool waitForCardPresent(long timeout);

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
    bool waitForCardAbsent(long timeout);

    /**
     * 
     */
    std::string toString();

    /**
     * Constructs a new CardTerminal object.
     *
     * <p>This constructor is called by subclasses only. Application should
     * call {@linkplain CardTerminals#list list()}
     * or {@linkplain CardTerminals#getTerminal getTerminal()}
     * to obtain a CardTerminal object.
     */
    CardTerminal(SCARDCONTEXT ctx, const std::string& name);

private:
    /**
     *
     */
    bool waitForCard(bool wantPresent, long timeout);
};

}
}
}
}

