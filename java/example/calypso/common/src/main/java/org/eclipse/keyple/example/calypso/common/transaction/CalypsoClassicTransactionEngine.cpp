#include "CalypsoClassicTransactionEngine.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadDataStructure.h"
#include "../../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadRecordsRespPars.h"
#include "../postructure/CalypsoClassicInfo.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/TransmissionMode.h"
#include "../../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/AppendRecordRespPars.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoSelector.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelector.h"
#include "CalypsoUtilities.h"
#include "../../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/CalypsoPo.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace calypso {
                    namespace common {
                        namespace transaction {
                            using AppendRecordRespPars = org::eclipse::keyple::calypso::command::po::parser::AppendRecordRespPars;
                            using ReadDataStructure = org::eclipse::keyple::calypso::command::po::parser::ReadDataStructure;
                            using ReadRecordsRespPars = org::eclipse::keyple::calypso::command::po::parser::ReadRecordsRespPars;
                            using CalypsoPo = org::eclipse::keyple::calypso::transaction::CalypsoPo;
                            using PoSelector = org::eclipse::keyple::calypso::transaction::PoSelector;
                            using PoTransaction = org::eclipse::keyple::calypso::transaction::PoTransaction;
                            using CalypsoClassicInfo = org::eclipse::keyple::example::calypso::common::postructure::CalypsoClassicInfo;
                            using AbstractReaderObserverEngine = org::eclipse::keyple::example::generic_Renamed::common::AbstractReaderObserverEngine;
                            using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                            using SeReader = org::eclipse::keyple::seproxy::SeReader;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                            using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                            using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                            using namespace org::eclipse::keyple::transaction;
                            using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
                            using org::slf4j::profiler::Profiler;
std::shared_ptr<org::slf4j::Logger> CalypsoClassicTransactionEngine::logger = org::slf4j::LoggerFactory::getLogger(CalypsoClassicTransactionEngine::typeid);
const std::shared_ptr<java::util::EnumMap<org::eclipse::keyple::calypso::transaction::PoTransaction::SamSettings, Byte>> CalypsoClassicTransactionEngine::samSetting = std::make_shared<EnumMapAnonymousInnerClass>();

                            CalypsoClassicTransactionEngine::EnumMapAnonymousInnerClass::EnumMapAnonymousInnerClass() : java::util::EnumMap<org::eclipse::keyple::calypso::transaction::PoTransaction::SamSettings, Byte>(org::eclipse::keyple::calypso::transaction::PoTransaction::SamSettings::class) {
                            }

                            CalypsoClassicTransactionEngine::CalypsoClassicTransactionEngine() : org::eclipse::keyple::example::generic_Renamed::common::AbstractReaderObserverEngine() {
                                this->samChannelOpen = false;
                            }

                            void CalypsoClassicTransactionEngine::setReaders(std::shared_ptr<SeReader> poReader, std::shared_ptr<SeReader> samReader) {
                                this->poReader = poReader;
                                this->samReader = samReader;
                            }

                            void CalypsoClassicTransactionEngine::doCalypsoReadWriteTransaction(std::shared_ptr<PoTransaction> poTransaction, bool closeSeChannel) throw(KeypleReaderException) {

                                bool poProcessStatus;

                                /*
                                 * Read commands to execute during the opening step: EventLog, ContractList
                                 */

                                /* prepare Event Log read record */
                                std::shared_ptr<ReadRecordsRespPars> readEventLogParser = poTransaction->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_EventLog, ReadDataStructure::SINGLE_RECORD_DATA, CalypsoClassicInfo::RECORD_NUMBER_1, std::string::format("EventLog (SFI=%02X, recnbr=%d))", CalypsoClassicInfo::SFI_EventLog, CalypsoClassicInfo::RECORD_NUMBER_1));


                                /* prepare Contract List read record */
                                std::shared_ptr<ReadRecordsRespPars> readContractListParser = poTransaction->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_ContractList, ReadDataStructure::SINGLE_RECORD_DATA, CalypsoClassicInfo::RECORD_NUMBER_1, std::string::format("ContractList (SFI=%02X))", CalypsoClassicInfo::SFI_ContractList));

                                if (logger->isInfoEnabled()) {
                                    logger->info("========= PO Calypso session ======= Opening ============================");
                                }

                                /*
                                 * Open Session for the debit key - with reading of the first record of the cyclic EF of
                                 * Environment and Holder file
                                 */
                                poProcessStatus = poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT, CalypsoClassicInfo::SFI_EnvironmentAndHolder, CalypsoClassicInfo::RECORD_NUMBER_1);

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                logger->info("Parsing Read EventLog file: " + readEventLogParser->toString());

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                logger->info("Parsing Read ContractList file: " + readContractListParser->toString());

                                if (!poTransaction->wasRatified()) {
                                    logger->info("========= Previous Secure Session was not ratified. =====================");

                                    /*
                                     * [------------------------------------]
                                     *
                                     * The previous Secure Session has not been ratified, so we simply close the Secure
                                     * Session.
                                     *
                                     * We would analyze here the event log read during the opening phase.
                                     *
                                     * [------------------------------------]
                                     */

                                    if (logger->isInfoEnabled()) {
                                        logger->info("========= PO Calypso session ======= Closing ============================");
                                    }

                                    /*
                                     * A ratification command will be sent (CONTACTLESS_MODE).
                                     */
                                    poProcessStatus = poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);

                                }
                                else {
                                    /*
                                     * [------------------------------------]
                                     *
                                     * Place to analyze the PO profile available in seResponse: Environment/Holder,
                                     * EventLog, ContractList.
                                     *
                                     * The information available allows the determination of the contract to be read.
                                     *
                                     * [------------------------------------]
                                     */

                                    if (logger->isInfoEnabled()) {
                                        logger->info("========= PO Calypso session ======= Processing of PO commands =======================");
                                    }

                                    /* Read contract command (we assume we have determine Contract #1 to be read. */
                                    /* prepare Contract #1 read record */
                                    std::shared_ptr<ReadRecordsRespPars> readContractsParser = poTransaction->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_Contracts, ReadDataStructure::MULTIPLE_RECORD_DATA, CalypsoClassicInfo::RECORD_NUMBER_1, std::string::format("Contracts (SFI=%02X, recnbr=%d)", CalypsoClassicInfo::SFI_Contracts, CalypsoClassicInfo::RECORD_NUMBER_1));

                                    /* proceed with the sending of commands, don't close the channel */
                                    poProcessStatus = poTransaction->processPoCommands(ChannelState::KEEP_OPEN);

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                    logger->info("Parsing Read Contract file: " + readContractsParser->toString());

                                    if (logger->isInfoEnabled()) {
                                        logger->info("========= PO Calypso session ======= Closing ============================");
                                    }

                                    /*
                                     * [------------------------------------]
                                     *
                                     * Place to analyze the Contract (in seResponse).
                                     *
                                     * The content of the contract will grant or not access.
                                     *
                                     * In any case, a new record will be added to the EventLog.
                                     *
                                     * [------------------------------------]
                                     */

                                    /* prepare Event Log append record */
                                    std::shared_ptr<AppendRecordRespPars> appendEventLogParser = poTransaction->prepareAppendRecordCmd(CalypsoClassicInfo::SFI_EventLog, ByteArrayUtils::fromHex(CalypsoClassicInfo::eventLog_dataFill), std::string::format("EventLog (SFI=%02X)", CalypsoClassicInfo::SFI_EventLog));

                                    /*
                                     * A ratification command will be sent (CONTACTLESS_MODE).
                                     */
                                    poProcessStatus = poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);

