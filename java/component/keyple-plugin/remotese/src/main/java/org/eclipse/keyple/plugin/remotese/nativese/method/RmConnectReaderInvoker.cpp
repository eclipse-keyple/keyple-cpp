#include "RmConnectReaderInvoker.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "../../transport/model/KeypleDto.h"
#include "../../rm/RemoteMethod.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace nativese {
                        namespace method {
                            using RemoteMethod = org::eclipse::keyple::plugin::remotese::rm::RemoteMethod;
                            using RemoteMethodInvoker = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodInvoker;
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                            using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;

                            RmConnectReaderInvoker::RmConnectReaderInvoker(std::shared_ptr<ProxyReader> localReader, const std::string &clientNodeId) : localReader(localReader), clientNodeId(clientNodeId) {
                            }

                            std::shared_ptr<KeypleDto> RmConnectReaderInvoker::dto() {
                                return std::make_shared<KeypleDto>(RemoteMethod::READER_CONNECT.getName(), "{}", true, nullptr, localReader->getName(), nullptr, clientNodeId);
                            }
                        }
                    }
                }
            }
        }
    }
}
