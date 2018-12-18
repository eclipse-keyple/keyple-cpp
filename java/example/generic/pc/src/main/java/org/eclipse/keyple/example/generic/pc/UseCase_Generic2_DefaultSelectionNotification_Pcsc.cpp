#include "UseCase_Generic2_DefaultSelectionNotification_Pcsc.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelection.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.h"
#include "ReaderUtilities.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelector.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/MatchingSe.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic_Renamed {
                    namespace pc {
                        using PcscPlugin = org::eclipse::keyple::plugin::pcsc::PcscPlugin;
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using ReaderObserver = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader::ReaderObserver;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using MatchingSe = org::eclipse::keyple::transaction::MatchingSe;
                        using SeSelection = org::eclipse::keyple::transaction::SeSelection;
                        using SeSelector = org::eclipse::keyple::transaction::SeSelector;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> UseCase_Generic2_DefaultSelectionNotification_Pcsc::logger = org::slf4j::LoggerFactory::getLogger(UseCase_Generic2_DefaultSelectionNotification_Pcsc::typeid);
const std::shared_ptr<void> UseCase_Generic2_DefaultSelectionNotification_Pcsc::waitForEnd = nullptr;

                        UseCase_Generic2_DefaultSelectionNotification_Pcsc::UseCase_Generic2_DefaultSelectionNotification_Pcsc() throw(KeypleBaseException, InterruptedException) {
                            /* Get the instance of the SeProxyService (Singleton pattern) */
                            std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();

                            /* Get the instance of the PC/SC plugin */
                            std::shared_ptr<PcscPlugin> pcscPlugin = PcscPlugin::getInstance();

                            /* Assign PcscPlugin to the SeProxyService */
                            seProxyService->addPlugin(pcscPlugin);

                            /*
                             * Get a SE reader ready to work with contactless SE. Use the getReader helper method from
                             * the ReaderUtilities class.
                             */
                            seReader = ReaderUtilities::getDefaultContactLessSeReader(seProxyService);

                            /* Check if the reader exists */
                            if (seReader == nullptr) {
                                throw std::make_shared<IllegalStateException>("Bad SE reader setup");
                            }

                            logger->info("=============== UseCase Generic #2: AID based default selection ===================");
                            logger->info("= SE Reader  NAME = {}", seReader->getName());

                            /*
                             * Prepare a SE selection
                             */
                            seSelection = std::make_shared<SeSelection>(seReader);

                            /*
                             * Setting of an AID based selection
                             *
                             * Select the first application matching the selection AID whatever the SE communication
                             * protocol keep the logical channel open after the selection
                             */

                            /*
                             * Generic selection: configures a SeSelector with all the desired attributes to make the
                             * selection
                             */
                            std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(ByteArrayUtils::fromHex(seAid), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4, "AID: " + seAid);

                            /*
                             * Add the selection case to the current selection (we could have added other cases here)
                             */
                            seSelection->prepareSelection(seSelector);

                            /*
                             * Provide the SeReader with the selection operation to be processed when a SE is inserted.
                             */
                            (std::static_pointer_cast<ObservableReader>(seReader))->setDefaultSelectionRequest(seSelection->getSelectionOperation(), ObservableReader::NotificationMode::MATCHED_ONLY);

                            /* Set the current class as Observer of the first reader */
//JAVA TO C++ CONVERTER TODO TASK: You cannot use 'shared_from_this' in a constructor:
                            (std::static_pointer_cast<ObservableReader>(seReader))->addObserver(shared_from_this());

                            logger->info("==================================================================================");
                            logger->info("= Wait for a SE. The default AID based selection to be processed as soon as the  =");
                            logger->info("= SE is detected.                                                                =");
                            logger->info("==================================================================================");

                            /* Wait for ever (exit with CTRL-C) */
//JAVA TO C++ CONVERTER TODO TASK: Multithread locking is not converted to native C++ unless you choose one of the options on the 'Modern C++ Options' dialog:
                            synchronized(waitForEnd) {
                                waitForEnd->wait();
                            }
                        }

                        void UseCase_Generic2_DefaultSelectionNotification_Pcsc::update(std::shared_ptr<ReaderEvent> event_Renamed) {
                            switch (event_Renamed->getEventType()) {
                                case SE_MATCHED:
                                    if (seSelection->processDefaultSelection(event_Renamed->getDefaultSelectionResponse())) {
                                        std::shared_ptr<MatchingSe> selectedSe = seSelection->getSelectedSe();

                                        logger->info("Observer notification: the selection of the SE has succeeded.");

                                        logger->info("==================================================================================");
                                        logger->info("= End of the SE processing.                                                      =");
                                        logger->info("==================================================================================");
                                    }
                                    else {
                                        logger->error("The selection of the SE has failed. Should not have occurred due to the MATCHED_ONLY selection mode.");
                                    }
                                    break;
                                case SE_INSERTED:
                                    logger->error("SE_INSERTED event: should not have occurred due to the MATCHED_ONLY selection mode.");
                                    break;
                                case SE_REMOVAL:
                                    logger->info("The SE has been removed.");
                                    break;
                                default:
                                    break;
                            }
                        }

                        void UseCase_Generic2_DefaultSelectionNotification_Pcsc::main(std::vector<std::string> &args) throw(InterruptedException, KeypleBaseException) {
                            /* Create the observable object to handle the SE processing */
                            std::shared_ptr<UseCase_Generic2_DefaultSelectionNotification_Pcsc> m = std::make_shared<UseCase_Generic2_DefaultSelectionNotification_Pcsc>();
                        }
                    }
                }
            }
        }
    }
}
