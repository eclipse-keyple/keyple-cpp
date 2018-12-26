#include "UseCase_Calypso4_PoAuthentication_Pcsc.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/NoStackTraceThrowable.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "../../../../../../../../../../common/src/main/java/org/eclipse/keyple/example/calypso/common/transaction/CalypsoUtilities.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoSelector.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelector.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../common/src/main/java/org/eclipse/keyple/example/calypso/common/postructure/CalypsoClassicInfo.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/CalypsoPo.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoTransaction.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadDataStructure.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadRecordsRespPars.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/TransmissionMode.h"

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
                        using CalypsoUtilities = org::eclipse::keyple::example::calypso::common::transaction::CalypsoUtilities;
                        using PcscPlugin = org::eclipse::keyple::plugin::pcsc::PcscPlugin;
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                        using namespace org::eclipse::keyple::transaction;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> UseCase_Calypso4_PoAuthentication_Pcsc::logger = org::slf4j::LoggerFactory::getLogger(UseCase_Calypso4_PoAuthentication_Pcsc::typeid);

                        void UseCase_Calypso4_PoAuthentication_Pcsc::main(std::vector<std::string> &args) throw(KeypleBaseException, NoStackTraceThrowable) {

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


                            /*
                             * Get a SAM reader ready to work with Calypso PO. Use the getReader helper method from the
                             * CalypsoUtilities class.
                             */
                            std::shared_ptr<SeReader> samReader = CalypsoUtilities::getDefaultSamReader(seProxyService);

                            /* Check if the readers exists */
                            if (poReader == nullptr || samReader == nullptr) {
                                throw std::make_shared<IllegalStateException>("Bad PO or SAM reader setup");
                            }

                            logger->info("=============== UseCase Calypso #4: Po Authentication ==================");
                            logger->info("= PO Reader  NAME = {}", poReader->getName());
                            logger->info("= SAM Reader  NAME = {}", samReader->getName());

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

                                    std::shared_ptr<MatchingSe> selectedSe = seSelection->getSelectedSe();

                                    /* Go on with the reading of the first record of the EventLog file */
                                    logger->info("==================================================================================");
                                    logger->info("= 2nd PO exchange: open and close a secure session to perform authentication.    =");
                                    logger->info("==================================================================================");

                                    std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(poReader, std::static_pointer_cast<CalypsoPo>(selectedSe), samReader, CalypsoUtilities::getSamSettings());

                                    /*
                                     * Prepare the reading order and keep the associated parser for later use once the
                                     * transaction has been processed.
                                     */
                                    std::shared_ptr<ReadRecordsRespPars> readEventLogParser = poTransaction->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_EventLog, ReadDataStructure::SINGLE_RECORD_DATA, CalypsoClassicInfo::RECORD_NUMBER_1, std::string::format("EventLog (SFI=%02X, recnbr=%d))", CalypsoClassicInfo::SFI_EventLog, CalypsoClassicInfo::RECORD_NUMBER_1));


                                    /*
                                     * Open Session for the debit key
                                     */
                                    bool poProcessStatus = poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT, static_cast<char>(0), static_cast<char>(0));

                                    if (!poProcessStatus) {
                                        throw std::make_shared<IllegalStateException>("processingOpening failure.");
                                    }

                                    if (!poTransaction->wasRatified()) {
                                        logger->info("========= Previous Secure Session was not ratified. =====================");
                                    }
                                    /*
                                     * Prepare the reading order and keep the associated parser for later use once the
                                     * transaction has been processed.
                                     */
                                    std::shared_ptr<ReadRecordsRespPars> readEventLogParserBis = poTransaction->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_EventLog, ReadDataStructure::SINGLE_RECORD_DATA, CalypsoClassicInfo::RECORD_NUMBER_1, std::string::format("EventLog (SFI=%02X, recnbr=%d))", CalypsoClassicInfo::SFI_EventLog, CalypsoClassicInfo::RECORD_NUMBER_1));

                                    poProcessStatus = poTransaction->processPoCommands(ChannelState::KEEP_OPEN);

                                    /*
                                     * Retrieve the data read from the parser updated during the transaction process
                                     */
                                    std::vector<char> eventLog = (readEventLogParser->getRecords())->get(static_cast<int>(CalypsoClassicInfo::RECORD_NUMBER_1));

                                    /* Log the result */
                                    logger->info("EventLog file data: {}", ByteArrayUtils::toHex(eventLog));

                                    if (!poProcessStatus) {
                                        throw std::make_shared<IllegalStateException>("processPoCommands failure.");
                                    }

                                    /*
                                     * Close the Secure Session.
                                     */
                                    if (logger->isInfoEnabled()) {
                                        logger->info("========= PO Calypso session ======= Closing ============================");
                                    }

                                    /*
                                     * A ratification command will be sent (CONTACTLESS_MODE).
                                     */
                                    poProcessStatus = poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::CLOSE_AFTER);

                                    if (!poProcessStatus) {
                                        throw std::make_shared<IllegalStateException>("processClosing failure.");
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
