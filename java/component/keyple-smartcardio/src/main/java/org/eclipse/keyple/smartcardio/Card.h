/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#pragma once

#include <vector>

/* Smartcard I/O */
#include "CardException.h"
#include "PCSCException.h"

/* Common */
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "Thread.h"

/* PC/SC */
#if defined(WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
#include <winscard.h>
#else
#include <PCSC/winscard.h>
#include <PCSC/wintypes.h>
#endif

namespace org { namespace eclipse { namespace keyple { namespace smartcardio { class ATR; }}}}
namespace org { namespace eclipse { namespace keyple { namespace smartcardio { class CardChannel; }}}}
namespace org { namespace eclipse { namespace keyple { namespace smartcardio { class CardTerminal; }}}}

namespace org {
namespace eclipse {
namespace keyple {
namespace smartcardio {

using Logger        = org::eclipse::keyple::common::Logger;
using LoggerFactory = org::eclipse::keyple::common::LoggerFactory;

class EXPORT Card {
public:
    /** 
     * Protocol in use, one of SCARD_PROTOCOL_T0 and SCARD_PROTOCOL_T1 
     */
    DWORD protocol;

    /**
     * The native SCARDHANDL
     */
    SCARDHANDLE cardhdl;

    /**
     * 
     */
    SCARD_IO_REQUEST pioSendPCI;

private:
    /**
     * ATR of this card
     */
    ATR* atr;

    /**
     * 
     */
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(Card));

    /**
     * The terminal that created this card
     */
    CardTerminal* terminal;

    /**
     * The basic logical channel (channel 0)
     */
    CardChannel* basicChannel;

    /**
     * State of this card connection
     */
    DWORD state;

    /**
     * Thread holding exclusive access to the card, or null
     */
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
    Card(CardTerminal* terminal, std::string protocol);

    /**
     * Returns the ATR of this card.
     *
     * @return the ATR of this card.
     */
    ATR* getATR();

    /**
     * 
     */
    std::string getProtocol();

    /**
     * Returns the CardChannel for the basic logical channel. The basic logical
     * channel has a channel number of 0.
     *
     * @throws SecurityException if a SecurityManager exists and the caller does
     *                           not have the required permission
     * @throws IllegalStateException if this card object has been disposed of via
     *                               the disconnect() method
     */
    CardChannel* getBasicChannel();

    /**
     * 
     */
    CardChannel* openLogicalChannel();

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
    void beginExclusive();

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
    void endExclusive();

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
    void disconnect(bool reset);

    /**
     * 
     */
    bool isValid();

    /**
     * 
     */
    void checkExclusive();

    /**
     * 
     */
    void checkState();

    /**
     *
     */
    void checkSecurity(std::string action);

    /**
     * 
     */
    void handleError(PCSCException e);

private:
    /**
     *
     */
    static int getSW(std::vector<char> b);
};

}
}
}
}
