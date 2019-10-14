#include "RmDisconnectReaderExecutor.h"
#include "../RemoteSePlugin.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {
                        namespace method {
                            using RemoteSePlugin = org::eclipse::keyple::plugin::remotese::pluginse::RemoteSePlugin;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> RmDisconnectReaderExecutor::logger = org::slf4j::LoggerFactory::getLogger(RmDisconnectReaderExecutor::typeid);

                            RmDisconnectReaderExecutor::RmDisconnectReaderExecutor(std::shared_ptr<RemoteSePlugin> plugin) : plugin(plugin) {
                            }

                            std::shared_ptr<TransportDto> RmDisconnectReaderExecutor::execute(std::shared_ptr<TransportDto> transportDto) {
                                std::shared_ptr<KeypleDto> keypleDto = transportDto->getKeypleDTO();

                                std::string nativeReaderName = keypleDto->getNativeReaderName();
                                std::string clientNodeId = keypleDto->getNodeId();

                                try {
                                    plugin->disconnectRemoteReader(nativeReaderName);
                                    return transportDto->nextTransportDTO(std::make_shared<KeypleDto>(RemoteMethod::READER_DISCONNECT.getName(), "{}", false, nullptr, nativeReaderName, nullptr, clientNodeId));
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
}
