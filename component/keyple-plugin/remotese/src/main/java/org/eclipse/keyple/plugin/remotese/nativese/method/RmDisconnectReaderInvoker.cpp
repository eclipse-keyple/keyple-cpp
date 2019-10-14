#include "RmDisconnectReaderInvoker.h"
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
                            using com::google::gson::JsonObject;

                            RmDisconnectReaderInvoker::RmDisconnectReaderInvoker(const std::string &sessionId, const std::string &nativeReaderName, const std::string &slaveNodeId) : sessionId(sessionId), nativeReaderName(nativeReaderName), slaveNodeId(slaveNodeId) {
                            }

                            std::shared_ptr<KeypleDto> RmDisconnectReaderInvoker::dto() {

                                std::shared_ptr<JsonObject> body = std::make_shared<JsonObject>();
                                body->addProperty("sessionId", sessionId);

                                return std::make_shared<KeypleDto>(RemoteMethod::READER_DISCONNECT.getName(), body->getAsString(), true, nullptr, nativeReaderName, nullptr, slaveNodeId);
                            }
                        }
                    }
                }
            }
        }
    }
}
