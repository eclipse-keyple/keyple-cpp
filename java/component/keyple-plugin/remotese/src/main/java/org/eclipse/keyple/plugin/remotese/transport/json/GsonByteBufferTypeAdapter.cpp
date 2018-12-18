#include "GsonByteBufferTypeAdapter.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace transport {
                        namespace json {
                            using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                            using namespace com::google::gson;

                            std::shared_ptr<JsonElement> GsonByteBufferTypeAdapter::serialize(std::vector<char> &src, std::shared_ptr<Type> typeOfSrc, std::shared_ptr<JsonSerializationContext> context) {
                                return std::make_shared<JsonPrimitive>(ByteArrayUtils::toHex(src));
                            }

                            std::vector<char> GsonByteBufferTypeAdapter::deserialize(std::shared_ptr<JsonElement> json, std::shared_ptr<Type> typeOfT, std::shared_ptr<JsonDeserializationContext> context) throw(JsonParseException) {
                                return ByteArrayUtils::fromHex(json->getAsString());
                            }
                        }
                    }
                }
            }
        }
    }
}
