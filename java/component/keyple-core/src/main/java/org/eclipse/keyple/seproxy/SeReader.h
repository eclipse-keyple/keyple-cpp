#pragma once

#include "../util/Configurable.h"
#include "../util/Nameable.h"
#include "protocol/TransmissionMode.h"
#include <memory>
#include "NoStackTraceThrowable.h"
#include "Comparable.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace protocol { class SeProtocolSetting; } } } } }

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
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {


                using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                using Configurable = org::eclipse::keyple::util::Configurable;
                using Nameable = org::eclipse::keyple::util::Nameable;


                /**
                 * SeReader interface
                 * <ul>
                 * <li>To retrieve the unique reader name</li>
                 * <li>To check the SE presence.</li>
                 * <li>To set the communication protocol and the specific reader parameters.</li>
                 * </ul>
                 * Interface used by applications processing SE.
                 */
                class SeReader : public Nameable, public Configurable, public Comparable<std::shared_ptr<SeReader>> {

                    /**
                     * Checks if is SE present.
                     *
                     * @return true if a Secure Element is present in the reader
                     * @throws NoStackTraceThrowable a exception without stack trace in order to be catched and
                     *         processed silently
                     */
                public:
                    virtual bool isSePresent() = 0;

                    /**
                     * A protocol setting is a map that establish the link between a protocol identifier and a
                     * String that defines how a particular SE may match this protocol.
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
                     * @param seProtocolSetting the protocol setting to be added to the plugin internal list
                     */
                    virtual void addSeProtocolSetting(std::shared_ptr<SeProtocolSetting> seProtocolSetting) = 0;

                    /**
                     * @return the transmission mode in use with this SE reader
                     */
                    virtual TransmissionMode getTransmissionMode() = 0;
                };

            }
        }
    }
}
