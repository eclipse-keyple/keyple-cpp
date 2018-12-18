#include "GsonSelectorTypeAdapter.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequest.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace transport {
                        namespace json {
                            using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
                            using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                            using namespace com::google::gson;

                            std::shared_ptr<JsonElement> GsonSelectorTypeAdapter::serialize(std::shared_ptr<SeRequest::Selector> src, std::shared_ptr<Type> typeOfSrc, std::shared_ptr<JsonSerializationContext> context) {
                                if (std::dynamic_pointer_cast<SeRequest::AidSelector>(src) != nullptr) {
                                    return std::make_shared<JsonPrimitive>("aidselector::" + ByteArrayUtils::toHex((std::static_pointer_cast<SeRequest::AidSelector>(src))->getAidToSelect()));
                                }
                                else {
                                    return std::make_shared<JsonPrimitive>("atrselector::" + (std::static_pointer_cast<SeRequest::AtrSelector>(src))->getAtrRegex());
                                }
                            }

                            std::shared_ptr<SeRequest::Selector> GsonSelectorTypeAdapter::deserialize(std::shared_ptr<JsonElement> json, std::shared_ptr<Type> typeOfT, std::shared_ptr<JsonDeserializationContext> context) throw(JsonParseException) {
                                std::string element = json->getAsString();
                                if (StringHelper::startsWith(element, "atrselector::")) {
                                    std::string regex = StringHelper::replace(element, "atrselector::", "");
                                    return std::make_shared<SeRequest::AtrSelector>(regex);
                                }
                                else {
                                    if (StringHelper::startsWith(element, "aidselector::")) {
                                        std::string aidToSelect = StringHelper::replace(element, "aidselector::", "");
                                        return std::make_shared<SeRequest::AidSelector>(ByteArrayUtils::fromHex(aidToSelect));

                                    }
                                    else {
                                        throw std::make_shared<JsonParseException>("SeRequest.Selector malformed");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
