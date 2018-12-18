#include "LocalServer.h"
#include "LocalClient.h"
#include "../DtoHandler.h"
#include "../TransportDto.h"
#include "LocalTransportDto.h"
#include "../KeypleDtoHelper.h"
#include "../KeypleDto.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace transport {
                        namespace java {
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> LocalServer::logger = org::slf4j::LoggerFactory::getLogger(LocalServer::typeid);

                            LocalServer::LocalServer() {
                            }

                            void LocalServer::onLocalMessage(std::shared_ptr<TransportDto> transportDto) {
                                theClient = (std::static_pointer_cast<LocalTransportDto>(transportDto))->getTheClient();

                                if (dtoHandler != nullptr) {
                                    std::shared_ptr<TransportDto> response = dtoHandler->onDTO(std::make_shared<LocalTransportDto>(transportDto->getKeypleDTO(), theClient));
                                    // send back response
                                    this->sendDTO(response);
                                }
                                else {
                                    throw std::make_shared<IllegalStateException>("no DtoHanlder defined");
                                }
                            }

                            void LocalServer::start() {
                                logger->info("Local server start");
                            }

                            void LocalServer::setDtoHandler(std::shared_ptr<DtoHandler> handler) {
                                this->dtoHandler = handler;
                            }

                            void LocalServer::sendDTO(std::shared_ptr<TransportDto> transportDto) {
                                if (KeypleDtoHelper::isNoResponse(transportDto->getKeypleDTO())) {
                                    logger->trace("Keyple DTO is empty, do not send it");
                                }
                                else {
                                    // send keypleDto to the unique client
                                    theClient->onLocalMessage(transportDto->getKeypleDTO());
                                }
                            }

                            void LocalServer::sendDTO(std::shared_ptr<KeypleDto> keypleDto) {
                                if (KeypleDtoHelper::isNoResponse(keypleDto)) {
                                    logger->trace("Keyple DTO is empty, do not send it");
                                }
                                else {
                                    // send keypleDto to the unique client
                                    theClient->onLocalMessage(keypleDto);
                                }
                            }

                            std::string LocalServer::getNodeId() {
                                return "localServer1";
                            }

                            void LocalServer::update(std::shared_ptr<KeypleDto> event_Renamed) {
                                sendDTO(event_Renamed);
                            }
                        }
                    }
                }
            }
        }
    }
}
