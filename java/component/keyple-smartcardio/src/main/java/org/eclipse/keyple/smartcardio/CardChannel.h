/*
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * All rights reserved. This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
 * available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
 */

#pragma once

/* Common */
#include "exceptionhelper.h"
#include "stringhelper.h"

/* Smartcard I/O */
#include "CardException.h"
#include "CommandAPDU.h"
#include "ResponseAPDU.h"

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

/*
 * Forward declaration
 */
namespace org { namespace eclipse { namespace keyple { namespace smartcardio { class Card; }}}}


/*
 *
 */
class CardChannel {
public:
private:
    /**
     * The card this channel is associated with
     */
    Card card;

    /**
     * The channel number, 0 for the basic logical channel
     */
    int channel;

    /**
     * Whether this channel has been closed. only logical channels can be
     * closed
     */
    bool isClosed;

public:
    /*
     * Constructs a new CardChannel object.
     *
     * This constructor is called by subclasses only. Application should call
     * the Card.getBasicChannel() and Card.openLogicalChannel() methods to
     * obtain a CardChannel object.
     */
    CardChannel(Card& card, int channel) : card(card), channel(channel)
    {
        
    }

    /*
     * public abstract Card getCard()
     *
     * Returns the Card this channel is associated with.
     *
     * @return the Card this channel is associated with
     */
    Card& getCard()
    {
        return card;
    }

    /*
     * Returns the channel number of this CardChannel. A channel number of
     * indicates the basic logical channel.
     *
     * @return the channel number of this CardChannel.
     * @throws IllegalStateException if this channel has been closed or if the
     *                               corresponding Card has been disconnected.
     */
    int getChannelNumber()
    {
        checkClosed();
        return channel;
    }

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
        checkClosed();
        card.checkExclusive();
        std::vector<char> commandBytes = command->getBytes();
        std::vector<char> responseBytes = doTransmit(commandBytes);
        return std::make_shared<ResponseAPDU>(responseBytes);
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
    int transmit(std::vector<char>& command, std::vector<char>& response)
    {
        checkClosed();
        card.checkExclusive();

        if (command.size() == 0 || command == response) {
            throw new IllegalArgumentException
                    ("command and response must not be the same object");
        }

        std::vector<char> resp = doTransmit(commandBytes);
        response.insert(resp.begin(), resp.end());

        return resp.size();
    }

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
    void close()
    {
        if (getChannelNumber() == 0)
            throw IllegalStateException("Cannot close basic logical channel");

        if (this->isClosed)
            return;

        card.checkExclusive();
        try {
            std::vector<char> com = {0x00, 0x70, (char)0x80, 0};
            com[3] = getChannelNumber();
            setChannel(com);

            uint8_t r_apdu[261];
            DWORD dwRecv = sizeof(r_apdu);
            SCardTransmit(card.ctx, &card.pioSendPCI, com.data, com.data(),
                          com.size(), r_apdu, &dwRecv);
            std::vector<char> rapdu(r_apdu);
            if (isOK(rapdu) == false) {
                throw new CardException("close() failed: " + PCSC.toString(res));
            }
        } catch (PCSCException e) {
            card.handleError(e);
            throw new CardException("Could not close channel", e);
        }

        this->isClosed = true;
    }

    std::string toString() {
        return StringHelper::formatSimple("PC/SC channel %d", channel);
    }

private:
    static int getSW(std::vector<char> res)
    {
        if (res.size() < 2)
            throw CardException("Invalid response length: " + res.size());

        int sw1 = res[res.size() - 2] & 0xff;
        int sw2 = res[res.size() - 1] & 0xff;
        return (sw1 << 8) | sw2;
    }

    static bool isOK(std::vector<char> res)
    {
        return (res.size() == 2) && (getSW(res) == 0x9000);
    }

    void checkClosed()
    {
        card.checkState();
        if (isClosed) {
            throw new IllegalStateException("Logical channel has been closed");
        }
    }

    void checkManageChannel(std::vector<char> b)
     {
        if (b.size() < 4) {
            throw IllegalArgumentException("Command APDU must be at least 4 bytes long");
        }
        
        if ((b[0] >= 0) && (b[1] == 0x70)) {
            throw IllegalArgumentException("Manage channel command not allowed, use " \
                                           "openLogicalChannel()");
        }
    }

    void setChannel(std::vector<char> com)
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

    std::vector<char> concat(std::vector<char> b1, std::vector<char> b2,
                             int n2)
    {
        if ((b1.size() == 0) && (n2 == b2.size()))
            return b2;

        std::vector<char> res(b1.begin(), b1.end());
        res.insert(b2.begin(), b2.begin() + n2);

        return res;
    }

    std::vector<char> doTransmit(std::vector<char> command)
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
            boolean t0 = card.protocol == SCARD_PROTOCOL_T0;
            boolean t1 = card.protocol == SCARD_PROTOCOL_T1;
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
            std::vector<char> result();
            while (true) {
                if (++k >= 32) {
                    throw CardException("Could not obtain response");
                }
                std::vector<char> response = SCardTransmit(card.cardId, card.protocol, command, 0, n);
                int rn = response.length;
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
                            result = concat(result, response, rn - 2);
                        }
                        command[1] = 0xC0;
                        command[2] = 0;
                        command[3] = 0;
                        command[4] = response[rn - 1];
                        n = 5;
                        continue;
                    }
                }
                result = concat(result, response, rn);
                break;
            }
            return result;
        } catch (PCSCException e) {
            card.handleError(e);
            throw new CardException(e);
        }
    }
};

}
}
}
}
