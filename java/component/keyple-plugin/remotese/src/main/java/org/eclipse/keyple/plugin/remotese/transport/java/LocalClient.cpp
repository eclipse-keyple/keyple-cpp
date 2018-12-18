#include "LocalClient.h"
#include "LocalServer.h"
#include "../DtoHandler.h"
#include "../KeypleDto.h"
#include "../TransportDto.h"
#include "LocalTransportDto.h"
#include "../KeypleDtoHelper.h"
#include "../KeypleRemoteException.h"

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
const std::shared_ptr<org::slf4j::Logger> LocalClient::logger = org::slf4j::LoggerFactory::getLogger(LocalClient::typeid);

                            LocalClient::LocalClient(std::shared_ptr<LocalServer> server) : theServer(server) {
                            }

                            void LocalClient::onLocalMessage(std::shared_ptr<KeypleDto> keypleDto) {
                                if (dtoHandler != nullptr) {
                                    std::shared_ptr<TransportDto> response = dtoHandler->onDTO(std::make_shared<LocalTransportDto>(keypleDto, shared_from_this()));
                                    // send back response
                                    this->sendDTO(response);
                                }
                                else {
                                    throw std::make_shared<IllegalStateException>("no DtoHandler defined");
                                }
                            }

                            void LocalClient::setDtoHandler(std::shared_ptr<DtoHandler> handler) {
                                this->dtoHandler = handler;
                            }

                            void LocalClient::sendDTO(std::shared_ptr<TransportDto> transportDto) {
                                if (KeypleDtoHelper::isNoResponse(transportDto->getKeypleDTO())) {
                                    logger->trace("Keyple DTO is empty, do not send it");
                                }
                                else {
                                    // send keypleDto to the server
                                    theServer->onLocalMessage(transportDto);
                                }
                            }

                            void LocalClient::sendDTO(std::shared_ptr<KeypleDto> keypleDto) throw(KeypleRemoteException) {

                                if (theServer == nullptr) {
                                    throw std::make_shared<KeypleRemoteException>("Unable to connect to server");
                                }

                                if (KeypleDtoHelper::isNoResponse(keypleDto)) {
                                    logger->trace("Keyple DTO is empty, do not send it");
                                }
                                else {
                                    // send keypleDto to the server
                                    theServer->onLocalMessage(std::make_shared<LocalTransportDto>(keypleDto, shared_from_this()));
                                }
                            }

                            std::string LocalClient::getNodeId() {
                                return "localClient1";
                            }

                            void LocalClient::update(std::shared_ptr<KeypleDto> event_Renamed) {
                                try {
                                    sendDTO(event_Renamed);
                                }
                                catch (const KeypleRemoteException &e) {
                                    // Error is not propagated
                                    logger->error("Exception while sending event throw KeypleRemoteInterface", e);
                                }
                            }

                            void LocalClient::connect() {
                                // dummy
                                logger->info("Connect Local Client");
                            }

                            void LocalClient::disconnect() {
                                // dummy
                                logger->info("Disconnect Local Client");
                            }
                        }
                    }
                }
            }
        }
    }
}
