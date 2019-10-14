#include "DemoWsKMasterClient.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/TransportFactory.h"
#include "WskFactory.h"
#include "../calypso/DemoThreads.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace websocket {
                        using DemoThreads = org::eclipse::keyple::example::remote::calypso::DemoThreads;
                        using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;

                        void DemoWsKMasterClient::main(std::vector<std::string> &args) throw(std::runtime_error) {

                            // Create the procotol factory
                            std::shared_ptr<TransportFactory> factory = std::make_shared<WskFactory>(false); // Web socket

                            // Launch the server thread
                            DemoThreads::startServer(false, factory);

                            delay(1000);

                            // Launch the client thread
                            DemoThreads::startClient(true, factory);
                        }
                    }
                }
            }
        }
    }
}
