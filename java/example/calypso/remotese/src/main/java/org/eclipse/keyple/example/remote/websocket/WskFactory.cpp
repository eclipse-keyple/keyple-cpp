#include "WskFactory.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/ClientNode.h"
#include "WskClient.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/ServerNode.h"
#include "WskServer.h"

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
const std::shared_ptr<org::slf4j::Logger> WskFactory::logger = org::slf4j::LoggerFactory::getLogger(WskFactory::typeid);

                        WskFactory::WskFactory(Boolean isMasterServer) {
                            this->isMasterServer = isMasterServer;
                        }

                        std::shared_ptr<ClientNode> WskFactory::getClient() {

                            logger->info("*** Create Websocket Client ***");


                            std::shared_ptr<ClientNode> wskClient;
                            try {
                                wskClient = std::make_shared<WskClient>(std::make_shared<URI>(protocol + "localhost:" + std::to_string(port) + keypleUrl), clientNodeId);
                                // wskClient.connectAReader();
                                return wskClient;
                            }
                            catch (const URISyntaxException &e) {
                                e->printStackTrace();
                                return nullptr;
                            }

                        }

                        std::shared_ptr<ServerNode> WskFactory::getServer() throw(IOException) {

                            logger->info("*** Create Websocket Server ***");

                            std::shared_ptr<InetSocketAddress> inet = std::make_shared<InetSocketAddress>(Inet4Address::getByName(bindUrl), port);
                            return std::make_shared<WskServer>(inet, this->isMasterServer, clientNodeId + "server");

                        }
                    }
                }
            }
        }
    }
}
