#include "RmDisconnectReaderTx.h"
#include "../../exception/KeypleRemoteException.h"
#include "../../transport/model/KeypleDto.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../transport/json/JsonParser.h"
#include "../../rm/RemoteMethod.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace nativese {
                        namespace method {
                            using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteException;
                            using RemoteMethod = org::eclipse::keyple::plugin::remotese::rm::RemoteMethod;
                            using RemoteMethodTx = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodTx;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
                            using com::google::gson::JsonObject;
const std::shared_ptr<org::slf4j::Logger> RmDisconnectReaderTx::logger = org::slf4j::LoggerFactory::getLogger(RmDisconnectReaderTx::typeid);

                            RmDisconnectReaderTx::RmDisconnectReaderTx(const std::string &sessionId, const std::string &nativeReaderName, const std::string &slaveNodeId) : org::eclipse::keyple::plugin::remotese::rm::RemoteMethodTx<Boolean>(sessionId, nativeReaderName, "", slaveNodeId) {
                            }

                            Boolean RmDisconnectReaderTx::parseResponse(std::shared_ptr<KeypleDto> keypleDto) throw(KeypleRemoteException) {
                                // if reader connection thrown an exception
                                if (KeypleDtoHelper::containsException(keypleDto)) {
                                    logger->trace("KeypleDto contains an exception: {}", keypleDto);
                                    std::shared_ptr<KeypleReaderException> ex = JsonParser::getGson()->fromJson(keypleDto->getBody(), KeypleReaderException::typeid);
                                    throw std::make_shared<KeypleRemoteException>("An exception occurs while calling the remote method disconnectReader", ex);
                                }
                                else {
                                    std::shared_ptr<JsonObject> body = JsonParser::getGson()->fromJson(keypleDto->getBody(), JsonObject::typeid);
                                    return body->get("status").getAsBoolean();
                                }

                            }

                            std::shared_ptr<KeypleDto> RmDisconnectReaderTx::dto() {
                                std::shared_ptr<JsonObject> body = std::make_shared<JsonObject>();
                                body->addProperty("sessionId", sessionId);

                                return std::make_shared<KeypleDto>(RemoteMethod::READER_DISCONNECT.getName(), JsonParser::getGson()->toJson(body, JsonObject::typeid), true, nullptr, nativeReaderName, nullptr, clientNodeId);
                            }
                        }
                    }
                }
            }
        }
    }
}
