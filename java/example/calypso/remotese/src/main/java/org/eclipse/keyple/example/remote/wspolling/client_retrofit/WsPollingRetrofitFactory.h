#pragma once

#include "../../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/TransportFactory.h"
#include <string>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class ClientNode; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class ServerNode; } } } } } }

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
            namespace example {
                namespace remote {
                    namespace wspolling {
                        namespace client_retrofit {

                            using ClientNode = org::eclipse::keyple::plugin::remotese::transport::ClientNode;
                            using ServerNode = org::eclipse::keyple::plugin::remotese::transport::ServerNode;
                            using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;

                            /**
                             * Web service factory, get @{@link WsPRetrofitClientImpl} and {@link WsPServer} Optimized for
                             * Android and Java
                             */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("PMD.AvoidUsingHardCodedIP") public class WsPollingRetrofitFactory extends org.eclipse.keyple.plugin.remotese.transport.TransportFactory
                            class WsPollingRetrofitFactory : public TransportFactory {

                                // default values
                            private:
                                const std::string pollingUrl = "/polling";
                                const std::string keypleUrl = "/keypleDTO";
                                Integer port = 8000 + (std::make_shared<Random>())->nextInt((100) + 1);
                                std::string clientNodeId = "local1";
                                std::string hostname = "0.0.0.0";
                                std::string protocol = "http://";

                                static const std::shared_ptr<Logger> logger;

                            public:
                                WsPollingRetrofitFactory();

                            private:
                                WsPollingRetrofitFactory(Integer port, const std::string &clientNodeId, const std::string &hostname, const std::string &protocol);

                                WsPollingRetrofitFactory(std::shared_ptr<Properties> serverProp, const std::string &clientNodeId);

                            public:
                                std::shared_ptr<ClientNode> getClient() override;



                                std::shared_ptr<ServerNode> getServer() throw(IOException) override;

protected:
                                std::shared_ptr<WsPollingRetrofitFactory> shared_from_this() {
                                    return std::static_pointer_cast<WsPollingRetrofitFactory>(org.eclipse.keyple.plugin.remotese.transport.TransportFactory::shared_from_this());
                                }
                            };

                        }
                    }
                }
            }
        }
    }
}
