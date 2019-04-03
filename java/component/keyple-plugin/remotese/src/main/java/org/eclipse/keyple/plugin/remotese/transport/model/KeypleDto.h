#pragma once

#include <string>
#include <memory>

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
                namespace remotese {
                    namespace transport {
                        namespace model {

                            /**
                             * Data Transfer Object used to common an API call from a Master Reader to a Slave Reader (and vice
                             * versa)
                             */
                            class KeypleDto : public std::enable_shared_from_this<KeypleDto> {

                                // Slave Node Id
                            private:
                                std::string clientNodeId;

                                // Master reader session
                                std::string sessionId;

                                // Slave reader name
                                std::string nativeReaderName;

                                // Master reader name
                                std::string virtualReaderName;

                                // API method to be called
                                const std::string action;

                                // Arguments of the API (json)
                                const std::string body;

                                // Is a request or a response
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
                                const Boolean isRequest_Renamed;


                                /**
                                 * Basic Constructor
                                 * 
                                 * @param action : API method to be called
                                 * @param body : Arguments of the API (json)
                                 * @param isRequest : Is a request or a response
                                 */
                            public:
                                KeypleDto(const std::string &action, const std::string &body, Boolean isRequest);

                                /**
                                 * Constructor with a Virtual Reader Session Id
                                 *
                                 * @param action : API method to be called
                                 * @param body : Arguments of the API (json)
                                 * @param isRequest : Is a request or a response
                                 * @param sessionId : Session Id of current Virtual Reader Session Id
                                 */
                                KeypleDto(const std::string &action, const std::string &body, Boolean isRequest, const std::string &sessionId);

                                /**
                                 * Constructor with a Virtual Reader Session Id
                                 *
                                 * @param action : API method to be called
                                 * @param body : Arguments of the API (json)
                                 * @param isRequest : Is a request or a response
                                 * @param sessionId : Session Id of current Virtual Reader Session Id
                                 */
                                KeypleDto(const std::string &action, const std::string &body, Boolean isRequest, const std::string &sessionId, const std::string &nativeReaderName, const std::string &virtualReaderName, const std::string &clientNodeId);

                                /*
                                 * Getters and Setters
                                 */

                                virtual Boolean isRequest();

                                virtual std::string getAction();

                                virtual std::string getBody();

                                virtual std::string getSessionId();

                                virtual std::string getNodeId();

                                virtual std::string getNativeReaderName();

                                virtual std::string getVirtualReaderName();

                                std::string toString() override;
                            };

                        }
                    }
                }
            }
        }
    }
}
