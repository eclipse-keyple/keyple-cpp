#pragma once

#include "TransportNode.h"

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



                        /**
                         * Client type of TransportNode, connects to a ServerNode
                         */
                        class ClientNode : public TransportNode {

                        public:
                            virtual void connect() = 0;

                            virtual void disconnect() = 0;

                        };

                    }
                }
            }
        }
    }
}
