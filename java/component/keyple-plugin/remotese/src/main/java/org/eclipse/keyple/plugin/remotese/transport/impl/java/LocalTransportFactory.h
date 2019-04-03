#pragma once

#include "../../factory/TransportFactory.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace impl { namespace java { class LocalClient; } } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace impl { namespace java { class LocalServer; } } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace factory { class ClientNode; } } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { namespace factory { class ServerNode; } } } } } } }

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
                        namespace impl {
                            namespace java {



                                using ClientNode = org::eclipse::keyple::plugin::remotese::transport::factory::ClientNode;
                                using ServerNode = org::eclipse::keyple::plugin::remotese::transport::factory::ServerNode;
                                using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::factory::TransportFactory;

                                class LocalTransportFactory : public TransportFactory {

                                    /*
                                     * static private LocalTransportFactory instance = new LocalTransportFactory(); private
                                     * LocalTransportFactory(){} static public LocalTransportFactory instance(){ return instance; }
                                     */

                                private:
                                    const std::shared_ptr<LocalClient> theClient;
                                    const std::shared_ptr<LocalServer> theServer;

                                public:
                                    LocalTransportFactory();

                                    std::shared_ptr<ClientNode> getClient() override;

                                    std::shared_ptr<ServerNode> getServer() override;

protected:
                                    std::shared_ptr<LocalTransportFactory> shared_from_this() {
                                        return std::static_pointer_cast<LocalTransportFactory>(org.eclipse.keyple.plugin.remotese.transport.factory.TransportFactory::shared_from_this());
                                    }
                                };

                            }
                        }
                    }
                }
            }
        }
    }
}
