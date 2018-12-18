#include "KeypleDtoHelper.h"
#include "KeypleDto.h"
#include "json/JsonParser.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace transport {
                        using JsonParser = org::eclipse::keyple::plugin::remotese::transport::json::JsonParser;
                        using com::google::gson::JsonObject;

                        std::string KeypleDtoHelper::toJson(std::shared_ptr<KeypleDto> keypleDto) {
                            return JsonParser::getGson()->toJson(keypleDto);
                        }

                        std::shared_ptr<KeypleDto> KeypleDtoHelper::fromJson(const std::string &json) {
                            return JsonParser::getGson()->fromJson(json, KeypleDto::typeid);
                        }

                        std::shared_ptr<KeypleDto> KeypleDtoHelper::fromJsonObject(std::shared_ptr<JsonObject> jsonObj) {
                            return JsonParser::getGson()->fromJson(jsonObj, KeypleDto::typeid);
                        }

                        std::shared_ptr<KeypleDto> KeypleDtoHelper::NoResponse() {
                            return std::make_shared<KeypleDto>("", "", false);
                        }

                        std::shared_ptr<KeypleDto> KeypleDtoHelper::ExceptionDTO(const std::string &action, std::runtime_error exception, const std::string &sessionId, const std::string &nativeReaderName, const std::string &virtualReaderName, const std::string &clientNodeId) {
                            return std::make_shared<KeypleDto>(action, JsonParser::getGson()->toJson(exception), false, sessionId, nativeReaderName, virtualReaderName, clientNodeId);
                        }

                        Boolean KeypleDtoHelper::isNoResponse(std::shared_ptr<KeypleDto> dto) {
                            return dto == nullptr || dto->getAction() == "" || dto->getAction().isEmpty();
                        }

                        Boolean KeypleDtoHelper::isKeypleDTO(const std::string &json) {
                            return isKeypleDTO(JsonParser::getGson()->fromJson(json, JsonObject::typeid));
                        }

                        std::shared_ptr<KeypleDto> KeypleDtoHelper::ACK() {
                            return std::make_shared<KeypleDto>("ACK", "ACK", false);
                        }

                        Boolean KeypleDtoHelper::isKeypleDTO(std::shared_ptr<JsonObject> json) {
                            return json->has("action");
                        }

                        Boolean KeypleDtoHelper::containsException(std::shared_ptr<KeypleDto> keypleDto) {
                            return keypleDto->getBody().find("stackTrace") != std::string::npos;
                        }
                    }
                }
            }
        }
    }
}
