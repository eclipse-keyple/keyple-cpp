/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#ifndef KEYPLE_PLUGIN_PCSC_CARD_CHANNEL_H
#define KEYPLE_PLUGIN_PCSC_CARD_CHANNEL_H

#include "CommandAPDU.h"
#include "ResponseAPDU.h"

using namespace keyple::plugin::pcsc;

namespace keyple {
    namespace plugin {
        namespace pcsc {

            /*
             * Forward declaration
             */
            class Card;

            /*
             *
             */
            class CardChannel {
            public:
                /*
                 * Constructs a new CardChannel object.
                 *
                 * This constructor is called by subclasses only. Application should call
                 * the Card.getBasicChannel() and Card.openLogicalChannel() methods to
                 * obtain a CardChannel object.
                 */
                CardChannel()
                {

                }

                /*
                 *
                 */
                ~CardChannel()
                {

                }

                /*
                 * public abstract Card getCard()
                 *
                 * Returns the Card this channel is associated with.
                 *
                 * @return the Card this channel is associated with
                 */
                Card& getCard();

                /*
                 * Returns the channel number of this CardChannel. A channel number of
                 * indicates the basic logical channel.
                 *
                 * @return the channel number of this CardChannel.
                 * @throws IllegalStateException if this channel has been closed or if the
                 *                               corresponding Card has been disconnected.
                 */
                int getChannelNumber();

                /*
                 * Transmits the specified command APDU to the Smart Card and returns the
                 * response APDU.
                 *
                 * The CLA byte of the command APDU is automatically adjusted to match the
                 * channel number of this CardChannel.
                 *
                 * Note that this method cannot be used to transmit MANAGE CHANNEL APDUs.
                 * Logical channels should be managed using the Card.openLogicalChannel()
                 * and CardChannel.close() methods.
                 *
                 * Implementations should transparently handle artifacts of the
                 * transmission protocol. For example, when using the T=0 protocol, the
                 * following processing should occur as described in ISO/IEC 7816-4:
                 *  - if the response APDU has an SW1 of 61, the implementation should
                 *    issue a GET RESPONSE command using SW2 as the Lefield. This process
                 *    is repeated as long as an SW1 of 61 is received. The response body
                 *    of these exchanges is concatenated to form the final response body.
                 *  - if the response APDU is 6C XX, the implementation should reissue the
                 *    command using XX as the Le field.
                 *
                 * The ResponseAPDU returned by this method is the result after this
                 * processing has been performed.
                 *
                 * @param command the command APDU
                 * @return the response APDU received from the card
                 * @throws IllegalStateException if this channel has been closed or if the
                 *                               corresponding Card has been disconnected.
                 * @throws IllegalArgumentException if the APDU encodes a MANAGE CHANNEL
                 *                                  command
                 * @throws NullPointerException if command is null
                 * @throws CardException if the card operation failed
                 */
                std::shared_ptr<ResponseAPDU> transmit(std::shared_ptr<CommandAPDU> command)
                {
                    return std::shared_ptr<ResponseAPDU>(new ResponseAPDU(command->getApdu()));
                }

                /*
                 * Transmits the command APDU stored in the command ByteBuffer and receives
                 * the reponse APDU in the response ByteBuffer.
                 *
                 * The command buffer must contain valid command APDU data starting at
                 * command.position() and the APDU must be command.remaining() bytes long.
                 * Upon return, the command buffer's position will be equal to its limit;
                 * its limit will not have changed. The output buffer will have received
                 * the response APDU bytes. Its position will have advanced by the number
                 * of bytes received, which is also the return value of this method.
                 *
                 * The CLA byte of the command APDU is automatically adjusted to match the
                 * channel number of this CardChannel.
                 *
                 * Note that this method cannot be used to transmit MANAGE CHANNEL APDUs.
                 * Logical channels should be managed using the Card.openLogicalChannel()
                 * and CardChannel.close() methods.
                 *
                 * See transmit() for a discussion of the handling of response APDUs with
                 * the SW1 values 61 or 6C.
                 *
                 * @param command the buffer containing the command APDU
                 * @param response the buffer that shall receive the response APDU from the
                 *                 card
                 * @return the length of the received response APDU
                 * @throws IllegalStateException if this channel has been closed or if the
                 *                               corresponding Card has been disconnected.
                 * @throws NullPointerException if command or response is null
                 * @throws ReadOnlyBufferException if the response buffer is read-only
                 * @throws IllegalArgumentException if command and response are the same
                 *                                  object, if response may not have
                 *                                  sufficient space to receive the
                 *                                  response APDU or if the APDU encodes
                 *                                  a MANAGE CHANNEL command
                 * @throws CardException if the card operation failed
                 */
                //int transmit(ByteBuffer command, ByteBuffer response);

                /*
                 * Closes this CardChannel. The logical channel is closed by issuing a
                 * MANAGE CHANNEL command that should use the format [xx 70 80 0n] where n
                 * is the channel number of this channel and xx is the CLA byte that
                 * encodes this logical channel and has all other bits set to 0. After
                 * this method returns, calling other methods in this class will raise
                 * an IllegalStateException.
                 *
                 * Note that the basic logical channel cannot be closed using this method.
                 * It can be closed by calling Card.disconnect(boolean).
                 *
                 * @throws CardException if the card operation failed
                 * @throws IllegalStateException if this CardChannel represents a
                 *                               connection the basic logical channel
                 */
                void close();
            };
        }
    }
}

#endif /* KEYPLE_PLUGIN_PCSC_CARD_CHANNEL_H */
