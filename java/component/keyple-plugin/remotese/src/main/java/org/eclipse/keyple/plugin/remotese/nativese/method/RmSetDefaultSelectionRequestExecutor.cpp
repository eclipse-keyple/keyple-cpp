#include "RmSetDefaultSelectionRequestExecutor.h"
#include "../NativeReaderServiceImpl.h"
#include "../../transport/json/JsonParser.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/transaction/SelectionRequest.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace nativese {
                        namespace method {
                            using NativeReaderServiceImpl = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderServiceImpl;
                            using namespace org::eclipse::keyple::plugin::remotese::transport;
                            using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                            using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                            using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                            using SelectionRequest = org::eclipse::keyple::transaction::SelectionRequest;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
                            using com::google::gson::JsonObject;
                            using com::google::gson::JsonPrimitive;
const std::shared_ptr<org::slf4j::Logger> RmSetDefaultSelectionRequestExecutor::logger = org::slf4j::LoggerFactory::getLogger(RmSetDefaultSelectionRequestExecutor::typeid);

                            RmSetDefaultSelectionRequestExecutor::RmSetDefaultSelectionRequestExecutor(std::shared_ptr<NativeReaderServiceImpl> nativeReaderService) : nativeReaderService(nativeReaderService) {
                            }

                            std::shared_ptr<TransportDto> RmSetDefaultSelectionRequestExecutor::execute(std::shared_ptr<TransportDto> transportDto) {
                                std::shared_ptr<KeypleDto> keypleDto = transportDto->getKeypleDTO();

                                // Extract info from keypleDto
                                std::string body = keypleDto->getBody();
                                std::shared_ptr<JsonObject> jsonObject = JsonParser::getGson()->fromJson(body, JsonObject::typeid);

                                std::shared_ptr<JsonPrimitive> selectionRequestJson = jsonObject->getAsJsonPrimitive("selectionRequest");
                                std::shared_ptr<JsonPrimitive> notificationModeJson = jsonObject->getAsJsonPrimitive("notificationMode");

                                logger->debug(selectionRequestJson->getAsString());
                                logger->debug(notificationModeJson->getAsString());

                                std::shared_ptr<SelectionRequest> selectionRequest = JsonParser::getGson()->fromJson(selectionRequestJson->getAsString(), SelectionRequest::typeid);
                                ObservableReader::NotificationMode notificationMode = ObservableReader::NotificationMode.get(notificationModeJson->getAsString());

                                std::string nativeReaderName = keypleDto->getNativeReaderName();
                                logger->debug("Execute locally SetDefaultSelectionRequest : {} - {}", notificationMode, selectionRequest);

                                try {
                                    // find native reader by name
                                    std::shared_ptr<ProxyReader> reader = nativeReaderService->findLocalReader(nativeReaderName);

                                    if (std::dynamic_pointer_cast<ObservableReader>(reader) != nullptr) {
                                        logger->debug(reader->getName() + " is an ObservableReader, invoke setDefaultSelectionRequest on it");
                                        (std::static_pointer_cast<ObservableReader>(reader))->setDefaultSelectionRequest(selectionRequest, notificationMode);

                                        // prepare response
                                        std::string parseBody = "{}";
                                        return transportDto->nextTransportDTO(std::make_shared<KeypleDto>(RemoteMethod::DEFAULT_SELECTION_REQUEST.getName(), parseBody, false, keypleDto->getSessionId(), nativeReaderName, keypleDto->getVirtualReaderName(), keypleDto->getNodeId()));
                                    }
                                    else {
                                        throw std::make_shared<KeypleReaderException>("Reader is not observable, can not invoke SetDefaultSelectionRequest on " + nativeReaderName);
                                    }


                                }
                                catch (const KeypleReaderException &e) {
                                    // if an exception occurs, send it into a keypleDto to the Master
                                    return transportDto->nextTransportDTO(KeypleDtoHelper::ExceptionDTO(RemoteMethod::DEFAULT_SELECTION_REQUEST.getName(), e, keypleDto->getSessionId(), nativeReaderName, keypleDto->getVirtualReaderName(), keypleDto->getNodeId()));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
