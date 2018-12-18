#include "WsPollingRetrofitFactory.h"
#include "../../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/ClientNode.h"
#include "WsPRetrofitClientImpl.h"
#include "../../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/ServerNode.h"
#include "../server/WsPServer.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace wspolling {
                        namespace client_retrofit {
                            using WsPServer = org::eclipse::keyple::example::remote::wspolling::server::WsPServer;
                            using ClientNode = org::eclipse::keyple::plugin::remotese::transport::ClientNode;
                            using ServerNode = org::eclipse::keyple::plugin::remotese::transport::ServerNode;
                            using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> WsPollingRetrofitFactory::logger = org::slf4j::LoggerFactory::getLogger(WsPollingRetrofitFactory::typeid);

                            WsPollingRetrofitFactory::WsPollingRetrofitFactory() {
                            }

                            WsPollingRetrofitFactory::WsPollingRetrofitFactory(Integer port, const std::string &clientNodeId, const std::string &hostname, const std::string &protocol) {
                                this->port = port;
                                this->clientNodeId = clientNodeId;
                                this->hostname = hostname;
                                this->protocol = protocol;
                            }

                            WsPollingRetrofitFactory::WsPollingRetrofitFactory(std::shared_ptr<Properties> serverProp, const std::string &clientNodeId) {
                                if (serverProp->containsKey("server.port")) {
                                    this->port = Integer::decode(serverProp->getProperty("server.port"));
                                }
                                if (serverProp->containsKey("server.hostname")) {
                                    this->hostname = serverProp->getProperty("server.hostname");
                                }

                                if (serverProp->containsKey("server.protocol")) {
                                    this->protocol = serverProp->getProperty("server.protocol") + "://";
                                }

                                this->clientNodeId = clientNodeId;
                            }

                            std::shared_ptr<ClientNode> WsPollingRetrofitFactory::getClient() {

                                logger->info("*** Create RETROFIT Ws Polling Client ***");
                                return std::make_shared<WsPRetrofitClientImpl>(protocol + hostname + ":" + std::to_string(port), clientNodeId);
                            }

                            std::shared_ptr<ServerNode> WsPollingRetrofitFactory::getServer() throw(IOException) {

                                logger->info("*** Create Ws Polling Server ***");
                                return std::make_shared<WsPServer>(hostname, port, keypleUrl, pollingUrl, clientNodeId + "server");

                            }
                        }
                    }
                }
            }
        }
    }
}
