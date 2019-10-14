#include "WsPollingFactory.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/ClientNode.h"
#include "client/WsPClient.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/ServerNode.h"
#include "server/WsPServer.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace wspolling {
                        using WsPClient = org::eclipse::keyple::example::remote::wspolling::client::WsPClient;
                        using WsPServer = org::eclipse::keyple::example::remote::wspolling::server::WsPServer;
                        using ClientNode = org::eclipse::keyple::plugin::remotese::transport::ClientNode;
                        using ServerNode = org::eclipse::keyple::plugin::remotese::transport::ServerNode;
                        using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> WsPollingFactory::logger = org::slf4j::LoggerFactory::getLogger(WsPollingFactory::typeid);

                        WsPollingFactory::WsPollingFactory() {
                        }

                        WsPollingFactory::WsPollingFactory(Integer port, const std::string &pollingUrl, const std::string &keypleUrl, const std::string &clientNodeId, const std::string &bindUrl, const std::string &protocol) {
                            this->port = port;
                            this->pollingUrl = pollingUrl;
                            this->keypleUrl = keypleUrl;
                            this->clientNodeId = clientNodeId;
                            this->bindUrl = bindUrl;
                            this->protocol = protocol;
                        }

                        std::shared_ptr<ClientNode> WsPollingFactory::getClient() {
                            logger->info("*** Create Ws Polling Client ***");
                            return std::make_shared<WsPClient>(protocol + bindUrl + ":" + std::to_string(port), keypleUrl, pollingUrl, clientNodeId);
                        }

                        std::shared_ptr<ServerNode> WsPollingFactory::getServer() {

                            logger->info("*** Create Ws Polling Server ***");
                            try {
                                return std::make_shared<WsPServer>(bindUrl, port, keypleUrl, pollingUrl, clientNodeId + "server");
                            }
                            catch (const IOException &e) {
                                return nullptr;
                            }

                        }
                    }
                }
            }
        }
    }
}
