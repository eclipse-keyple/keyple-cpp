#include "JsonParser.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocol.h"
#include "GsonSeProtocolTypeAdapter.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace transport {
                        namespace json {
                            using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                            using com::google::gson::Gson;
                            using com::google::gson::GsonBuilder;

                            std::shared_ptr<Gson> JsonParser::getGson() {
                                std::shared_ptr<GsonBuilder> * const gsonBuilder = std::make_shared<GsonBuilder>();
                                gsonBuilder->registerTypeAdapter(SeProtocol::typeid, std::make_shared<GsonSeProtocolTypeAdapter>());
                                gsonBuilder->setPrettyPrinting();
                                return gsonBuilder->create();
                            }
                        }
                    }
                }
            }
        }
    }
}
