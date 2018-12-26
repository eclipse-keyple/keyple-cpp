#include "RmSetDefaultSelectionRequestTx.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/transaction/SelectionRequest.h"
#include "../../transport/KeypleDto.h"
#include "../../transport/KeypleRemoteException.h"
#include "../../transport/json/JsonParser.h"
#include "../../transport/RemoteMethod.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace pluginse {
                        namespace method {
                            using KeypleDto = org::eclipse::keyple::plugin::remotese::transport::KeypleDto;
                            using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::transport::KeypleRemoteException;
                            using RemoteMethod = org::eclipse::keyple::plugin::remotese::transport::RemoteMethod;
                            using RemoteMethodTx = org::eclipse::keyple::plugin::remotese::transport::RemoteMethodTx;
                            using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                            using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                            using SelectionRequest = org::eclipse::keyple::transaction::SelectionRequest;
                            using com::google::gson::JsonObject;

                            RmSetDefaultSelectionRequestTx::RmSetDefaultSelectionRequestTx(std::shared_ptr<SelectionRequest> selectionRequest, ObservableReader::NotificationMode notificationMode, const std::string &nativeReaderName, const std::string &virtualReaderName, const std::string &sessionId, const std::string &clientNodeId) : org::eclipse::keyple::plugin::remotese::transport::RemoteMethodTx(sessionId, nativeReaderName, virtualReaderName, clientNodeId) {
                                this->selectionRequest = selectionRequest;
                                this->notificationMode = notificationMode;

                            }

                            std::shared_ptr<void> RmSetDefaultSelectionRequestTx::parseResponse(std::shared_ptr<KeypleDto> keypleDto) throw(KeypleRemoteException) {
                                return std::make_shared<Object>();

                            }

                            std::shared_ptr<KeypleDto> RmSetDefaultSelectionRequestTx::dto() {
                                std::shared_ptr<JsonObject> body = std::make_shared<JsonObject>();
                                body->addProperty("selectionRequest", JsonParser::getGson()->toJson(selectionRequest));
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
