#include "DemoSlave.h"
#include "../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubReader.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/nativese/NativeReaderServiceImpl.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubPlugin.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/PluginEvent.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocolSetting.h"
#include "../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubProtocolSetting.h"
#include "../../../../../../../../../../pc/src/main/java/org/eclipse/keyple/example/calypso/pc/stub/se/StubHoplink.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace calypso {
                        using StubHoplink = org::eclipse::keyple::example::calypso::pc::stub::se::StubHoplink;
                        using NativeReaderServiceImpl = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderServiceImpl;
                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using StubPlugin = org::eclipse::keyple::plugin::stub::StubPlugin;
                        using StubProtocolSetting = org::eclipse::keyple::plugin::stub::StubProtocolSetting;
                        using StubReader = org::eclipse::keyple::plugin::stub::StubReader;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using ObservablePlugin = org::eclipse::keyple::seproxy::event_Renamed::ObservablePlugin;
                        using PluginEvent = org::eclipse::keyple::seproxy::event_Renamed::PluginEvent;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> DemoSlave::logger = org::slf4j::LoggerFactory::getLogger(DemoSlave::typeid);

                        DemoSlave::DemoSlave(std::shared_ptr<TransportFactory> transportFactory, Boolean isServer) {
                            logger->info("*******************");
                            logger->info("Create DemoSlave    ");
                            logger->info("*******************");

                            if (isServer) {
                                // Slave is server, start Server and wait for Master clients
                                try {
                                    node = transportFactory->getServer();
                                    // start server in a new thread
//JAVA TO C++ CONVERTER TODO TASK: You cannot use 'shared_from_this' in a constructor:
                                    std::make_shared<ThreadAnonymousInnerClass>(shared_from_this())
                                    .start();

                                }
                                catch (const IOException &e) {
                                    e->printStackTrace();
                                }
                            }
                            else {
                                // Slave is client, connectAReader to Master Server
                                node = transportFactory->getClient();
                                (std::static_pointer_cast<ClientNode>(node))->connect();
                            }
                        }

                        DemoSlave::ThreadAnonymousInnerClass::ThreadAnonymousInnerClass(std::shared_ptr<DemoSlave> outerInstance) {
                            this->outerInstance = outerInstance;
                        }

                        void DemoSlave::ThreadAnonymousInnerClass::run() {
                            (std::static_pointer_cast<ServerNode>(outerInstance->node))->start();
                            logger->info("Waits for remote connections");
                        }

                        void DemoSlave::connectAReader() throw(KeypleReaderException, InterruptedException, KeypleRemoteException) {


                            logger->info("Boot DemoSlave LocalReader ");

                            // get seProxyService
                            std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();

                            logger->info("Create Local StubPlugin");
                            std::shared_ptr<StubPlugin> stubPlugin = StubPlugin::getInstance();

                            SeProxyService::getInstance()->addPlugin(stubPlugin);

                            std::shared_ptr<ObservablePlugin::PluginObserver> observer = std::make_shared<PluginObserverAnonymousInnerClass>(shared_from_this());

                            // add observer to have the reader management done by the monitoring thread
                            stubPlugin->addObserver(observer);

                            delay(100);

                            stubPlugin->plugStubReader("stubClientSlave");

                            delay(1000);

                            // get the created proxy reader
                            localReader = std::static_pointer_cast<StubReader>(stubPlugin->getReader("stubClientSlave"));

                            localReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4)); // should
                                                                                                             // be in
                                                                                                             // master


                            // Binds node for outgoing KeypleDto
                            nativeReaderService = std::make_shared<NativeReaderServiceImpl>(node);

                            // Binds node for incoming KeypleDTo
                            nativeReaderService->bindDtoEndpoint(node);

                            // connect a reader to Remote Plugin
                            logger->info("Connect remotely the StubPlugin ");
                            nativeReaderService->connectReader(localReader, nodeId);

                        }

                        DemoSlave::PluginObserverAnonymousInnerClass::PluginObserverAnonymousInnerClass(std::shared_ptr<DemoSlave> outerInstance) {
                            this->outerInstance = outerInstance;
                        }

                        void DemoSlave::PluginObserverAnonymousInnerClass::update(std::shared_ptr<PluginEvent> event_Renamed) {
                            logger->info("Update - pluginEvent from inline observer", event_Renamed);
                        }

                        void DemoSlave::insertSe() {
                            logger->info("************************");
                            logger->info("Start DEMO - insert SE  ");
                            logger->info("************************");

                            logger->info("Insert HoplinkStubSE into Local StubReader");
                            // insert SE
                            localReader->insertSe(std::make_shared<StubHoplink>());


                        }

                        void DemoSlave::removeSe() {

                            logger->info("************************");
                            logger->info(" remove SE ");
                            logger->info("************************");

                            localReader->removeSe();

                        }

                        void DemoSlave::disconnect() throw(KeypleReaderException, KeypleRemoteException) {

                            logger->info("*************************");
                            logger->info("Disconnect native reader ");
                            logger->info("*************************");

                            nativeReaderService->disconnectReader(localReader, nodeId);
                        }
                    }
                }
            }
        }
    }
}
