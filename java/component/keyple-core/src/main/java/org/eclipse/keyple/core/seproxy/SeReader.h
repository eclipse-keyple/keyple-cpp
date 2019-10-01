/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <memory>

#include "Configurable.h"
#include "Comparable.h"
#include "Nameable.h"
#include "SeProtocol.h"
#include "NoStackTraceThrowable.h"
#include "TransmissionMode.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace protocol { class SeProtocolSetting; } } } } } }


namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
    
using Configurable          = org::eclipse::keyple::core::util::Configurable;
using Nameable              = org::eclipse::keyple::core::util::Nameable;
using NoStackTraceThrowable = org::eclipse::keyple::core::seproxy::exception::NoStackTraceThrowable;
using SeProtocol            = org::eclipse::keyple::core::seproxy::protocol::SeProtocol;
using SeProtocolSetting     = org::eclipse::keyple::core::seproxy::protocol::SeProtocolSetting;
using TransmissionMode      = org::eclipse::keyple::core::seproxy::protocol::TransmissionMode;


/**
 * SeReader interface
 * <ul>
 * <li>To retrieve the unique reader name</li>
 * <li>To check the SE presence.</li>
 * <li>To set the communication protocol and the specific reader parameters.</li>
 * </ul>
 * Interface used by applications processing SE.
 */
class SeReader : public virtual Nameable, public Configurable, public Comparable<std::shared_ptr<SeReader>> {
public:
    /**
     *
     */
    virtual ~SeReader() {}

    /**
     * Checks if is SE present.
     *
     * @return true if a Secure Element is present in the reader
     * @throws NoStackTraceThrowable a exception without stack trace in order to be catched and
     *         processed silently
     */
    virtual bool isSePresent() = 0;

    /**
     * A protocol setting is an association that establish the link between a protocol identifier
      * and a String that defines how a particular SE may match this protocol.
     * <p>
     * For example:
     * <p>
     * for a PC/SC plugin the String is defined as a regular expression that will be applied to the
     * ATR in order to identify which type of SE is currently communicating.
     * <p>
     * for another plugin (e.g. NFC or proprietary plugin) the String would be any specific word to
     * match a value handled by the low level API of the reader (e.g. "NfcA", "NfcB",
     * "MifareClassic", etc)
     *
     * <p>
     * A reader plugin will handle a list of protocol settings in order to target multiple types of
     * SE.
     *
     * @param seProtocol the protocol key identifier to be added to the plugin internal list
     * @param protocolRule a string use to define how to identify the protocol
     */
    virtual void addSeProtocolSetting(SeProtocol& seProtocol, const std::string &protocolRule) = 0;

    /**
     * Complete the current setting map with the provided map
     *
     * @param protocolSetting
     */
    virtual void setSeProtocolSetting(std::unordered_map<SeProtocol, std::string>& protocolSetting) = 0;

    /**
     * @return the transmission mode in use with this SE reader
     */
    virtual TransmissionMode getTransmissionMode() = 0;
};

}
}
}
}
}
