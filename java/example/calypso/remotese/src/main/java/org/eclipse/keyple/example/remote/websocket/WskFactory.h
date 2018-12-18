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
                    namespace websocket {

                        using ClientNode = org::eclipse::keyple::plugin::remotese::transport::ClientNode;
                        using ServerNode = org::eclipse::keyple::plugin::remotese::transport::ServerNode;
                        using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * Web socket factory, by default works at localhost
                         */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("PMD.AvoidUsingHardCodedIP") public class WskFactory extends org.eclipse.keyple.plugin.remotese.transport.TransportFactory
                        class WskFactory : public TransportFactory {

                        private:
                            const Integer port = 8000 + (std::make_shared<Random>())->nextInt((100) + 1);
                            const std::string keypleUrl = "/keypleDTO";
                            const std::string bindUrl = "0.0.0.0";
                            const std::string protocol = "http://";
                            const std::string clientNodeId = "local1";
                            Boolean isMasterServer;

                            static const std::shared_ptr<Logger> logger;


                        public:
                            WskFactory(Boolean isMasterServer);

                            std::shared_ptr<ClientNode> getClient() override;

                            std::shared_ptr<ServerNode> getServer() throw(IOException) override;



protected:
                            std::shared_ptr<WskFactory> shared_from_this() {
                                return std::static_pointer_cast<WskFactory>(org.eclipse.keyple.plugin.remotese.transport.TransportFactory::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
