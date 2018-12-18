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

                            EndpointPolling::EndpointPolling(std::shared_ptr<BlockingQueue<std::shared_ptr<HttpExchange>>> requestQueue, const std::string &nodeId) : nodeId(nodeId), requestQueue(requestQueue) {
                            }

                            std::string EndpointPolling::getNodeId() {
                                return nodeId;
                            }

                            void EndpointPolling::handle(std::shared_ptr<HttpExchange> t) {

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                logger->debug("Incoming HttpExchange {} ", t->toString());
                                logger->debug("Incoming Request {} ", t->getRequestMethod());
                                std::string requestMethod = t->getRequestMethod();

                                if (requestMethod == "GET") {

                                    // hold response until we got a response or timeout
                                    std::unordered_map<std::string, std::string> params = queryToMap(t->getRequestURI().getQuery());
                                    std::string nodeId = params["clientNodeId"];
                                    // logger.trace("param clientNodeId=" + params.get("clientNodeId"));

                                    // set httpExchange in queue
                                    requestQueue->add(t);

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                    logger->debug("Receive a polling request {} from clientNodeId {} queue size {}", t->toString(), nodeId, requestQueue->size());

                                }
                            }

                            void EndpointPolling::setDtoHandler(std::shared_ptr<DtoHandler> receiver) {
                                this->dtoHandler = receiver;
                            }

                            void EndpointPolling::sendDTO(std::shared_ptr<TransportDto> message) {
                                logger->warn("Send DTO with transport message {}", message);
                                this->sendDTO(message->getKeypleDTO());
                            }

                            void EndpointPolling::sendDTO(std::shared_ptr<KeypleDto> message) {
                                logger->debug("Using polling to send keypleDTO whose action : {}", message->getAction());

                                std::shared_ptr<HttpExchange> t;
                                try {
                                    t = requestQueue->poll(2, TimeUnit::SECONDS);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                    logger->debug("Found a waiting HttpExchange {}", t != nullptr ? t->toString() : "null");
                                    setHttpResponse(t, message);
                                }
                                catch (const IOException &e) {
                                    e->printStackTrace();
                                    throw std::make_shared<IllegalStateException>("Polling has failed due to " + e->getMessage());
                                }
                                catch (const InterruptedException &e) {
                                    throw std::make_shared<IllegalStateException>("Request Queue is still empty after timeout, impossible to send DTO");
                                }

                            }

                            void EndpointPolling::update(std::shared_ptr<KeypleDto> event_Renamed) {
                                logger->info("Send DTO from update {}", event_Renamed->getAction());
                                this->sendDTO(event_Renamed);
                            }

                            std::unordered_map<std::string, std::string> EndpointPolling::queryToMap(const std::string &query) {
                                std::unordered_map<std::string, std::string> result;
                                for (std::string param : query.split("&")) {
                                    std::vector<std::string> entry = param.split("=");
                                    if (entry.size() > 1) {
                                        result.emplace(entry[0], entry[1]);
                                    }
                                    else {
                                        result.emplace(entry[0], "");
                                    }
                                }
                                return result;
                            }

                            void EndpointPolling::setHttpResponse(std::shared_ptr<HttpExchange> t, std::shared_ptr<KeypleDto> resp) throw(IOException) {
                                if (!resp->getAction().isEmpty()) {
                                    std::string responseBody = KeypleDtoHelper::toJson(resp);
                                    Integer responseCode = 200;
                                    t->getResponseHeaders()->add("Content-Type", "application/json");
                                    t->sendResponseHeaders(responseCode, responseBody.length());
                                    std::shared_ptr<OutputStream> os = t->getResponseBody();
                                    os->write(responseBody.getBytes());
                                    os->close();
                                    logger->debug("Outcoming Response Code {} ", responseCode);
                                    logger->debug("Outcoming Response Body {} ", responseBody);


                                }
                                else {
                                    std::string responseBody = "{}";
                                    Integer responseCode = 200;
                                    t->getResponseHeaders()->add("Content-Type", "application/json");
                                    t->sendResponseHeaders(responseCode, responseBody.length());
                                    std::shared_ptr<OutputStream> os = t->getResponseBody();
                                    os->write(responseBody.getBytes());
                                    os->close();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
