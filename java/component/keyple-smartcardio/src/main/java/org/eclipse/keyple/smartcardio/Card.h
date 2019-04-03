/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef KEYPLE_PLUGIN_PCSC_CARD_H
#define KEYPLE_PLUGIN_PCSC_CARD_H

/* Smartcard I/O */
#include "CardChannel.h"
#include "CardException.h"

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

namespace keyple {
    namespace plugin {
        namespace pcsc {

            using Logger = org::eclipse::keyple::common::Logger;
            using LoggerFactory = org::eclipse::keyple::common::LoggerFactory;

            /**
             * @class Card
             *
             */
            class Card {
            public:
                /**
                 * Constructor
                 *
                 * Constructs a new Card object.
                 *
                 * This constructor is called by subclasses only. Application should call list() or
                 * getTerminal() to obtain a CardTerminal object.
                 */
                Card()
                {
                    logger->debug("Card::Card\n");
                }

                /**
                 * Destructor
                 *
                 */
                ~Card()
                {

                }

                /**
                 * Returns the CardChannel for the basic logical channel. The basic logical
                 * channel has a channel number of 0.
                 *
                 * @throws SecurityException if a SecurityManager exists and the caller does
                 *                           not have the required permission
                 * @throws IllegalStateException if this card object has been disposed of via
                 *                               the disconnect() method
                 */
                std::shared_ptr<CardChannel> getBasicChannel()
                {
                    logger->debug("Card::getBasicChannel\n");

                    return std::make_shared<CardChannel>(channel);
                }


                /**
                 * Returns the ATR of this card.
                 *
                 * @return the ATR of this card.
                 */
                std::vector<char> getATR()
                {
                    return atr;
                }

                /**
                 * Requests exclusive access to this card.
                 *
                 * Once a thread has invoked beginExclusive, only this thread is
                 * allowed to communicate with this card until it calls
                 * endExclusive. Other threads attempting communication will
                 * receive a CardException.
                 *
                 * Applications have to ensure that exclusive access is
                 * correctly released. This can be achieved by executing the
                 * beginExclusive() and endExclusive calls in a try ... finally
                 * block.
                 *
                 * @throw SecurityException if a SecurityManager exists and the
                 *        caller does not have the required permission
                 * @throw CardException if exclusive access has already been set
                 *        or if exclusive access could not be established
                 * @throw IllegalStateException if this card object has been
                 *        disposed of via the disconnect() method
                 */
                void beginExclusive() throw(CardException)
                {

                }

                /**
                 * Releases the exclusive access previously established using beginExclusive.
                 *
                 * @throws SecurityException if a SecurityManager exists and the caller does not have the required
                 *                           permission
                 * @throws llegalStateException if the active Thread does not currently have exclusive access to this
                 *                              card or if this card object has been disposed of via the disconnect()
                 *                              method
                 * @throws CardException if the operation failed
                 */
                 void endExclusive()
                 {

                 }

                /**
                 * Disconnects the connection with this card. After this method
                 * returns, calling methods on this object or in CardChannels
                 * associated with this object that require interaction with
                 * the card will raise an IllegalStateException.
                 *
                 * @param reset whether to reset the card after disconnecting.
                 *
                 * @throw CardException if the card operation failed
                 * @throw SecurityException if a SecurityManager exists and the
                 *        caller does not have the required permission
                 */
                void disconnect(bool reset) throw(CardException)
                {
                    (void)reset;
                }

            private:
                /**
                 *
                 */
                CardChannel channel;

                /**
                 *
                 */
                std::vector<char> atr;

                /**
                 *
                 */
                const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(this));
            };
        }
    }
}

#endif /* KEYPLE_PLUGIN_PCSC_CARD_H */
