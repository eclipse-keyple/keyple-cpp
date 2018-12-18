#pragma once

#include <string>
#include <typeinfo>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ApduRequest; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace command { class CommandsTable; } } } }

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
            namespace command {

                using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;

                /**
                 * Generic APDU command builder.
                 * <p>
                 * It provides the generic getters to retrieve:
                 * <ul>
                 * <li>the name of the command,</li>
                 * <li>the built APDURequest,</li>
                 * <li>the corresponding AbstractApduResponseParser class.</li>
                 * </ul>
                 */

                class AbstractApduCommandBuilder : public std::enable_shared_from_this<AbstractApduCommandBuilder> {

                    /**
                     * The command name (will appear in logs)
                     */
                private:
                    std::string name;
                    /**
                     * The command parser class
                     */
                    std::type_info commandParserClass;

                    /** the byte array APDU request. */
                protected:
                    std::shared_ptr<ApduRequest> request;

                    /**
                     * the generic abstract constructor to build an APDU request with a command reference and a byte
                     * array.
                     *
                     * @param commandReference command reference
                     * @param request request
                     */
                    // public AbstractApduCommandBuilder(CalypsoCommands commandReference, ApduRequest request) {
                public:
                    AbstractApduCommandBuilder(std::shared_ptr<CommandsTable> commandReference, std::shared_ptr<ApduRequest> request);

                    AbstractApduCommandBuilder(const std::string &name, std::shared_ptr<ApduRequest> request);

                    /**
                     * Append a string to the current name
                     * 
                     * @param subName the string to append
                     */
                    void addSubName(const std::string &subName);

                    /**
                     * Gets the name.
                     *
                     * @return the name of the APDU command from the CalypsoCommands information.
                     */
                    std::string getName();

                    /**
                     * Gets the apdu response parser class.
                     *
                     * @return the corresponding AbstractApduResponseParser class of the APDU command from the
                     *         CommandsTable information
                     */
                    std::type_info getApduResponseParserClass();

                    /**
                     * Gets the request.
                     *
                     * @return the request
                     */
                    std::shared_ptr<ApduRequest> getApduRequest();

                };

            }
        }
    }
}
