#include "DemoMaster.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelection.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/pluginse/VirtualReader.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadRecordsRespPars.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/TransportNode.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/TransportFactory.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/ClientNode.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/transport/ServerNode.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/pluginse/VirtualReaderService.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ReaderPlugin.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/PluginEvent.h"
#include "../../../../../../../../../../../../component/keyple-plugin/remotese/src/main/java/org/eclipse/keyple/plugin/remotese/pluginse/RemoteSePlugin.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoSelector.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelector.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../common/src/main/java/org/eclipse/keyple/example/calypso/common/postructure/CalypsoClassicInfo.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadDataStructure.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeyplePluginNotFoundException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/MatchingSe.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/CalypsoPo.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoTransaction.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace remote {
                    namespace calypso {
                        using ReadDataStructure = org::eclipse::keyple::calypso::command::po::parser::ReadDataStructure;
                        using ReadRecordsRespPars = org::eclipse::keyple::calypso::command::po::parser::ReadRecordsRespPars;
                        using CalypsoPo = org::eclipse::keyple::calypso::transaction::CalypsoPo;
                        using PoSelector = org::eclipse::keyple::calypso::transaction::PoSelector;
                        using PoTransaction = org::eclipse::keyple::calypso::transaction::PoTransaction;
                        using CalypsoClassicInfo = org::eclipse::keyple::example::calypso::common::postructure::CalypsoClassicInfo;
                        using RemoteSePlugin = org::eclipse::keyple::plugin::remotese::pluginse::RemoteSePlugin;
                        using VirtualReader = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader;
                        using VirtualReaderService = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReaderService;
                        using ClientNode = org::eclipse::keyple::plugin::remotese::transport::ClientNode;
                        using ServerNode = org::eclipse::keyple::plugin::remotese::transport::ServerNode;
                        using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;
                        using TransportNode = org::eclipse::keyple::plugin::remotese::transport::TransportNode;
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using PluginEvent = org::eclipse::keyple::seproxy::event_Renamed::PluginEvent;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeyplePluginNotFoundException = org::eclipse::keyple::seproxy::exception::KeyplePluginNotFoundException;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using MatchingSe = org::eclipse::keyple::transaction::MatchingSe;
                        using SeSelection = org::eclipse::keyple::transaction::SeSelection;
                        using SeSelector = org::eclipse::keyple::transaction::SeSelector;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
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
                            if (std::static_pointer_cast<PluginEvent>(o) != nullptr) {
                                std::shared_ptr<PluginEvent> event_Renamed = std::static_pointer_cast<PluginEvent>(o);
                                switch (event_Renamed->getEventType()) {
                                    case READER_CONNECTED:
                                        // a new virtual reader is connected, let's observe it readers event
                                        logger->info("READER_CONNECTED {} {}", event_Renamed->getPluginName(), event_Renamed->getReaderName());
                                        try {
                                            std::shared_ptr<RemoteSePlugin> remoteSEPlugin = std::static_pointer_cast<RemoteSePlugin>(SeProxyService::getInstance()->getPlugin("RemoteSePlugin"));
                                            poReader = std::static_pointer_cast<VirtualReader>(remoteSEPlugin->getReader(event_Renamed->getReaderName()));

                                            /* set default selection request */
                                            seSelection = std::make_shared<SeSelection>(poReader);

                                            /*
                                             * Setting of an AID based selection of a Calypso REV3 PO
                                             *
                                             * Select the first application matching the selection AID whatever the SE
                                             * communication protocol keep the logical channel open after the selection
                                             */

                                            /*
                                             * Calypso selection: configures a PoSelector with all the desired
                                             * attributes to make the selection and read additional information
                                             * afterwards
                                             */
                                            std::shared_ptr<PoSelector> poSelector = std::make_shared<PoSelector>(ByteArrayUtils::fromHex(CalypsoClassicInfo::AID), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4, "AID: " + CalypsoClassicInfo::AID);

                                            /*
                                             * Prepare the reading order and keep the associated parser for later use
                                             * once the selection has been made.
                                             */
                                            readEnvironmentParser = poSelector->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_EnvironmentAndHolder, ReadDataStructure::SINGLE_RECORD_DATA, CalypsoClassicInfo::RECORD_NUMBER_1, std::string::format("EnvironmentAndHolder (SFI=%02X))", CalypsoClassicInfo::SFI_EnvironmentAndHolder));

                                            /*
                                             * Add the selection case to the current selection (we could have added
                                             * other cases here)
                                             */
                                            seSelection->prepareSelection(poSelector);

                                            /*
                                             * Provide the SeReader with the selection operation to be processed when a
                                             * PO is inserted.
                                             */
                                            (std::static_pointer_cast<ObservableReader>(poReader))->setDefaultSelectionRequest(seSelection->getSelectionOperation(), ObservableReader::NotificationMode::MATCHED_ONLY);


                                            // observe reader events
                                            logger->info("Add ServerTicketingApp as a Observer of RSE reader");
                                            poReader->addObserver(masterThread);

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
                            else if (std::static_pointer_cast<ReaderEvent>(o) != nullptr) {
                                std::shared_ptr<ReaderEvent> event_Renamed = std::static_pointer_cast<ReaderEvent>(o);
                                switch (event_Renamed->getEventType()) {
                                    case SE_MATCHED:
                                        if (seSelection->processDefaultSelection(event_Renamed->getDefaultSelectionResponse())) {
                                            std::shared_ptr<MatchingSe> selectedSe = seSelection->getSelectedSe();

                                            logger->info("Observer notification: the selection of the PO has succeeded.");

                                            /*
                                             * Retrieve the data read from the parser updated during the selection
                                             * process
                                             */
                                            std::vector<char> environmentAndHolder = (readEnvironmentParser->getRecords())->get(static_cast<int>(CalypsoClassicInfo::RECORD_NUMBER_1));

                                            /* Log the result */
                                            logger->info("Environment file data: {}", ByteArrayUtils::toHex(environmentAndHolder));

                                            /* Go on with the reading of the first record of the EventLog file */
                                            logger->info("==================================================================================");
                                            logger->info("= 2nd PO exchange: reading transaction of the EventLog file.                     =");
                                            logger->info("==================================================================================");

                                            std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(poReader, std::static_pointer_cast<CalypsoPo>(selectedSe));

                                            /*
                                             * Prepare the reading order and keep the associated parser for later use
                                             * once the transaction has been processed.
                                             */
                                            std::shared_ptr<ReadRecordsRespPars> readEventLogParser = poTransaction->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_EventLog, ReadDataStructure::SINGLE_RECORD_DATA, CalypsoClassicInfo::RECORD_NUMBER_1, std::string::format("EventLog (SFI=%02X, recnbr=%d))", CalypsoClassicInfo::SFI_EventLog, CalypsoClassicInfo::RECORD_NUMBER_1));

                                            /*
                                             * Actual PO communication: send the prepared read order, then close the
                                             * channel with the PO
                                             */
                                            try {
                                                if (poTransaction->processPoCommands(ChannelState::CLOSE_AFTER)) {
                                                    logger->info("The reading of the EventLog has succeeded.");

                                                    /*
                                                     * Retrieve the data read from the parser updated during the
                                                     * transaction process
                                                     */
                                                    std::vector<char> eventLog = (readEventLogParser->getRecords())->get(static_cast<int>(CalypsoClassicInfo::RECORD_NUMBER_1));

                                                    /* Log the result */
                                                    logger->info("EventLog file data: {}", ByteArrayUtils::toHex(eventLog));
                                                }
                                            }
                                            catch (const KeypleReaderException &e) {
                                                e->printStackTrace();
                                            }
                                            logger->info("==================================================================================");
                                            logger->info("= End of the Calypso PO processing.                                              =");
                                            logger->info("==================================================================================");
                                        }
                                        else {
                                            logger->error("The selection of the PO has failed. Should not have occurred due to the MATCHED_ONLY selection mode.");
                                        }
                                        break;
                                    case SE_INSERTED:
                                        logger->info("SE_INSERTED {} {}", event_Renamed->getPluginName(), event_Renamed->getReaderName());

                                        // Transmit a SeRequestSet to native reader
                                        // CommandSample.transmit(logger, event.getReaderName());

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
