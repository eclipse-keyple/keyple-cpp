#include "DemoThreads.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/TransportFactory.h"
#include "DemoMaster.h"
#include "DemoSlave.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/KeypleRemoteException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace calypso {
                        using KeypleRemoteException = org::eclipse::keyple::plugin::remotese::transport::KeypleRemoteException;
                        using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> DemoThreads::logger = org::slf4j::LoggerFactory::getLogger(DemoThreads::typeid);

                        void DemoThreads::startServer(Boolean const isMaster, std::shared_ptr<TransportFactory> factory) {
                            std::shared_ptr<Thread> server = std::make_shared<ThreadAnonymousInnerClass>(isMaster, factory);

                            server->start();
                        }

                        DemoThreads::ThreadAnonymousInnerClass::ThreadAnonymousInnerClass(Boolean isMaster, std::shared_ptr<TransportFactory> factory) {
                            this->isMaster = isMaster;
                            this->factory = factory;
                        }

                        void DemoThreads::ThreadAnonymousInnerClass::run() {
                            try {

                                logger->info("**** Starting Server Thread ****");

                                if (isMaster) {
                                    std::shared_ptr<DemoMaster> master = std::make_shared<DemoMaster>(factory, true);
                                    master->boot();

                                }
                                else {
                                    std::shared_ptr<DemoSlave> slave = std::make_shared<DemoSlave>(factory, true);
                                    logger->info("Wait for 5 seconds, then connectAReader to master");
                                    delay(5000);
                                    slave->connectAReader();
                                    logger->info("Wait for 5 seconds, then insert SE");
                                    delay(5000);
                                    slave->insertSe();
                                    logger->info("Wait for 5 seconds, then remove SE");
                                    delay(5000);
                                    slave->removeSe();
                                    logger->info("Wait for 5 seconds, then disconnect reader");
                                    delay(5000);
                                    slave->disconnect();
                                }

                            }
                            catch (const KeypleReaderNotFoundException &e) {
                                e->printStackTrace();
                            }
                            catch (const InterruptedException &e) {
                                e->printStackTrace();
                            }
                            catch (const KeypleReaderException &e) {
                                e->printStackTrace();
                            }
                            catch (const KeypleRemoteException &e) {
                                e->printStackTrace();
                            }

                        }

                        void DemoThreads::startClient(Boolean const isMaster, std::shared_ptr<TransportFactory> factory) {
                            std::shared_ptr<Thread> client = std::make_shared<ThreadAnonymousInnerClass2>(isMaster, factory);
                            client->start();
                        }

                        DemoThreads::ThreadAnonymousInnerClass2::ThreadAnonymousInnerClass2(Boolean isMaster, std::shared_ptr<TransportFactory> factory) {
                            this->isMaster = isMaster;
                            this->factory = factory;
                        }

                        void DemoThreads::ThreadAnonymousInnerClass2::run() {
                            logger->info("**** Starting Client Thread ****");

                            try {
                                if (isMaster) {
                                    std::shared_ptr<DemoMaster> master = std::make_shared<DemoMaster>(factory, false);
                                    master->boot();
                                }
                                else {
                                    std::shared_ptr<DemoSlave> slave = std::make_shared<DemoSlave>(factory, false);
                                    slave->connectAReader();
                                    logger->info("Wait for 5 seconds, then insert SE");
                                    delay(5000);
                                    slave->insertSe();
                                    logger->info("Wait for 5 seconds, then remove SE");
                                    delay(5000);
                                    slave->removeSe();
                                    logger->info("Wait for 5 seconds, then disconnect reader");
                                    delay(5000);
                                    slave->disconnect();
                                }

                            }
                            catch (const KeypleReaderNotFoundException &e) {
                                e->printStackTrace();
                            }
                            catch (const InterruptedException &e) {
                                e->printStackTrace();
                            }
                            catch (const KeypleReaderException &e) {
                                e->printStackTrace();
                            }
                            catch (const KeypleRemoteException &e) {
                                e->printStackTrace();
                            }

                        }
                    }
                }
            }
        }
    }
}
