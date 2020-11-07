/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

#include <memory>

/* Core */
#include "ProxyElement.h"
#include "SeProtocol.h"
#include "TransmissionMode.h"

/* Common */
#include "KeypleStd.h"

namespace keyple {
namespace core {
namespace seproxy {

using namespace keyple::core::seproxy::protocol;

/**
 * SeReader interface
 * <ul>
 * <li>To retrieve the unique reader name</li>
 * <li>To check the SE presence.</li>
 * <li>To set the communication protocol and the specific reader parameters.
 * </li>
 * </ul>
 * Interface used by applications processing SE.
 */
class SeReader : public virtual ProxyElement {
public:
    /**
     *
     */
    virtual ~SeReader() {}

    /**
     * Checks if is SE present.
     *
     * @return true if a Secure Element is present in the reader
     * @throws KeypleReaderIOException if the communication with the reader or
     *         the SE has failed
     */
    virtual bool isSePresent() = 0;

    /**
     * A protocol setting is an association that establish the link between a
     * protocol identifier and a String that defines how a particular SE may
     * match this protocol.
     * <p>
     * For example:
     * <p>
     * for a PC/SC plugin the String is defined as a regular expression that
     * will be applied to the ATR in order to identify which type of SE is
     * currently communicating.
     * <p>
     * for another plugin (e.g. NFC or proprietary plugin) the String would be
     * any specific word to match a value handled by the low level API of the
     * reader (e.g. "NfcA", "NfcB",
     * "MifareClassic", etc)
     *
     * <p>
     * A reader plugin will handle a list of protocol settings in order to
     * target multiple types of SE.
     *
     * @param seProtocol the protocol key identifier to be added to the plugin internal list
     * @param protocolRule a string use to define how to identify the protocol
     */
    virtual void addSeProtocolSetting(std::shared_ptr<SeProtocol> seProtocol,
                                      const std::string& protocolRule) = 0;

    /**
     * Complete the current setting map with the provided map
     *
     * @param protocolSetting the protocol setting map
     */
    virtual void setSeProtocolSetting(
        const std::map<std::shared_ptr<SeProtocol>, std::string>& protocolSetting) = 0;

    /**
     * @return the transmission mode in use with this SE reader
     */
    virtual const TransmissionMode& getTransmissionMode() const = 0;

    /**
     *
     */
    virtual bool operator==(const SeReader& o) const
    {
        return this->getParameters() ==  o.getParameters();
    }

    /**
     *
     */
    friend std::ostream& operator<<(std::ostream& os, const std::shared_ptr<SeReader> sr)
    {
        os << "SEREADER: {"
           << "PARAMETERS: " << sr->getParameters()
           << "}";

        return  os;
    }
};

}
}
}
