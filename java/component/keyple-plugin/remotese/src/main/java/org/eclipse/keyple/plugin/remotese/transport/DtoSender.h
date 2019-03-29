#pragma once

#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace model { class TransportDto; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace model { class KeypleDto; } } } } } } }

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

                        using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteException;
                        using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                        using TransportDto = org::eclipse::keyple::plugin::remotese::transport::model::TransportDto;

                        /**
                         * Components that sends a DTO over the network to the other end. (slave or master) It can be an
                         * observer for KeypleDto to propagate them through the network
                         */
                        class DtoSender {

                            /**
                             * Send DTO with common information
                             * 
                             * @param message to be sent
                             */
                        public:
                            virtual void sendDTO(std::shared_ptr<TransportDto> message) = 0;

                            /**
                             * Send DTO with no common information (usually a new message)
                             * 
                             * @param message to be sent
                             */
                            virtual void sendDTO(std::shared_ptr<KeypleDto> message) = 0;

                            /**
                             * get nodeId of this DtoSender, must identify the terminal. ie : androidDevice2
                             * 
                             * @return : nodeId
                             */
                            virtual std::string getNodeId() = 0;

                        };

                    }
                }
            }
        }
    }
}
