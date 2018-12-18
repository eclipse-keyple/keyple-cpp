#include "RmConnectReaderExecutor.h"
#include "../RemoteSePlugin.h"
#include "../VirtualReader.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {
                        namespace method {
                            using RemoteSePlugin = org::eclipse::keyple::plugin::remotese::pluginse::RemoteSePlugin;
                            using VirtualReader = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                            using com::google::gson::JsonObject;
                            using com::google::gson::JsonPrimitive;

                            RmConnectReaderExecutor::RmConnectReaderExecutor(std::shared_ptr<RemoteSePlugin> plugin, std::shared_ptr<DtoSender> dtoSender) : plugin(plugin), dtoSender(dtoSender) {
                            }

                            std::shared_ptr<TransportDto> RmConnectReaderExecutor::execute(std::shared_ptr<TransportDto> transportDto) {
                                std::shared_ptr<KeypleDto> keypleDto = transportDto->getKeypleDTO();

                                // parseResponse msg
                                std::string nativeReaderName = keypleDto->getNativeReaderName();
                                std::string clientNodeId = keypleDto->getNodeId();

                                // create a virtual Reader
                                std::shared_ptr<VirtualReader> virtualReader = nullptr;
                                try {
                                    virtualReader = std::static_pointer_cast<VirtualReader>(this->plugin->createVirtualReader(clientNodeId, nativeReaderName, this->dtoSender));
                                    // response
                                    std::shared_ptr<JsonObject> respBody = std::make_shared<JsonObject>();
                                    respBody->add("statusCode", std::make_shared<JsonPrimitive>(0));
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                    return transportDto->nextTransportDTO(std::make_shared<KeypleDto>(keypleDto->getAction(), respBody->toString(), false, virtualReader->getSession()->getSessionId(), nativeReaderName, virtualReader->getName(), clientNodeId));
                                }
                                catch (const KeypleReaderException &e) {
                                    // virtual reader for remote reader already exists
                                    e->printStackTrace();
                                    // send the exception
                                    return transportDto->nextTransportDTO(KeypleDtoHelper::ExceptionDTO(keypleDto->getAction(), e, "", nativeReaderName, "", clientNodeId));

                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
