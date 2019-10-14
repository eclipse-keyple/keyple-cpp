#include "RmConnectReaderTx.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "../NativeReaderService.h"
#include "../../exception/KeypleRemoteException.h"
#include "../../transport/model/KeypleDto.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../transport/json/JsonParser.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractSelectionLocalReader.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"
#include "../../rm/RemoteMethod.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace nativese {
                        namespace method {
                            using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteException;
                            using NativeReaderService = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderService;
                            using RemoteMethod = org::eclipse::keyple::plugin::remotese::rm::RemoteMethod;
                            using RemoteMethodTx = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodTx;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                            using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                            using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                            using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                            using AbstractSelectionLocalReader = org::eclipse::keyple::seproxy::plugin::AbstractSelectionLocalReader;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
                            using com::google::gson::JsonObject;

                            RmConnectReaderTx::RmConnectReaderTx(const std::string &sessionId, const std::string &nativeReaderName, const std::string &virtualReaderName, const std::string &clientNodeId, std::shared_ptr<ProxyReader> localReader, const std::string &clientNodeId1, std::shared_ptr<NativeReaderService> nativeReaderService) : org::eclipse::keyple::plugin::remotese::rm::RemoteMethodTx<String>(sessionId, nativeReaderName, virtualReaderName, clientNodeId), localReader(localReader), clientNodeId(clientNodeId1), nativeReaderService(nativeReaderService) {
                            }

const std::shared_ptr<org::slf4j::Logger> RmConnectReaderTx::logger = org::slf4j::LoggerFactory::getLogger(RmConnectReaderTx::typeid);

                            std::string RmConnectReaderTx::parseResponse(std::shared_ptr<KeypleDto> keypleDto) throw(KeypleRemoteException) {
                                std::string nativeReaderName = keypleDto->getNativeReaderName();

                                // if reader connection thrown an exception
                                if (KeypleDtoHelper::containsException(keypleDto)) {
                                    logger->trace("KeypleDto contains an exception: {}", keypleDto);
                                    std::shared_ptr<KeypleReaderException> ex = JsonParser::getGson()->fromJson(keypleDto->getBody(), KeypleReaderException::typeid);
                                    throw std::make_shared<KeypleRemoteException>("An exception occurs while calling the remote method connecReader", ex);
                                }
                                else {
                                    // if dto does not contain an exception
                                    try {
                                        // configure nativeReaderService to propagate reader events if the reader is
                                        // observable

                                        // find the local reader by name
                                        std::shared_ptr<ProxyReader> localReader = std::static_pointer_cast<ProxyReader>(nativeReaderService->findLocalReader(nativeReaderName));

                                        if (std::dynamic_pointer_cast<AbstractSelectionLocalReader>(localReader) != nullptr) {
                                            logger->debug("Register NativeReaderService as an observer for native reader {}", localReader->getName());
                                            (std::static_pointer_cast<AbstractSelectionLocalReader>(localReader))->addObserver(std::static_pointer_cast<ObservableReader::ReaderObserver>(nativeReaderService));
                                        }

                                        // retrieve sessionId from keypleDto
                                        std::shared_ptr<JsonObject> body = JsonParser::getGson()->fromJson(keypleDto->getBody(), JsonObject::typeid);
                                        // Integer statusCode = body.get("statusCode").getAsInt();
                                        std::string sessionId = body->get("sessionId").getAsString();

                                        // sessionId is returned here
                                        return sessionId;

                                    }
                                    catch (const KeypleReaderNotFoundException &e) {
                                        logger->warn("While receiving a confirmation of Rse connection, local reader was not found");
                                        throw std::make_shared<KeypleRemoteException>("While receiving a confirmation of Rse connection, local reader was not found");
                                    }
                                }
                            }

                            std::shared_ptr<KeypleDto> RmConnectReaderTx::dto() {
                                return std::make_shared<KeypleDto>(RemoteMethod::READER_CONNECT.getName(), "{}", true, nullptr, localReader->getName(), nullptr, clientNodeId);
                            }
                        }
                    }
                }
            }
        }
    }
}
