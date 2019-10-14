#include "Demo_Webservice_MasterServer.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/TransportFactory.h"
#include "WsPollingFactory.h"
#include "../calypso/DemoThreads.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace wspolling {
                        using DemoThreads = org::eclipse::keyple::example::remote::calypso::DemoThreads;
                        using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;

                        void Demo_Webservice_MasterServer::main(std::vector<std::string> &args) throw(std::runtime_error) {

                            std::shared_ptr<TransportFactory> factory = std::make_shared<WsPollingFactory>(); // HTTP Web Polling

                            // Launch the server thread
                            DemoThreads::startServer(true, factory);

                            delay(1000);

                            // Launch the client thread
                            DemoThreads::startClient(false, factory);
                        }
                    }
                }
            }
        }
    }
}
