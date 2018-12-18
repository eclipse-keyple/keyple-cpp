#include "DemoMaster.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/TransportNode.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/TransportFactory.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/ClientNode.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/ServerNode.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/pluginse/VirtualReaderService.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ReaderPlugin.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/PluginEvent.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/pluginse/RemoteSePlugin.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/pluginse/VirtualReader.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeyplePluginNotFoundException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "CommandSample.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace calypso {
                        using RemoteSePlugin = org::eclipse::keyple::plugin::remotese::pluginse::RemoteSePlugin;
                        using VirtualReader = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader;
                        using VirtualReaderService = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReaderService;
                        using ClientNode = org::eclipse::keyple::plugin::remotese::transport::ClientNode;
                        using ServerNode = org::eclipse::keyple::plugin::remotese::transport::ServerNode;
                        using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;
                        using TransportNode = org::eclipse::keyple::plugin::remotese::transport::TransportNode;
                        using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using PluginEvent = org::eclipse::keyple::seproxy::event_Renamed::PluginEvent;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeyplePluginNotFoundException = org::eclipse::keyple::seproxy::exception::KeyplePluginNotFoundException;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using Observable = org::eclipse::keyple::util::Observable;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> DemoMaster::logger = org::slf4j::LoggerFactory::getLogger(DemoMaster::typeid);

                        DemoMaster::DemoMaster(std::shared_ptr<TransportFactory> transportFactory, Boolean isServer) {

                            logger->info("*******************");
                            logger->info("Create DemoMaster  ");
                            logger->info("*******************");

                            if (isServer) {
                                // Master is server, start Server and wait for Slave Clients
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
                                // Master is client, connectAReader to Slave Server
                                node = transportFactory->getClient();
                                (std::static_pointer_cast<ClientNode>(node))->connect();
                            }
                        }

                        DemoMaster::ThreadAnonymousInnerClass::ThreadAnonymousInnerClass(std::shared_ptr<DemoMaster> outerInstance) {
                            this->outerInstance = outerInstance;
                        }

                        void DemoMaster::ThreadAnonymousInnerClass::run() {
                            (std::static_pointer_cast<ServerNode>(outerInstance->node))->start();
                            logger->info("Waits for remote connections");
                        }

                        void DemoMaster::boot() {


                            logger->info("Create VirtualReaderService, start plugin");
                            // Create virtualReaderService with a DtoSender
                            // Dto Sender is required so virtualReaderService can send KeypleDTO to Slave
                            // In this case, node is used as the dtosender (can be client or server)
                            std::shared_ptr<VirtualReaderService> virtualReaderService = std::make_shared<VirtualReaderService>(SeProxyService::getInstance(), node);

                            // observe remote se plugin for events
                            logger->info("Observe SeRemotePlugin for Plugin Events and Reader Events");
                            std::shared_ptr<ReaderPlugin> rsePlugin = virtualReaderService->getPlugin();
                            (std::static_pointer_cast<Observable>(rsePlugin))->addObserver(shared_from_this());

                            // Binds virtualReaderService to a TransportNode so virtualReaderService receives incoming
                            // KeypleDto from Slaves
                            // in this case we binds it to node (can be client or server)
                            virtualReaderService->bindDtoEndpoint(node);


                        }

                        void DemoMaster::update(std::shared_ptr<void> o) {
                            logger->debug("UPDATE {}", o);
                            std::shared_ptr<DemoMaster> * const masterThread = shared_from_this();

                            // Receive a PluginEvent
                            if (std::dynamic_pointer_cast<PluginEvent>(o) != nullptr) {
                                std::shared_ptr<PluginEvent> event_Renamed = std::static_pointer_cast<PluginEvent>(o);
                                switch (event_Renamed->getEventType()) {
                                    case READER_CONNECTED:
                                        // a new virtual reader is connected, let's observe it readers event
                                        logger->info("READER_CONNECTED {} {}", event_Renamed->getPluginName(), event_Renamed->getReaderName());
                                        try {
                                            std::shared_ptr<RemoteSePlugin> remoteSEPlugin = std::static_pointer_cast<RemoteSePlugin>(SeProxyService::getInstance()->getPlugin("RemoteSePlugin"));
                                            std::shared_ptr<VirtualReader> virtualReader = std::static_pointer_cast<VirtualReader>(remoteSEPlugin->getReader(event_Renamed->getReaderName()));

                                            // should parameter reader, addSeProtocolSetting, defaultCommand

                                            // observe reader events
                                            logger->info("Add ServerTicketingApp as a Observer of RSE reader");
                                            virtualReader->addObserver(masterThread);

                                        }
                                        catch (const KeypleReaderNotFoundException &e) {
                                            logger->error(e->what());
                                            e->printStackTrace();
                                        }
                                        catch (const KeyplePluginNotFoundException &e) {
                                            logger->error(e->what());
                                            e->printStackTrace();
                                        }


                                        break;
                                    case READER_DISCONNECTED:
                                        logger->info("READER_DISCONNECTED {} {}", event_Renamed->getPluginName(), event_Renamed->getReaderName());
                                        break;
                                }
                            }
                            // ReaderEvent
                            else if (std::dynamic_pointer_cast<ReaderEvent>(o) != nullptr) {
                                std::shared_ptr<ReaderEvent> event_Renamed = std::static_pointer_cast<ReaderEvent>(o);
                                switch (event_Renamed->getEventType()) {
                                    case SE_INSERTED:
                                        logger->info("SE_INSERTED {} {}", event_Renamed->getPluginName(), event_Renamed->getReaderName());

                                        // Transmit a SeRequestSet to native reader
                                        CommandSample::transmit(logger, event_Renamed->getReaderName());

                                        break;
                                    case SE_REMOVAL:
                                        logger->info("SE_REMOVAL {} {}", event_Renamed->getPluginName(), event_Renamed->getReaderName());
                                        break;
                                    case IO_ERROR:
                                        logger->info("IO_ERROR {} {}", event_Renamed->getPluginName(), event_Renamed->getReaderName());
                                        break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
