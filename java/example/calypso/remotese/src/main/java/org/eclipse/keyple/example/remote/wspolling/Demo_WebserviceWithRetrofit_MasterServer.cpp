#include "Demo_WebserviceWithRetrofit_MasterServer.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/TransportFactory.h"
#include "client_retrofit/WsPollingRetrofitFactory.h"
#include "../calypso/DemoThreads.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace wspolling {
                        using DemoThreads = org::eclipse::keyple::example::remote::calypso::DemoThreads;
                        using WsPollingRetrofitFactory = org::eclipse::keyple::example::remote::wspolling::client_retrofit::WsPollingRetrofitFactory;
                        using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;

                        void Demo_WebserviceWithRetrofit_MasterServer::main(std::vector<std::string> &args) throw(std::runtime_error) {

                            // Create the procotol factory
                            std::shared_ptr<TransportFactory> factory = std::make_shared<WsPollingRetrofitFactory>(); // HTTP Web Polling

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
