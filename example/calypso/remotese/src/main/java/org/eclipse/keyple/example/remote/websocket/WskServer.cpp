#include "WskServer.h"
#include "WskTransportDTO.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace websocket {
                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using org::java_websocket::WebSocket;
                        using org::java_websocket::handshake::ClientHandshake;
                        using org::java_websocket::server::WebSocketServer;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> WskServer::logger = org::slf4j::LoggerFactory::getLogger(WskServer::typeid);

                        WskServer::WskServer(std::shared_ptr<InetSocketAddress> address, Boolean isMaster, const std::string &nodeId) : org::java_websocket::server::WebSocketServer(address), nodeId(nodeId) {

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            logger->info("Create websocket server on address {}", address->toString());
                            this->isMaster = isMaster;
                        }

                        void WskServer::onOpen(std::shared_ptr<WebSocket> conn, std::shared_ptr<ClientHandshake> handshake) {
                            logger->debug("Web socket onOpen {} {}", conn, handshake);
                            masterWebSocket = conn;
                        }

                        void WskServer::onClose(std::shared_ptr<WebSocket> conn, int code, const std::string &reason, bool remote) {
                            logger->debug("Web socket onClose {} {} {} {}", conn, code, reason, remote);
                        }

                        void WskServer::onMessage(std::shared_ptr<WebSocket> conn, const std::string &message) {
                            logger->trace("Web socket onMessage {} {}", conn, message);
                            std::shared_ptr<KeypleDto> keypleDto = KeypleDtoHelper::fromJson(message);

                            if (dtoHandler != nullptr) {

                                // LOOP pass DTO and get DTO Response is any
                                std::shared_ptr<TransportDto> transportDto = dtoHandler->onDTO(std::make_shared<WskTransportDTO>(keypleDto, conn, shared_from_this()));

                                if (isMaster) {
                                    // if server is master, can have numerous clients
                                    if (transportDto->getKeypleDTO()->getSessionId() != "") {
                                        sessionId_Connection.emplace(transportDto->getKeypleDTO()->getSessionId(), conn);
                                    }
                                    else {
                                        logger->debug("No session defined in message {}", transportDto);
                                    }
                                }

                                this->sendDTO(transportDto);
                            }
                            else {
                                throw std::make_shared<IllegalStateException>("Received a message but no DtoHandler is defined to process the message");
                            }
                        }

                        void WskServer::onError(std::shared_ptr<WebSocket> conn, std::runtime_error ex) {
                            logger->debug("Web socket onError {} {}", conn, ex);

                        }

                        void WskServer::onStart() {
                            logger->info("Web socket server started");
                        }

                        std::shared_ptr<void> WskServer::getConnection(const std::string &sessionId) {
                            return sessionId_Connection[sessionId];
                        }

                        void WskServer::setDtoHandler(std::shared_ptr<DtoHandler> stubplugin) {
                            this->dtoHandler = stubplugin;
                        }

                        void WskServer::sendDTO(std::shared_ptr<TransportDto> transportDto) {
                            logger->trace("sendDTO {} {}", KeypleDtoHelper::toJson(transportDto->getKeypleDTO()));

                            if (KeypleDtoHelper::isNoResponse(transportDto->getKeypleDTO())) {
                                logger->trace("Keyple DTO is empty, do not send it");
                            }
                            else {

                                if (!isMaster) {
                                    // if server is client -> use the master web socket
                                    masterWebSocket->send(KeypleDtoHelper::toJson(transportDto->getKeypleDTO()));
                                }
                                else {
                                    // server is master, can have numerous slave clients
                                    if ((std::static_pointer_cast<WskTransportDTO>(transportDto))->getSocketWeb() != nullptr) {
                                        logger->trace("Use socketweb included in TransportDto");
                                        (std::static_pointer_cast<WskTransportDTO>(transportDto))->getSocketWeb()->send(KeypleDtoHelper::toJson(transportDto->getKeypleDTO()));
                                    }
                                    else {
                                        // if there is no socketweb defined in the transport dto
                                        // retrieve the socketweb by the sessionId
                                        if (transportDto->getKeypleDTO()->getSessionId() == "") {
                                            logger->warn("No sessionId defined in message, Keyple DTO can not be sent");
                                        }
                                        else {
                                            logger->trace("Retrieve socketweb from sessionId");
                                            // retrieve connection object from the sessionId
                                            auto conn = getConnection(transportDto->getKeypleDTO()->getSessionId());
                                            logger->trace("send DTO {} {}", KeypleDtoHelper::toJson(transportDto->getKeypleDTO()), conn);
                                            (std::static_pointer_cast<WebSocket>(conn))->send(KeypleDtoHelper::toJson(transportDto->getKeypleDTO()));
                                        }
                                    }

                                }
                            }
                        }

                        void WskServer::sendDTO(std::shared_ptr<KeypleDto> message) {
                            logger->trace("Web socket sendDTO without predefined socket {}", KeypleDtoHelper::toJson(message));
                            this->sendDTO(std::make_shared<WskTransportDTO>(message, nullptr));
                        }

                        std::string WskServer::getNodeId() {
                            return nodeId;
                        }

                        void WskServer::update(std::shared_ptr<KeypleDto> event_Renamed) {
                            sendDTO(event_Renamed);
                        }
                    }
                }
            }
        }
    }
}
