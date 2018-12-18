#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "stringhelper.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleChannelStateException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleIOReaderException; } } } } }

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
            namespace plugin {
                namespace stub {

                    using KeypleChannelStateException = org::eclipse::keyple::seproxy::exception::KeypleChannelStateException;
                    using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;

                    class StubSecureElement : public std::enable_shared_from_this<StubSecureElement> {

                        /**
                         * Getter for ATR
                         * 
                         * @return Secured Element ATR
                         */
                    public:
                        virtual std::vector<char> getATR() = 0;


//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
                        bool isPhysicalChannelOpen_Renamed = false;

                        virtual bool isPhysicalChannelOpen();

                        virtual void openPhysicalChannel() throw(KeypleChannelStateException);

                        virtual void closePhysicalChannel() throw(KeypleChannelStateException);


                        /**
                         * @return SE protocol supported by the SE
                         */
                        virtual std::string getSeProcotol() = 0;


                        std::unordered_map<std::string, std::string> hexCommands = std::unordered_map<std::string, std::string>();

                        /**
                         * Add more simulated commands to the Stub SE
                         *
                         * @param command : hexadecimal command to react to
                         * @param response : hexadecimal response to be sent in reaction to command
                         */
                        virtual void addHexCommand(const std::string &command, const std::string &response);

                        /**
                         * Remove simulated commands from the Stub SE
                         *
                         * @param command : hexadecimal command to be removed
                         */
                        virtual void removeHexCommand(const std::string &command);

                        /**
                         * Return APDU Response to APDU Request
                         *
                         * @param apduIn : commands to be processed
                         * @return APDU response
                         * @throws KeypleIOReaderException if the transmission fails
                         */
                        virtual std::vector<char> processApdu(std::vector<char> &apduIn) throw(KeypleIOReaderException);
                    };

                }
            }
        }
    }
}
