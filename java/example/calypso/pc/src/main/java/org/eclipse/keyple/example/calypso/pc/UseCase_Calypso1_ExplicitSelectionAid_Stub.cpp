#include "UseCase_Calypso1_ExplicitSelectionAid_Stub.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/NoStackTraceThrowable.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubPlugin.h"
#include "../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubReader.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocolSetting.h"
#include "../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubProtocolSetting.h"
#include "../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubSecureElement.h"
#include "stub/se/StubCalypsoClassic.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelection.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoSelector.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelector.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../common/src/main/java/org/eclipse/keyple/example/calypso/common/postructure/CalypsoClassicInfo.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadDataStructure.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadRecordsRespPars.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/CalypsoPo.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoTransaction.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace calypso {
                    namespace pc {
                        using ReadDataStructure = org::eclipse::keyple::calypso::command::po::parser::ReadDataStructure;
                        using ReadRecordsRespPars = org::eclipse::keyple::calypso::command::po::parser::ReadRecordsRespPars;
                        using CalypsoPo = org::eclipse::keyple::calypso::transaction::CalypsoPo;
                        using PoSelector = org::eclipse::keyple::calypso::transaction::PoSelector;
                        using PoTransaction = org::eclipse::keyple::calypso::transaction::PoTransaction;
                        using CalypsoClassicInfo = org::eclipse::keyple::example::calypso::common::postructure::CalypsoClassicInfo;
                        using StubCalypsoClassic = org::eclipse::keyple::example::calypso::pc::stub::se::StubCalypsoClassic;
                        using StubPlugin = org::eclipse::keyple::plugin::stub::StubPlugin;
                        using StubProtocolSetting = org::eclipse::keyple::plugin::stub::StubProtocolSetting;
                        using StubReader = org::eclipse::keyple::plugin::stub::StubReader;
                        using StubSecureElement = org::eclipse::keyple::plugin::stub::StubSecureElement;
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                        using SeSelection = org::eclipse::keyple::transaction::SeSelection;
                        using SeSelector = org::eclipse::keyple::transaction::SeSelector;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> UseCase_Calypso1_ExplicitSelectionAid_Stub::logger = org::slf4j::LoggerFactory::getLogger(UseCase_Calypso1_ExplicitSelectionAid_Stub::typeid);

                        void UseCase_Calypso1_ExplicitSelectionAid_Stub::main(std::vector<std::string> &args) throw(KeypleBaseException, InterruptedException, IOException, NoStackTraceThrowable) {

                            /* Get the instance of the SeProxyService (Singleton pattern) */
                            std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();

                            /* Get the instance of the Stub plugin */
                            std::shared_ptr<StubPlugin> stubPlugin = StubPlugin::getInstance();

                            /* Assign StubPlugin to the SeProxyService */
                            seProxyService->addPlugin(stubPlugin);

                            /* Plug the PO stub reader. */
                            stubPlugin->plugStubReader("poReader");

                            /*
                             * Get a PO reader ready to work with Calypso PO.
                             */
                            std::shared_ptr<StubReader> poReader = std::static_pointer_cast<StubReader>(stubPlugin->getReader("poReader"));

                            /* Check if the reader exists */
                            if (poReader == nullptr) {
                                throw std::make_shared<IllegalStateException>("Bad PO reader setup");
                            }

                            poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4));

                            /* Create 'virtual' Calypso PO */
                            std::shared_ptr<StubSecureElement> calypsoStubSe = std::make_shared<StubCalypsoClassic>();

                            logger->info("Insert stub PO.");
                            poReader->insertSe(calypsoStubSe);

                            logger->info("=============== UseCase Calypso #1: AID based explicit selection ==================");
                            logger->info("= PO Reader  NAME = {}", poReader->getName());

                            /* Check if a PO is present in the reader */
                            if (poReader->isSePresent()) {

                                logger->info("==================================================================================");
                                logger->info("= 1st PO exchange: AID based selection with reading of Environment file.         =");
                                logger->info("==================================================================================");

                                /*
                                 * Prepare a Calypso PO selection
                                 */
                                std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>(poReader);

                                /*
                                 * Setting of an AID based selection of a Calypso REV3 PO
                                 *
                                 * Select the first application matching the selection AID whatever the SE communication
                                 * protocol keep the logical channel open after the selection
                                 */

                                /*
                                 * Calypso selection: configures a PoSelector with all the desired attributes to make
                                 * the selection and read additional information afterwards
                                 */
                                std::shared_ptr<PoSelector> poSelector = std::make_shared<PoSelector>(ByteArrayUtils::fromHex(CalypsoClassicInfo::AID), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4, "AID: " + CalypsoClassicInfo::AID);

                                /*
                                 * Prepare the reading order and keep the associated parser for later use once the
                                 * selection has been made.
                                 */
                                std::shared_ptr<ReadRecordsRespPars> readEnvironmentParser = poSelector->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_EnvironmentAndHolder, ReadDataStructure::SINGLE_RECORD_DATA, CalypsoClassicInfo::RECORD_NUMBER_1, std::string::format("EnvironmentAndHolder (SFI=%02X))", CalypsoClassicInfo::SFI_EnvironmentAndHolder));

                                /*
                                 * Add the selection case to the current selection (we could have added other cases
                                 * here)
                                 */
                                std::shared_ptr<CalypsoPo> calypsoPo = std::static_pointer_cast<CalypsoPo>(seSelection->prepareSelection(poSelector));

                                /*
                                 * Actual PO communication: operate through a single request the Calypso PO selection
                                 * and the file read
                                 */
                                if (seSelection->processExplicitSelection()) {
                                    logger->info("The selection of the PO has succeeded.");

                                    /* Retrieve the data read from the parser updated during the selection process */
                                    std::vector<char> environmentAndHolder = (readEnvironmentParser->getRecords())->get(static_cast<int>(CalypsoClassicInfo::RECORD_NUMBER_1));

                                    /* Log the result */
                                    logger->info("Environment file data: {}", ByteArrayUtils::toHex(environmentAndHolder));

                                    /* Go on with the reading of the first record of the EventLog file */
                                    logger->info("==================================================================================");
                                    logger->info("= 2nd PO exchange: reading transaction of the EventLog file.                     =");
                                    logger->info("==================================================================================");

                                    std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(poReader, calypsoPo);

                                    /*
                                     * Prepare the reading order and keep the associated parser for later use once the
                                     * transaction has been processed.
                                     */
                                    std::shared_ptr<ReadRecordsRespPars> readEventLogParser = poTransaction->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_EventLog, ReadDataStructure::SINGLE_RECORD_DATA, CalypsoClassicInfo::RECORD_NUMBER_1, std::string::format("EventLog (SFI=%02X, recnbr=%d))", CalypsoClassicInfo::SFI_EventLog, CalypsoClassicInfo::RECORD_NUMBER_1));

                                    /*
                                     * Actual PO communication: send the prepared read order, then close the channel
                                     * with the PO
                                     */
                                    if (poTransaction->processPoCommands(ChannelState::CLOSE_AFTER)) {
                                        logger->info("The reading of the EventLog has succeeded.");

                                        /*
                                         * Retrieve the data read from the parser updated during the transaction process
                                         */
                                        std::vector<char> eventLog = (readEventLogParser->getRecords())->get(static_cast<int>(CalypsoClassicInfo::RECORD_NUMBER_1));

                                        /* Log the result */
                                        logger->info("EventLog file data: {}", ByteArrayUtils::toHex(eventLog));
                                    }
                                    logger->info("==================================================================================");
                                    logger->info("= End of the Calypso PO processing.                                              =");
                                    logger->info("==================================================================================");
                                }
                                else {
                                    logger->error("The selection of the PO has failed.");
                                }
                            }
                            else {
                                logger->error("No PO were detected.");
                            }

                            logger->info("Remove stub PO.");
                            poReader->removeSe();

                            exit(0);
                        }
                    }
                }
            }
        }
    }
}
