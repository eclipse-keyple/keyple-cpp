#include "GsonSeProtocolTypeAdapter.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocol.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactsProtocols.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/Protocol.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace transport {
                        namespace json {
                            using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                            using ContactsProtocols = org::eclipse::keyple::seproxy::protocol::ContactsProtocols;
                            using Protocol = org::eclipse::keyple::seproxy::protocol::Protocol;
                            using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                            using namespace com::google::gson;

                            std::shared_ptr<JsonElement> GsonSeProtocolTypeAdapter::serialize(std::shared_ptr<SeProtocol> src, std::shared_ptr<Type> typeOfSrc, std::shared_ptr<JsonSerializationContext> context) {
                                return std::make_shared<JsonPrimitive>(src->getName());
                            }

                            std::shared_ptr<SeProtocol> GsonSeProtocolTypeAdapter::deserialize(std::shared_ptr<JsonElement> json, std::shared_ptr<Type> typeOfT, std::shared_ptr<JsonDeserializationContext> context) throw(JsonParseException) {

                                std::string value = json->getAsString();

                                for (ContactlessProtocols p : ContactlessProtocols::values()) {
                                    if (p.name().equals(value)) {
                                        return p;
                                    }
                                }

                                for (ContactsProtocols p : ContactsProtocols::values()) {
                                    if (p.name().equals(value)) {
                                        return p;
                                    }
                                }

                                for (Protocol p : Protocol::values()) {
                                    if (p.name().equals(value)) {
                                        return p;
                                    }
                                }
                                throw std::make_shared<JsonParseException>("Value of SeProtocol not found : " + value);



                            }
                        }
                    }
                }
            }
        }
    }
}
