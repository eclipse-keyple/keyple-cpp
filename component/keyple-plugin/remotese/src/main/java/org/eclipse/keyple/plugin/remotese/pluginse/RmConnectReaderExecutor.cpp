#include "RmConnectReaderExecutor.h"
#include "RemoteSePlugin.h"
#include "../transport/model/TransportDto.h"
#include "../transport/model/KeypleDto.h"
#include "VirtualReader.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {
                        using RemoteMethodExecutor = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodExecutor;
                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                        using TransportDto = org::eclipse::keyple::plugin::remotese::transport::model::TransportDto;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
                        using com::google::gson::JsonObject;
                        using com::google::gson::JsonPrimitive;
const std::shared_ptr<org::slf4j::Logger> RmConnectReaderExecutor::logger = org::slf4j::LoggerFactory::getLogger(RmConnectReaderExecutor::typeid);

                        RmConnectReaderExecutor::RmConnectReaderExecutor(std::shared_ptr<RemoteSePlugin> plugin, std::shared_ptr<DtoSender> dtoSender) : plugin(plugin), dtoSender(dtoSender) {
                        }

                        std::shared_ptr<TransportDto> RmConnectReaderExecutor::execute(std::shared_ptr<TransportDto> transportDto) {
                            std::shared_ptr<KeypleDto> keypleDto = transportDto->getKeypleDTO();

                            // parseResponse msg
                            std::string nativeReaderName = keypleDto->getNativeReaderName();
                            std::string clientNodeId = keypleDto->getNodeId();

                            std::shared_ptr<VirtualReader> virtualReader = nullptr;
                            try {
                                // create a virtual Reader
                                virtualReader = std::static_pointer_cast<VirtualReader>(this->plugin->createVirtualReader(clientNodeId, nativeReaderName, this->dtoSender));

                                // create response
                                std::shared_ptr<JsonObject> respBody = std::make_shared<JsonObject>();
                                respBody->add("statusCode", std::make_shared<JsonPrimitive>(0));
                                respBody->add("sessionId", std::make_shared<JsonPrimitive>(virtualReader->getSession()->getSessionId()));

                                // build transport DTO with body
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                return transportDto->nextTransportDTO(std::make_shared<KeypleDto>(keypleDto->getAction(), respBody->toString(), false, virtualReader->getSession()->getSessionId(), nativeReaderName, virtualReader->getName(), clientNodeId));

                            }
                            catch (const KeypleReaderException &e) {
                                // virtual reader for remote reader already exists
                                logger->warn("Virtual reader already exists for reader " + nativeReaderName, e);

                                // send the exception inside the dto
                                return transportDto->nextTransportDTO(KeypleDtoHelper::ExceptionDTO(keypleDto->getAction(), e, "", nativeReaderName, "", clientNodeId));

                            }
                        }
                    }
                }
            }
        }
    }
}
