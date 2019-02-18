/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

 #ifndef KEYPLE_PLUGIN_PCSC_CARD_TERMINAL_H
 #define KEYPLE_PLUGIN_PCSC_CARD_TERMINAL_H

#include <string>

#if defined(WIN32) || defined(__MINGW32__) || defined (__MINGW64__)
#  include <winscard.h>
#else
#  include <PCSC/winscard.h>
#  include <PCSC/wintypes.h>
#endif

/* Smartcard I/O */
#include "Card.h"
#include "CardException.h"

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

namespace keyple {
namespace plugin {
namespace pcsc {

/**
 * \class CardTerminal
 */
class CardTerminal {

public:
    /**
     * Establishes a connection to the card.
     *
     * @param protocol the protocol to use ("T=0", "T=1", or "T=CL"), or "*" to connect
     *                 using any available protocol.
     * @throws NullPointerException if protocol is null
     * @throws IllegalArgumentException if protocol is an invalid protocol
     *                                  specification
     * @throws CardNotPresentException if no card is present in this terminal
     * @throws CardException if a connection could not be established using the
     *                       specified protocol or if a connection has previously
     *                       been established using a different protocol
     * @throws SecurityException if a SecurityManager exists and the caller does not
     *                           have the required permission
     */
    std::shared_ptr<Card> connect(std::string protocol)
    {
        logger->debug("[CardTerminal::connect] connecting to protocol %s\n", protocol);
        LONG status = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &context);
        if (status != SCARD_S_SUCCESS) {
            logger->debug("[CardTerminal::CardTerminal] error establishing context (%x)\n", status);
            throw CardException("could not establish context");
        }

        return std::make_shared<Card>(card);
    }

    /**
     * Returns the unique name of this terminal.
     *
     * @return the unique name of this terminal
     */
    std::string &getName()
    {
        return name;
    }

    /**
     * Returns whether a card is present in this terminal.
     *
     * @return whether a card is present in this terminal.
     * @throws CardException if the status could not be determined
     */
    bool isCardPresent()
    {
        return isPresent;
    }

    /**
     * Waits until a card is absent in this terminal or the timeout expires. If the
     * method returns due to an expired timeout, it returns false. Otherwise it return
     * true.
     *
     * If no card is present in this terminal when this method is called, it returns
     * immediately.
     *
     * @param timeout if positive, block for up to timeout milliseconds; if zero, block
     *                indefinitely; must not be negative
     * @return false if the method returns due to an expired timeout, true otherwise.
     * @throws IllegalArgumentException if timeout is negative
     * @throws CardException if the operation failed
     */
    bool waitForCardAbsent(long timeout)
    {
        return false;
    }

    /**
     * Waits until a card is present in this terminal or the timeout expires.
     *
     * If the method returns due to an expired timeout, it returns false. Otherwise it
     * return true.
     *
     * If a card is present in this terminal when this method is called, it returns
     * immediately.
     *
     * @param timeout if positive, block for up to timeout milliseconds; if zero, block
     *                indefinitely; must not be negative
     * @return false if the method returns due to an expired timeout, true otherwise.
     * @throws IllegalArgumentException if timeout is negative
     * @throws CardException if the operation failed
     */
    bool waitForCardPresent(long timeout)
    {
        return false;
    }

    /**
     * Constructor
     *
     * Constructs a new CardTerminal object.
     *
     */
    CardTerminal(std::string &name)
    : name(name)
    {
        logger = LoggerFactory::getLogger(typeid(this));

        logger->debug("[CardTerminal::CardTerminal]\n");
    }

    /**
     * Destructor
     *
     */
    ~CardTerminal()
    {

    }

private:
    /**
     *
     */
    SCARDCONTEXT context;

    /**
     *
     */
    Card card;

    /**
     *
     */
    std::string name;

    /**
     *
     */
    bool isPresent;

    /**
     *
     */
    std::shared_ptr<Logger> logger;
};
}
}
}

#endif /* KEYPLE_PLUGIN_PCSC_CARD_TERMINAL_H */
