#pragma once

#include "VirtualReaderSession.h"
#include <string>
#include <stdexcept>
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
                    namespace pluginse {



                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * Manage RSE Reader Session
                         */
                        class VirtualReaderSessionImpl : public std::enable_shared_from_this<VirtualReaderSessionImpl>, public VirtualReaderSession {

                        private:
                            static const std::shared_ptr<Logger> logger;

                            const std::string sessionId;
                            const std::string slaveNodeId;

                            // constructor
                        public:
                            VirtualReaderSessionImpl(const std::string &sessionId, const std::string &slaveNodeId);


                            std::string getSessionId() override;

                            std::string getSlaveNodeId() override;


                            std::string toString() override;
                        };

                    }
                }
            }
        }
    }
}
