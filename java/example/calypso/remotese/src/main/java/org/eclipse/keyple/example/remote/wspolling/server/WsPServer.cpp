#include "WsPServer.h"
#include "EndpointKeypleDTO.h"
#include "EndpointPolling.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace wspolling {
                        namespace server {
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
                            using com::sun::net::httpserver::HttpExchange;
                            using com::sun::net::httpserver::HttpHandler;
                            using com::sun::net::httpserver::HttpServer;
const std::shared_ptr<org::slf4j::Logger> WsPServer::logger = org::slf4j::LoggerFactory::getLogger(WsPServer::typeid);

                            WsPServer::WsPServer(const std::string &url, Integer port, const std::string &apiUrl, const std::string &pollingUrl, const std::string &nodeId) throw(IOException) : apiUrl(apiUrl), pollingUrl(pollingUrl), nodeId(nodeId), server(HttpServer::create(inet, MAX_CONNECTION)), keypleDTOEndpoint(std::make_shared<EndpointKeypleDTO>(std::static_pointer_cast<DtoSender>(pollingEndpoint), nodeId)), pollingEndpoint(std::make_shared<EndpointPolling>(requestQueue, nodeId)) {
                                logger->info("Init Web Service DemoMaster on url : {}:{}", url, port);


                                // Create Endpoint for polling DTO

                                // Create Endpoint for sending DTO


                                // deploy endpoint
                                this->inet = std::make_shared<InetSocketAddress>(Inet4Address::getByName(url), port);
                                server->createContext(apiUrl, keypleDTOEndpoint);
                                server->createContext(pollingUrl, pollingEndpoint);

                                server->setExecutor(java::util::concurrent::Executors::newCachedThreadPool()); // creates a
                                                                                                          // default
                                                                                                          // executor
                            }

                            void WsPServer::start() {
                                logger->info("Starting Http Web Polling Server on http://{}:{}", inet->getHostName(), inet->getPort());
                                logger->info("Keyple Endpoint {}", this->apiUrl);
                                logger->info("Polling Endpoint {}", this->pollingUrl);
                                server->start();
                            }

                            void WsPServer::setDtoHandler(std::shared_ptr<DtoHandler> receiver) {
                                (std::static_pointer_cast<EndpointKeypleDTO>(this->keypleDTOEndpoint))->setDtoHandler(receiver);
                            }

                            std::string WsPServer::getNodeId() {
                                return nodeId;
                            }

                            void WsPServer::sendDTO(std::shared_ptr<TransportDto> message) {
                                (std::static_pointer_cast<EndpointPolling>(this->pollingEndpoint))->update(message->getKeypleDTO());
                            }

                            void WsPServer::sendDTO(std::shared_ptr<KeypleDto> message) {
                                (std::static_pointer_cast<EndpointPolling>(this->pollingEndpoint))->update(message);
                            }

                            void WsPServer::update(std::shared_ptr<KeypleDto> event_Renamed) {
                                (std::static_pointer_cast<EndpointPolling>(this->pollingEndpoint))->update(event_Renamed);

                            }
                        }
                    }
                }
            }
        }
    }
}
