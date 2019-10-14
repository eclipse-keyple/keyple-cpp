#pragma once

#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/TransportFactory.h"
#include <string>
#include "exceptionhelper.h"
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

                        using ClientNode = org::eclipse::keyple::plugin::remotese::transport::ClientNode;
                        using ServerNode = org::eclipse::keyple::plugin::remotese::transport::ServerNode;
                        using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * Web service factory, get {@link WsPClient} and {@link WsPServer}
                         */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("PMD.AvoidUsingHardCodedIP") public class WsPollingFactory extends org.eclipse.keyple.plugin.remotese.transport.TransportFactory
                        class WsPollingFactory : public TransportFactory {

                            // default values
                        private:
                            Integer port = 8000 + (std::make_shared<Random>())->nextInt((100) + 1);
                            std::string pollingUrl = "/polling";
                            std::string keypleUrl = "/keypleDTO";
                            std::string clientNodeId = "local1";
                            std::string bindUrl = "0.0.0.0";
                            std::string protocol = "http://";

                            static const std::shared_ptr<Logger> logger;

                        public:
                            WsPollingFactory();

                        private:
                            WsPollingFactory(Integer port, const std::string &pollingUrl, const std::string &keypleUrl, const std::string &clientNodeId, const std::string &bindUrl, const std::string &protocol);

                        public:
                            std::shared_ptr<ClientNode> getClient() override;


                            std::shared_ptr<ServerNode> getServer() override;

protected:
                            std::shared_ptr<WsPollingFactory> shared_from_this() {
                                return std::static_pointer_cast<WsPollingFactory>(org.eclipse.keyple.plugin.remotese.transport.TransportFactory::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
