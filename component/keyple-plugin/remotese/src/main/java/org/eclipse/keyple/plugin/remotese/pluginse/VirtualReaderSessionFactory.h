#pragma once

#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace pluginse { class VirtualReaderSession; } } } } } }

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
                    namespace pluginse {

                        /**
                         * Create Virtual Reader Sessions
                         */
                        class VirtualReaderSessionFactory : public std::enable_shared_from_this<VirtualReaderSessionFactory> {



                            /**
                             * Create a new session
                             * 
                             * @param nativeReaderName
                             * @param nodeId slave device nodeId
                             * @return Session for this reader
                             */
                        public:
                            virtual std::shared_ptr<VirtualReaderSession> createSession(const std::string &nativeReaderName, const std::string &nodeId);


                            /*
                             * PRIVATE METHODS
                             */

                            /**
                             * Generate a unique sessionId for a new connecting localreader
                             * 
                             * @param nativeReaderName : Local Reader Name
                             * @param nodeId : Node Id from which the local reader name connect to
                             * @return unique sessionId
                             */
                        private:
                            std::string generateSessionId(const std::string &nativeReaderName, const std::string &nodeId);


                        };

                    }
                }
            }
        }
    }
}
