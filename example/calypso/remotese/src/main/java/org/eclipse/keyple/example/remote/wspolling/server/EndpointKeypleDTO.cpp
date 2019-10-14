#include "EndpointKeypleDTO.h"
#include "../WsPTransportDTO.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace wspolling {
                        namespace server {
                            using WsPTransportDTO = org::eclipse::keyple::example::remote::wspolling::WsPTransportDTO;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
                            using com::sun::net::httpserver::HttpExchange;
                            using com::sun::net::httpserver::HttpHandler;

                            EndpointKeypleDTO::EndpointKeypleDTO(std::shared_ptr<DtoSender> dtoSender, const std::string &nodeId) : dtoSender(dtoSender) / * endpointPolling */, nodeId(nodeId) {

                            }

                            void EndpointKeypleDTO::handle(std::shared_ptr<HttpExchange> t) throw(IOException) {

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                logger->trace("Incoming HttpExchange {} ", t->toString());
                                logger->trace("Incoming Request {} ", t->getRequestMethod());
                                std::string requestMethod = t->getRequestMethod();

                                if (requestMethod == "POST") {
                                    std::string body = parseBodyToString(t->getRequestBody()); // .. parse the
                                    // request body
                                    std::shared_ptr<KeypleDto> incoming = KeypleDtoHelper::fromJson(body);
                                    std::shared_ptr<TransportDto> transportDto = std::make_shared<WsPTransportDTO>(incoming, dtoSender);

                                    logger->trace("Incoming DTO {} ", KeypleDtoHelper::toJson(incoming));
                                    std::shared_ptr<TransportDto> outcoming = dtoHandler->onDTO(transportDto);

                                    setHttpResponse(t, outcoming->getKeypleDTO());

                                }
                            }

                            void EndpointKeypleDTO::setDtoHandler(std::shared_ptr<DtoHandler> receiver) {
                                this->dtoHandler = receiver;
                            }

                            void EndpointKeypleDTO::sendDTO(std::shared_ptr<TransportDto> message) {
                                logger->warn("Send DTO can not be used in Web Service DemoMaster");
                                // not in use, one way communication, server do not send message
                            }

                            void EndpointKeypleDTO::sendDTO(std::shared_ptr<KeypleDto> message) {
                                logger->warn("Send DTO can not be used in Web Service DemoMaster");
                            }

                            std::string EndpointKeypleDTO::getNodeId() {
                                return this->nodeId;
                            }

                            void EndpointKeypleDTO::setHttpResponse(std::shared_ptr<HttpExchange> t, std::shared_ptr<KeypleDto> resp) throw(IOException) {
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

                            std::string EndpointKeypleDTO::parseBodyToString(std::shared_ptr<InputStream> is) {
                                std::shared_ptr<Scanner> s = (std::make_shared<Scanner>(is))->useDelimiter("\\A");
                                return s->hasNext() ? s->next() : "";
                            }

                            void EndpointKeypleDTO::update(std::shared_ptr<KeypleDto> event_Renamed) {
                                // not in used in ws
                            }
                        }
                    }
                }
            }
        }
    }
}
