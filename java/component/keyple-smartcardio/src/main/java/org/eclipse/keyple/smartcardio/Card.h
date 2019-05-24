/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#pragma once

/* Smartcard I/O */
#include "ATR.h"
#include "CardChannel.h"
#include "CardException.h"
#include "CardTerminal.h"

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace smartcardio {

using ATR           = org::eclipse::keyple::smartcardio::ATR;
using CardTerminal  = org::eclipse::keyple::smartcardio::CardTerminal;
using Logger        = org::eclipse::keyple::common::Logger;
using LoggerFactory = org::eclipse::keyple::common::LoggerFactory;

class Card {
public:
    /** */
    enum State {
        OK = 0,
        REMOVED,
        DISCONNECTED
    };

    /** Protocol in use, one of SCARD_PROTOCOL_T0 and SCARD_PROTOCOL_T1 */
    DWORD protocol;

    /** The native SCARDHANDLE */
    SCARDHANDLE ctx;

    /** */
    SCARD_IO_REQUEST pioSendPCI;

private:
    /** */
    CardChannel* channel;

    /** ATR of this card */
    std::vector<char> atr;

    /** */
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(Card));

    /** The terminal that created this card */
    CardTerminal* terminal;

    /** The basic logical channel (channel 0) */
    CardChannel* basicChannel;

    /* State of this card connection */
    State state;

    /** Thread holding exclusive access to the card, or null */
    Thread *exclusiveThread;

public:
    /**
     * Constructor
     *
     * Constructs a new Card object.
     *
     * This constructor is called by subclasses only. Application should call list() or
     * getTerminal() to obtain a CardTerminal object.
     */
    Card(CardTerminal* terminal, std::string protocol) : terminal(terminal)
    {
        logger->debug("constructor\n");

        int sharingMode = SCARD_SHARE_SHARED;
        int connectProtocol;
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

        SCardConnect(terminal->ctx, terminal->name, sharingMode,
                     connectProtocol, &this->ctx, &this->protocol);

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
        logger->debug("getBasicChannel\n");

        return std::make_shared<CardChannel>(channel);
    }


    /**
     * Returns the ATR of this card.
     *
     * @return the ATR of this card.
     */
    ATR& getATR()
    {
        logger->debug("getATR\n");

        return atr;
    }

    std::string getProtocol()
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

    CardChannel& getBasicChannel()
    {
        logger->debug("getBasicChannel\n");

        checkSecurity("getBasicChannel");
        checkState();

        return basicChannel;
    }

    CardChannel* openLogicalChannel()
    {
        logger->debug("openLogicalChannel\n");

        checkSecurity("openLogicalChannel");
        checkState();
        checkExclusive();

        try {
            byte[] response = SCardTransmit(cardId, protocol, commandOpenChannel, 0, commandOpenChannel.size());
            if ((response.length != 3) || (getSW(response) != 0x9000))
                throw new CardException
                        ("openLogicalChannel() failed, card response: "
                        + PCSC.toString(response));

            return new Channel(this, response[0]);
        } catch (PCSCException e) {
            handleError(e);
            throw new CardException("openLogicalChannel() failed", e);
        }
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
    void beginExclusive()
    {
        checkSecurity("exclusive");
        checkState();

        if (exclusiveThread != null)
            throw new CardException
                    ("Exclusive access has already been assigned to Thread "
                    + exclusiveThread.getName());

        try {
            SCardBeginTransaction(cardId);
        } catch (PCSCException e) {
            handleError(e);
            throw new CardException("beginExclusive() failed", e);
        }
        exclusiveThread = Thread.currentThread();
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
        checkState();
        if (exclusiveThread != Thread.currentThread())
            throw new IllegalStateException
                    ("Exclusive access not assigned to current Thread");


        try {
            SCardEndTransaction(cardId, SCARD_LEAVE_CARD);
        } catch (PCSCException e) {
            handleError(e);
            throw new CardException("endExclusive() failed", e);
        }

        exclusiveThread = NULL;
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
    void disconnect(bool reset)
    {
        if (reset)
            checkSecurity("reset");

        if (state != State.OK)
            return;

        checkExclusive();
        try {
            SCardDisconnect(cardId, (reset ? SCARD_LEAVE_CARD : SCARD_RESET_CARD));
        } catch (PCSCException e) {
            throw new CardException("disconnect() failed", e);
        } finally {
            state = State.DISCONNECTED;
            exclusiveThread = null;
        }
    }

    bool isValid()
    {
        if (state != State.OK)
            return false;

        // ping card via SCardStatus
        try {
            SCardStatus(cardId, new byte[2]);
            return true;
        } catch (PCSCException e) {
            state = State.REMOVED;
            return false;
        }
    }

    void checkExclusive()
    {
        Thread *t = exclusiveThread;
        if (!t) {
            logger->debug("exclusiveThread is null\n");
            return;
        }

        if (t->self() != Thread::currentThread())
            throw new CardException("Exclusive access established by another Thread");
    }

private:
    void checkState()
    {
        State s = state;
        if (s == State::DISCONNECTED)
            throw IllegalStateException("Card has been disconnected");
        else if (s == State::REMOVED)
            throw IllegalStateException("Card has been removed");
    }
};

}
}
}
}
