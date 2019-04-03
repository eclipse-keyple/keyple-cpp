#include "RmSetDefaultSelectionRequestTx.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/DefaultSelectionRequest.h"
#include "../../exception/KeypleRemoteException.h"
#include "../../transport/model/KeypleDto.h"
#include "../../transport/json/JsonParser.h"
#include "../../rm/RemoteMethod.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {
                        namespace method {
                            using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::exception::KeypleRemoteException;
                            using RemoteMethod = org::eclipse::keyple::plugin::remotese::rm::RemoteMethod;
                            using RemoteMethodTx = org::eclipse::keyple::plugin::remotese::rm::RemoteMethodTx;
                            using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::model::KeypleDto;
                            using DefaultSelectionRequest = org::eclipse::keyple::seproxy::event_Renamed::DefaultSelectionRequest;
                            using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                            using com::google::gson::JsonObject;

                            RmSetDefaultSelectionRequestTx::RmSetDefaultSelectionRequestTx(std::shared_ptr<DefaultSelectionRequest> defaultSelectionRequest, ObservableReader::NotificationMode notificationMode, const std::string &nativeReaderName, const std::string &virtualReaderName, const std::string &sessionId, const std::string &clientNodeId) : org::eclipse::keyple::plugin::remotese::rm::RemoteMethodTx(sessionId, nativeReaderName, virtualReaderName, clientNodeId) {
                                this->defaultSelectionRequest = defaultSelectionRequest;
                                this->notificationMode = notificationMode;

                            }

                            std::shared_ptr<void> RmSetDefaultSelectionRequestTx::parseResponse(std::shared_ptr<KeypleDto> keypleDto) throw(KeypleRemoteException) {
                                return std::make_shared<Object>();

                            }

                            std::shared_ptr<KeypleDto> RmSetDefaultSelectionRequestTx::dto() {
                                std::shared_ptr<JsonObject> body = std::make_shared<JsonObject>();
                                body->addProperty("defaultSelectionRequest", JsonParser::getGson()->toJson(defaultSelectionRequest));
                                body->addProperty("notificationMode", notificationMode.getName());

                                return std::make_shared<KeypleDto>(RemoteMethod::DEFAULT_SELECTION_REQUEST.getName(), JsonParser::getGson()->toJson(body, JsonObject::typeid), true, sessionId, nativeReaderName, virtualReaderName, clientNodeId);

                            }
                        }
                    }
                }
            }
        }
    }
}
