#include "LocalTransportFactory.h"
#include "LocalClient.h"
#include "LocalServer.h"
#include "../ClientNode.h"
#include "../ServerNode.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace transport {
                        namespace java {
                            using ClientNode = org::eclipse::keyple::plugin::remotese::transport::ClientNode;
                            using ServerNode = org::eclipse::keyple::plugin::remotese::transport::ServerNode;
                            using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;

                            LocalTransportFactory::LocalTransportFactory() : theClient(std::make_shared<LocalClient>(theServer)), theServer(std::make_shared<LocalServer>()) {
                            }

                            std::shared_ptr<ClientNode> LocalTransportFactory::getClient() {
                                return theClient;
                            }

                            std::shared_ptr<ServerNode> LocalTransportFactory::getServer() {
                                return theServer;
                            }
                        }
                    }
                }
            }
        }
    }
}
