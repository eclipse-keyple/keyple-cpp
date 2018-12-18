#include "UseCase_Calypso3_Rev1Selection_Pcsc.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/NoStackTraceThrowable.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "../../../../../../../../../../common/src/main/java/org/eclipse/keyple/example/calypso/common/transaction/CalypsoUtilities.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelection.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoSelector.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/SelectFileRespPars.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadDataStructure.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadRecordsRespPars.h"
#include "../../../../../../../../../../common/src/main/java/org/eclipse/keyple/example/calypso/common/postructure/CalypsoClassicInfo.h"
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
                        using SelectFileRespPars = org::eclipse::keyple::calypso::command::po::parser::SelectFileRespPars;
                        using CalypsoPo = org::eclipse::keyple::calypso::transaction::CalypsoPo;
                        using PoSelector = org::eclipse::keyple::calypso::transaction::PoSelector;
                        using PoTransaction = org::eclipse::keyple::calypso::transaction::PoTransaction;
                        using CalypsoClassicInfo = org::eclipse::keyple::example::calypso::common::postructure::CalypsoClassicInfo;
                        using CalypsoUtilities = org::eclipse::keyple::example::calypso::common::transaction::CalypsoUtilities;
                        using PcscPlugin = org::eclipse::keyple::plugin::pcsc::PcscPlugin;
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using SeSelection = org::eclipse::keyple::transaction::SeSelection;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> UseCase_Calypso3_Rev1Selection_Pcsc::logger = org::slf4j::LoggerFactory::getLogger(UseCase_Calypso3_Rev1Selection_Pcsc::typeid);
std::string UseCase_Calypso3_Rev1Selection_Pcsc::poAtrRegex = ".*";
std::string UseCase_Calypso3_Rev1Selection_Pcsc::poDfRtPath = "2000";

                        void UseCase_Calypso3_Rev1Selection_Pcsc::main(std::vector<std::string> &args) throw(KeypleBaseException, InterruptedException, IOException, NoStackTraceThrowable) {

                            /* Get the instance of the SeProxyService (Singleton pattern) */
                            std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();

                            /* Get the instance of the PC/SC plugin */
                            std::shared_ptr<PcscPlugin> pcscPlugin = PcscPlugin::getInstance();

                            /* Assign PcscPlugin to the SeProxyService */
                            seProxyService->addPlugin(pcscPlugin);

                            /*
                             * Get a PO reader ready to work with Calypso PO. Use the getReader helper method from the
                             * CalypsoUtilities class.
                             */
                            std::shared_ptr<SeReader> poReader = CalypsoUtilities::getDefaultPoReader(seProxyService);

                            /* Check if the reader exists */
                            if (poReader == nullptr) {
                                throw std::make_shared<IllegalStateException>("Bad PO reader setup");
                            }

                            logger->info("=============== UseCase Calypso #1: ATR based explicit selection (PO Rev1) ===========");
                            logger->info("= PO Reader  NAME = {}", poReader->getName());

                            /* Check if a PO is present in the reader */
                            if (poReader->isSePresent()) {

                                logger->info("==================================================================================");
                                logger->info("= 1st PO exchange: ATR based selection with reading of Environment file.         =");
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
                                std::shared_ptr<PoSelector> poSelector = std::make_shared<PoSelector>(poAtrRegex, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4, "ATR: " + poAtrRegex);

                                /*
                                 * Prepare the selection of the DF RT.
                                 */
                                std::shared_ptr<SelectFileRespPars> selectFileRespPars = poSelector->prepareSelectFileDfCmd(ByteArrayUtils::fromHex(poDfRtPath), "Select file: " + poDfRtPath);

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

                                    logger->info("DF RT FCI: {}", ByteArrayUtils::toHex(selectFileRespPars->getSelectionData()));

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
                            exit(0);
                        }
                    }
                }
            }
        }
    }
}
