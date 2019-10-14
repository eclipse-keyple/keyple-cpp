#include "WskClient.h"
#include "WskTransportDTO.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace websocket {
                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using org::java_websocket::client::WebSocketClient;
                        using org::java_websocket::handshake::ServerHandshake;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> WskClient::logger = org::slf4j::LoggerFactory::getLogger(WskClient::typeid);

                        WskClient::WskClient(std::shared_ptr<URI> url, const std::string &nodeId) : org::java_websocket::client::WebSocketClient(url), nodeId(nodeId) {
                        }

                        void WskClient::onOpen(std::shared_ptr<ServerHandshake> handshakedata) {
                            logger->trace("Web socket onOpen {}", handshakedata);

                        }

                        void WskClient::onMessage(const std::string &message) {
                            logger->trace("Web socket onMessage {}", message);
                            std::shared_ptr<KeypleDto> dto = KeypleDtoHelper::fromJson(message);

                            // process dto
                            std::shared_ptr<TransportDto> transportDto = dtoHandler->onDTO(std::make_shared<WskTransportDTO>(dto, nullptr, shared_from_this()));

                            // there is a response/request to send back
                            if (!KeypleDtoHelper::isNoResponse(transportDto->getKeypleDTO())) {
                                this->sendDTO(transportDto);
                            }
                        }

                        void WskClient::onClose(int code, const std::string &reason, bool remote) {
                            logger->trace("Web socket onClose {} {}", code, reason);

                        }

                        void WskClient::onError(std::runtime_error ex) {
                            logger->trace("Web socket onError {}", ex);

                        }

                        void WskClient::sendDTO(std::shared_ptr<TransportDto> transportDto) {
                            this->sendDTO(transportDto->getKeypleDTO());
                        }

                        void WskClient::sendDTO(std::shared_ptr<KeypleDto> keypleDto) {
                            // if keypleDTO is no empty
                            if (!KeypleDtoHelper::isNoResponse(keypleDto)) {
                                logger->trace("send DTO {}", KeypleDtoHelper::toJson(keypleDto));
                                this->send(KeypleDtoHelper::toJson(keypleDto));
                            }
                            else {
                                logger->debug("No message to send back");
                            }
                        }

                        std::string WskClient::getNodeId() {
                            return nodeId;
                        }

                        void WskClient::setDtoHandler(std::shared_ptr<DtoHandler> receiver) {
                            this->dtoHandler = receiver;
                        }

                        void WskClient::update(std::shared_ptr<KeypleDto> event_Renamed) {
                            this->sendDTO(event_Renamed);
                        }

                        void WskClient::disconnect() {
                            this->getConnection()->close();
                        }
                    }
                }
            }
        }
    }
}
