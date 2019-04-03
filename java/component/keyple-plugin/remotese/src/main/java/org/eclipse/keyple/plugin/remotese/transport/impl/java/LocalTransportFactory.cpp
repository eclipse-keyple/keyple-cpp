#include "LocalTransportFactory.h"
#include "LocalClient.h"
#include "LocalServer.h"
#include "../../factory/ClientNode.h"
#include "../../factory/ServerNode.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace transport {
                        namespace impl {
                            namespace java {
                                using ClientNode = org::eclipse::keyple::plugin::remotese::transport::factory::ClientNode;
                                using ServerNode = org::eclipse::keyple::plugin::remotese::transport::factory::ServerNode;
                                using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::factory::TransportFactory;

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
}
