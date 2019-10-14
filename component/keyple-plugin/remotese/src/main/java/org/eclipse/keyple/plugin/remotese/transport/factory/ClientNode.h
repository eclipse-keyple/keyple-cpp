#pragma once

#include "TransportNode.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace factory { class ConnectCallback; } } } } } } }

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
                        namespace factory {


                            /**
                             * Client type of TransportNode, connects to a ServerNode
                             */
                            class ClientNode : public TransportNode {

                            public:
                                virtual void connect(std::shared_ptr<ConnectCallback> connectCallback) = 0;

                                virtual void disconnect() = 0;

                            private:
                                class ConnectCallback {
                                public:
                                    virtual void onConnectSuccess() = 0;

                                    virtual void onConnectFailure() = 0;
                                };

                            };

                        }
                    }
                }
            }
        }
    }
}
