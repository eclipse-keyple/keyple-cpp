#include "Server.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/example/remote/wspolling/server/WsPServer.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/KeypleDtoHelper.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace wspolling {
                        using WsPServer = org::eclipse::keyple::example::remote::wspolling::server::WsPServer;
                        using KeypleDtoHelper = org::eclipse::keyple::plugin::remotese::transport::KeypleDtoHelper;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> Server::logger = org::slf4j::LoggerFactory::getLogger(Server::typeid);

                        void Server::boot() throw(IOException) {


                            logger->info("*************************************");
                            logger->info("Start Polling Webservice server      ");
                            logger->info("*************************************");

                            std::shared_ptr<WsPServer> server = std::make_shared<WsPServer>(URL, PORT, END_POINT, POLLING_END_POINT, "server");
                            server->start();


                            for (int i = 0; i < 10; i++) {

                                try {
                                    delay(17000);

                                    // send message to client by polling (suppose that a client is polling)
                                    server->update(KeypleDtoHelper::ACK());

                                }
                                catch (const InterruptedException &e) {
                                    e->printStackTrace();
                                }
                            }


                        }

                        void Server::main(std::vector<std::string> &args) throw(std::runtime_error) {

                            std::shared_ptr<Server> server = std::make_shared<Server>();
                            server->boot();
                        }
                    }
                }
            }
        }
    }
}