//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                                    logger->info("Parsing Append EventLog file: " + appendEventLogParser->toString());
                                }

                                if (poTransaction->isSuccessful()) {
                                    if (logger->isInfoEnabled()) {
                                        logger->info("========= PO Calypso session ======= SUCCESS !!!!!!!!!!!!!!!!!!!!!!!!!!!!");
                                    }
                                }
                                else {
                                    logger->error("========= PO Calypso session ======= ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
                                }
                            }

                            std::shared_ptr<SelectionRequest> CalypsoClassicTransactionEngine::preparePoSelection() {
                                /*
                                 * Initialize the selection process for the poReader
                                 */
                                seSelection = std::make_shared<SeSelection>(poReader);

                                /* operate multiple PO selections */
                                std::string poFakeAid1 = "AABBCCDDEE"; // fake AID 1
                                std::string poFakeAid2 = "EEDDCCBBAA"; // fake AID 2

                                /*
                                 * Add selection case 1: Fake AID1, protocol ISO, target rev 3
                                 */
                                seSelection->prepareSelection(std::make_shared<PoSelector>(ByteArrayUtils::fromHex(poFakeAid1), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4, "Selector with fake AID1"));

                                /*
                                 * Add selection case 2: Calypso application, protocol ISO, target rev 2 or 3
                                 *
                                 * addition of read commands to execute following the selection
                                 */
                                std::shared_ptr<PoSelector> poSelectorCalypsoAid = std::make_shared<PoSelector>(ByteArrayUtils::fromHex(CalypsoClassicInfo::AID), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4, "Calypso selector");

                                poSelectorCalypsoAid->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_EventLog, ReadDataStructure::SINGLE_RECORD_DATA, CalypsoClassicInfo::RECORD_NUMBER_1, "EventLog (selection step)");

                                seSelection->prepareSelection(poSelectorCalypsoAid);

                                /*
                                 * Add selection case 3: Fake AID2, unspecified protocol, target rev 2 or 3
                                 */
                                seSelection->prepareSelection(std::make_shared<PoSelector>(ByteArrayUtils::fromHex(poFakeAid2), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_B_PRIME, "Selector with fake AID2"));

                                /*
                                 * Add selection case 4: ATR selection, rev 1 atrregex
                                 */
                                seSelection->prepareSelection(std::make_shared<PoSelector>(CalypsoClassicInfo::ATR_REV1_REGEX, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_B_PRIME, "Selector with fake AID2"));

                                return seSelection->getSelectionOperation();
                            }

                            void CalypsoClassicTransactionEngine::processSeMatch(std::shared_ptr<SelectionResponse> selectionResponse) {
                                if (seSelection->processDefaultSelection(selectionResponse)) {
                                    std::shared_ptr<MatchingSe> selectedSe = seSelection->getSelectedSe();
                                    try {
                                        /* first time: check SAM */
                                        if (!this->samChannelOpen) {
                                            /* the following method will throw an exception if the SAM is not available. */
                                            CalypsoUtilities::checkSamAndOpenChannel(samReader);
                                            this->samChannelOpen = true;
                                        }

                                        std::shared_ptr<Profiler> profiler = std::make_shared<Profiler>("Entire transaction");

                                        /* Time measurement */
                                        profiler->start("Initial selection");

                                        profiler->start("Calypso1");

                                        std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(poReader, std::static_pointer_cast<CalypsoPo>(selectedSe), samReader, samSetting);

                                        doCalypsoReadWriteTransaction(poTransaction, true);

                                        profiler->stop();
                                        logger->warn(System::getProperty("line.separator") + "{}", profiler);
                                    }
                                    catch (const std::runtime_error &e) {
                                        e.printStackTrace();
                                    }
                                }
                                else {
                                    logger->info("No SE matched the selection");
                                }
                            }

                            void CalypsoClassicTransactionEngine::processSeInsertion() {
                                std::cout << "Unexpected SE insertion event" << std::endl;
                            }

                            void CalypsoClassicTransactionEngine::processSeRemoval() {
                                std::cout << "SE removal event" << std::endl;
                            }

                            void CalypsoClassicTransactionEngine::processUnexpectedSeRemoval() {
                                std::cout << "Unexpected SE removal event" << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
}
