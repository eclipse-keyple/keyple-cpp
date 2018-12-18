#include "RmDisconnectReaderInvoker.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "../../transport/KeypleDto.h"
#include "../../transport/RemoteMethod.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace nativese {
                        namespace method {
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                            using RemoteMethod = org::eclipse::keyple::plugin::remotese::transport::RemoteMethod;
                            using RemoteMethodInvoker = org::eclipse::keyple::plugin::remotese::transport::RemoteMethodInvoker;
                            using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;

                            RmDisconnectReaderInvoker::RmDisconnectReaderInvoker(std::shared_ptr<ProxyReader> localReader, const std::string &clientNodeId) : localReader(localReader), clientNodeId(clientNodeId) {
                            }

                            std::shared_ptr<KeypleDto> RmDisconnectReaderInvoker::dto() {
                                return std::make_shared<KeypleDto>(RemoteMethod::READER_DISCONNECT.getName(), "{}", true, nullptr, localReader->getName(), nullptr, clientNodeId);
                            }
                        }
                    }
                }
            }
        }
    }
}
