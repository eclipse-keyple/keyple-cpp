#include "RmDisconnectReaderExecutor.h"
#include "RemoteSePlugin.h"
#include "../transport/model/TransportDto.h"
#include "../transport/model/KeypleDto.h"
#include "../rm/RemoteMethod.h"
#include "../transport/json/JsonParser.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {
                        using RemoteMethod = org::eclipse::keyple::plugin::remotese::rm::RemoteMethod;
                        using RemoteMethodExecutor = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodExecutor;
                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                        using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                        using TransportDto = org::eclipse::keyple::plugin::remotese::transport::model::TransportDto;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
                        using com::google::gson::JsonObject;
const std::shared_ptr<org::slf4j::Logger> RmDisconnectReaderExecutor::logger = org::slf4j::LoggerFactory::getLogger(RmDisconnectReaderExecutor::typeid);

                        RmDisconnectReaderExecutor::RmDisconnectReaderExecutor(std::shared_ptr<RemoteSePlugin> plugin) : plugin(plugin) {
                        }

                        std::shared_ptr<TransportDto> RmDisconnectReaderExecutor::execute(std::shared_ptr<TransportDto> transportDto) {
                            std::shared_ptr<KeypleDto> keypleDto = transportDto->getKeypleDTO();

                            std::string nativeReaderName = keypleDto->getNativeReaderName();
                            std::string clientNodeId = keypleDto->getNodeId();
                            std::string sessionId = keypleDto->getSessionId();

                            try {
                                // todo use sessionId is present
                                plugin->disconnectRemoteReader(nativeReaderName);
                                std::shared_ptr<JsonObject> body = std::make_shared<JsonObject>();
                                body->addProperty("status", true);
                                return transportDto->nextTransportDTO(std::make_shared<KeypleDto>(RemoteMethod::READER_DISCONNECT.getName(), JsonParser::getGson()->toJson(body, JsonObject::typeid), false, nullptr, nativeReaderName, nullptr, clientNodeId));
                            }
                            catch (const KeypleReaderNotFoundException &e) {
                                logger->error("Impossible to disconnect reader " + nativeReaderName, e);
                                return transportDto->nextTransportDTO(KeypleDtoHelper::ExceptionDTO(RemoteMethod::READER_DISCONNECT.getName(), e, keypleDto->getSessionId(), keypleDto->getNativeReaderName(), keypleDto->getVirtualReaderName(), keypleDto->getNodeId()));
                            }

                        }
                    }
                }
            }
        }
    }
}
