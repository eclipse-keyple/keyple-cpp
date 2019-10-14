#include "Demo_ValidationTransaction.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoTransaction.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadDataStructure.h"
#include "../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadRecordsRespPars.h"
#include "../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/UpdateRecordRespPars.h"
#include "../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/AppendRecordRespPars.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/TransmissionMode.h"
#include "../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/CalypsoPo.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelection.h"
#include "../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoSelectionRequest.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeSelector.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/Protocol.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../calypso/PoFileStructureInfo.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.h"
#include "DemoUtilities.h"
#include "../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscReader.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocolSetting.h"
#include "../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscProtocolSetting.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelectionRequest.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace integration {
                namespace example {
                    namespace pc {
                        namespace calypso {
                            using AppendRecordRespPars = org::eclipse::keyple::calypso::command::po::parser::AppendRecordRespPars;
                            using ReadDataStructure = org::eclipse::keyple::calypso::command::po::parser::ReadDataStructure;
                            using ReadRecordsRespPars = org::eclipse::keyple::calypso::command::po::parser::ReadRecordsRespPars;
                            using UpdateRecordRespPars = org::eclipse::keyple::calypso::command::po::parser::UpdateRecordRespPars;
                            using CalypsoPo = org::eclipse::keyple::calypso::transaction::CalypsoPo;
                            using PoSelectionRequest = org::eclipse::keyple::calypso::transaction::PoSelectionRequest;
                            using PoTransaction = org::eclipse::keyple::calypso::transaction::PoTransaction;
                            using PoFileStructureInfo = org::eclipse::keyple::integration::calypso::PoFileStructureInfo;
                            using PcscPlugin = org::eclipse::keyple::plugin::pcsc::PcscPlugin;
                            using PcscProtocolSetting = org::eclipse::keyple::plugin::pcsc::PcscProtocolSetting;
                            using PcscReader = org::eclipse::keyple::plugin::pcsc::PcscReader;
                            using namespace org::eclipse::keyple::seproxy;
                            using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                            using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                            using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                            using namespace org::eclipse::keyple::seproxy::message;
                            using Protocol = org::eclipse::keyple::seproxy::protocol::Protocol;
                            using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                            using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                            using SeSelection = org::eclipse::keyple::transaction::SeSelection;
                            using SeSelectionRequest = org::eclipse::keyple::transaction::SeSelectionRequest;
                            using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                            void Demo_ValidationTransaction::update(std::shared_ptr<ReaderEvent> event_Renamed) {
                                switch (event_Renamed->getEventType()) {
                                    case SE_INSERTED:
                                        std::cout << "\n==================================================================================" << std::endl;
                                        std::cout << "Found a Calypso PO! Validating...\n" << std::endl;
                                        detectAndHandlePO();
                                        break;
                                    case SE_REMOVAL:
                                        std::cout << "\nWaiting for new Calypso PO..." << std::endl;
                                        break;
                                    default:
                                        std::cout << "IO Error" << std::endl;
                                }
                            }

                            std::vector<char> Demo_ValidationTransaction::longToBytes(long long lg) {
                                std::vector<char> result(8);
                                long long l = lg;
                                for (int i = 7; i >= 0; i--) {
                                    result[i] = static_cast<char>(l & 0xFF);
                                    l >>= 8;
                                }
                                return result;
                            }

                            long long Demo_ValidationTransaction::bytesToLong(std::vector<char> &b) {
                                long long result = 0;
                                for (int i = 0; i < 8; i++) {
                                    result <<= 8;
                                    result |= (b[i] & 0xFF);
                                }
                                return result;
                            }

                            int Demo_ValidationTransaction::getCounterValueFromByteArray(std::vector<char> &inByteArray, int inCounterIndex) {
                                int counterValue = 0;

                                for (int i = 0; i < 3; i++) {
                                    counterValue <<= 8;
                                    counterValue |= (inByteArray[i + (3 * (inCounterIndex - 1))] & 0xFF);
                                }

                                return counterValue;
                            }

                            std::vector<char> Demo_ValidationTransaction::getByteArrayFromCounterValue(int inCounterValue) {

                                std::vector<char> result(3);

                                int counter = inCounterValue;
                                for (int i = 2; i >= 0; i--) {
                                    result[i] = static_cast<char>(inCounterValue & 0xFF);
                                    counter >>= 8;
                                }

                                return result;
                            }

                            void Demo_ValidationTransaction::validateAuditC0(std::shared_ptr<PoTransaction> poTransaction) throw(KeypleReaderException) {

                                char eventSfi = 0x08;
                                char contractListSfi = 0x1E;
                                char environmentSfi = 0x07;


                                std::shared_ptr<ReadRecordsRespPars> readEventParser = poTransaction->prepareReadRecordsCmd(eventSfi, ReadDataStructure::SINGLE_RECORD_DATA, static_cast<char>(0x01), "Event");
                                std::shared_ptr<ReadRecordsRespPars> readContractListParser = poTransaction->prepareReadRecordsCmd(contractListSfi, ReadDataStructure::SINGLE_RECORD_DATA, static_cast<char>(0x01), "ContractList");

                                // Open Session with debit key #3 and reading the Environment at SFI 07h
                                // Files to read during the beginning of the session: Event (SFI 0x08) and ContractList (SFI
                                // 0x1E)
                                poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT, environmentSfi, static_cast<char>(0x01));

                                char contractIndex = readContractListParser->getRecords()->get(1)[0];
                                std::vector<char> eventTimestampData = Arrays::copyOfRange(readEventParser->getRecords()->get(1), 1, (sizeof(long long) / sizeof(char)) + 1);

                                std::string timeStampString = (std::make_shared<SimpleDateFormat>("dd/MM/yyyy HH:mm:ss"))->format(Date(bytesToLong(eventTimestampData)));

                                std::cout << "\t------------------------------------------------------------------------------" << std::endl;
                                std::string nameInPO = std::string(poTransaction->getOpenRecordDataRead());
                                std::cout << "\tName in PO:: " << nameInPO << std::endl;
                                std::cout << "\t------------------------------------------------------------------------------" << std::endl;
                                std::cout << "\tPrevious Event Information" << std::endl;
                                std::cout << "\t- Index of Validated Contract:: " << (contractIndex == 0 ? 4 : contractIndex) << std::endl;
                                std::cout << "\t- Contract Type:: Season Pass" << std::endl;
                                std::cout << "\t- Event DateTime:: " << timeStampString << std::endl;
                                std::cout << "\t------------------------------------------------------------------------------\n" << std::endl;

                                std::shared_ptr<ReadRecordsRespPars> readContractParser = poTransaction->prepareReadRecordsCmd(static_cast<char>(0x29), ReadDataStructure::SINGLE_RECORD_DATA, static_cast<char>(contractIndex + 1), static_cast<char>(0x1D), "Contract");

                                poTransaction->processPoCommandsInSession();

                                std::cout << "Reading contract #" << (contractIndex << 1) << " for current validation..." << std::endl;

                                /*
                                 * System.out.println("Contract#" + (contractIndex+1) + ": " +
                                 * ByteArrayUtils.toHex(dataReadInSession.getApduResponses().get(0).getDataOut()) +
                                 * ", SW1SW2: " +
                                 * Integer.toHexString(dataReadInSession.getApduResponses().get(0).getStatusCode() &
                                 * 0xFFFF));
                                 */

                                std::vector<char> newEventData = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
                                std::vector<char> newContractListData = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

                                if (contractIndex < 3) {
                                    newContractListData[0] = static_cast<char>(contractIndex + 1);
                                }

                                newEventData[0] = static_cast<char>(contractIndex + 1);

                                std::vector<char> dateToInsert = longToBytes((Date()).getTime());
                                System::arraycopy(dateToInsert, 0, newEventData, 1, (sizeof(long long) / sizeof(char)));

                                std::shared_ptr<UpdateRecordRespPars> updateContractListParser = poTransaction->prepareUpdateRecordCmd(contractListSfi, static_cast<char>(0x01), newContractListData, "ContractList");
                                std::shared_ptr<AppendRecordRespPars> appendEventPars = poTransaction->prepareAppendRecordCmd(eventSfi, newEventData, "Event");

                                poTransaction->processPoCommandsInSession();

                                poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);

                                std::cout << "\nValidation Successful!" << std::endl;
                                std::cout << "==================================================================================" << std::endl;
                            }

                            void Demo_ValidationTransaction::validateClap(std::shared_ptr<CalypsoPo> detectedPO) throw(KeypleReaderException) {

                                char eventSfi = 0x08;
                                char countersSfi = 0x1B;
                                char environmentSfi = 0x14;
                                char contractsSfi = 0x29;

                                std::shared_ptr<SeResponse> dataReadInSession;
                                std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(poReader, detectedPO, samReader, nullptr);

                                std::shared_ptr<ReadRecordsRespPars> readEventParser = poTransaction->prepareReadRecordsCmd(eventSfi, ReadDataStructure::SINGLE_RECORD_DATA, static_cast<char>(0x01), "Event");
                                std::shared_ptr<ReadRecordsRespPars> readCountersParser = poTransaction->prepareReadRecordsCmd(countersSfi, ReadDataStructure::SINGLE_COUNTER, static_cast<char>(0x01), "Counters");
                                poTransaction->prepareReadRecordsCmd(contractsSfi, ReadDataStructure::MULTIPLE_RECORD_DATA, static_cast<char>(0x01), "Contracts");

                                // Open Session with debit key #3 and reading the Environment at SFI 07h
                                // Files to read during the beginning of the session: Event (SFI 0x08), Counters (SFI 0x1B)
                                // and all records of the Contracts (SFI 0x29)
                                poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT, environmentSfi, static_cast<char>(0x01));

                                std::vector<char> eventTimestampData = Arrays::copyOfRange(readEventParser->getRecords()->get(1), 1, (sizeof(long long) / sizeof(char)) + 1);

                                std::string timeStampString = (std::make_shared<SimpleDateFormat>("dd/MM/yyyy HH:mm:ss"))->format(Date(bytesToLong(eventTimestampData)));

                                int counterValue = readCountersParser->getCounters()->get(0);

                                std::cout << "\t------------------------------------------------------------------------------" << std::endl;
                                std::string nameInPO = std::string(poTransaction->getOpenRecordDataRead());
                                std::cout << "\tName in PO:: " << nameInPO << std::endl;
                                std::cout << "\t------------------------------------------------------------------------------" << std::endl;
                                std::cout << "\tPrevious Event Information" << std::endl;
                                std::cout << "\t- Index of Validated Contract:: 1" << std::endl;
                                std::cout << "\t- Contract Type:: MultiTrip Ticket" << std::endl;
                                std::cout << "\t- Counter Value:: " << counterValue << std::endl;
                                std::cout << "\t- Event DateTime:: " << timeStampString << std::endl;
                                std::cout << "\t------------------------------------------------------------------------------\n" << std::endl;

                                std::cout << "All contracts read during the beginning of the current transaction..." << std::endl;

                                // Perform automatic top-up when the value is 0 by closing the current session and opening a
                                // new one with a loading key
                                if (counterValue == 0) {

                                    std::cout << "No value present in the card. Initiating auto top-up..." << std::endl;

                                    poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);

                                    poTransaction = std::make_shared<PoTransaction>(poReader, detectedPO, samReader, nullptr);

                                    poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_LOAD, static_cast<char>(0x00), static_cast<char>(0x00));

                                    std::vector<char> newCounterData = {0x00, 0x00, 0x05, 0x00, 0x00, 0x00};

                                    poTransaction->prepareUpdateRecordCmd(countersSfi, static_cast<char>(0x01), newCounterData, "Counter");
                                    counterValue = 5;
                                }

                                /*
                                 * System.out.println("Contract#" + (contractIndex+1) + ": " +
                                 * ByteArrayUtils.toHex(dataReadInSession.getApduResponses().get(0).getDataOut()) +
                                 * ", SW1SW2: " +
                                 * Integer.toHexString(dataReadInSession.getApduResponses().get(0).getStatusCode() &
                                 * 0xFFFF));
                                 */

                                std::vector<char> newEventData = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

                                std::vector<char> dateToInsert = longToBytes((Date()).getTime());
                                System::arraycopy(dateToInsert, 0, newEventData, 1, (sizeof(long long) / sizeof(char)));

                                poTransaction->prepareAppendRecordCmd(eventSfi, newEventData, "Event");

                                poTransaction->prepareDecreaseCmd(countersSfi, static_cast<char>(0x01), 1, "Counter decval=1");

                                poTransaction->processPoCommandsInSession();

                                poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);

                                std::cout << "\nValidation Successful!" << std::endl;
                                std::cout << "==================================================================================" << std::endl;
                            }

                            void Demo_ValidationTransaction::detectAndHandlePO() {

                                try {
                                    // operate PO multiselection
                                    std::string poAuditC0Aid = "315449432E4943414C54"; // AID of the PO with Audit C0 profile
                                    std::string clapAid = "315449432E494341D62010029101"; // AID of the CLAP product being tested
                                    std::string cdLightAid = "315449432E494341"; // AID of the Rev2.4 PO emulating CDLight

                                    std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>(poReader);

                                    // Add Audit C0 AID to the list
                                    std::shared_ptr<CalypsoPo> auditC0Se = std::static_pointer_cast<CalypsoPo>(seSelection->prepareSelection(std::make_shared<PoSelectionRequest>(std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(PoFileStructureInfo::poAuditC0Aid), nullptr), nullptr, "Audit C0"), ChannelState::KEEP_OPEN, Protocol::ANY)));

                                    // Add CLAP AID to the list
                                    std::shared_ptr<CalypsoPo> clapSe = std::static_pointer_cast<CalypsoPo>(seSelection->prepareSelection(std::make_shared<PoSelectionRequest>(std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(PoFileStructureInfo::clapAid), nullptr), nullptr, "CLAP"), ChannelState::KEEP_OPEN, Protocol::ANY)));

                                    // Add cdLight AID to the list
                                    std::shared_ptr<CalypsoPo> cdLightSe = std::static_pointer_cast<CalypsoPo>(seSelection->prepareSelection(std::make_shared<PoSelectionRequest>(std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(PoFileStructureInfo::cdLightAid), nullptr), nullptr, "CDLight"), ChannelState::KEEP_OPEN, Protocol::ANY)));

                                    if (!seSelection->processExplicitSelection()) {
                                        throw std::invalid_argument("No recognizable PO detected.");
                                    }


                                    // Depending on the PO detected perform either a Season Pass validation or a MultiTrip
                                    // validation
                                    if (auditC0Se->isSelected()) {

                                        std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(poReader, auditC0Se, samReader, nullptr);
                                        validateAuditC0(poTransaction);

                                    }
                                    else if (clapSe->isSelected()) {

                                        std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(poReader, clapSe, samReader, nullptr);
                                        validateClap(clapSe);

                                    }
                                    else if (cdLightSe->isSelected()) {

                                        std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(poReader, cdLightSe, samReader, nullptr);
                                        validateAuditC0(poTransaction);

                                    }
                                    else {
                                        std::cout << "No recognizable PO detected." << std::endl;
                                    }

                                }
                                catch (const std::runtime_error &e) {
                                    // TODO Auto-generated catch block
                                    e.printStackTrace();
                                }
                            }

const std::shared_ptr<void> Demo_ValidationTransaction::waitForEnd = nullptr;

                            void Demo_ValidationTransaction::main(std::vector<std::string> &args) throw(IOException, InterruptedException, KeypleBaseException) {

                                std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();
                                std::shared_ptr<SortedSet<std::shared_ptr<ReaderPlugin>>> pluginsSet = std::make_shared<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>>();
                                pluginsSet->add(PcscPlugin::getInstance());
                                seProxyService->setPlugins(pluginsSet);

                                std::shared_ptr<SeReader> poReader = DemoUtilities::getReader(seProxyService, DemoUtilities::PO_READER_NAME_REGEX);
                                std::shared_ptr<SeReader> samReader = DemoUtilities::getReader(seProxyService, DemoUtilities::SAM_READER_NAME_REGEX);


                                if (poReader == samReader || poReader == nullptr || samReader == nullptr) {
                                    throw std::make_shared<IllegalStateException>("Bad PO/SAM setup");
                                }

                                std::cout << "\n==================================================================================" << std::endl;
                                std::cout << "PO Reader  : " << poReader->getName() << std::endl;
                                std::cout << "SAM Reader : " << samReader->getName() << std::endl;
                                std::cout << "==================================================================================" << std::endl;

                                poReader->setParameter(PcscReader::SETTING_KEY_PROTOCOL, PcscReader::SETTING_PROTOCOL_T1);
                                samReader->setParameter(PcscReader::SETTING_KEY_PROTOCOL, PcscReader::SETTING_PROTOCOL_T0);

                                // provide the reader with the map
                                poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(PcscProtocolSetting::SETTING_PROTOCOL_ISO14443_4));

                                const std::string SAM_ATR_REGEX = "3B3F9600805A[0-9a-fA-F]{2}80[0-9a-fA-F]{16}829000";

                                std::shared_ptr<SeSelection> samSelection = std::make_shared<SeSelection>(samReader);

                                std::shared_ptr<SeSelectionRequest> samSelectionRequest = std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(nullptr, std::make_shared<SeSelector::AtrFilter>(SAM_ATR_REGEX), "SAM Selection"), ChannelState::KEEP_OPEN, Protocol::ANY);

                                /* Prepare selector, ignore MatchingSe here */
                                samSelection->prepareSelection(samSelectionRequest);

                                try {
                                    if (!samSelection->processExplicitSelection()) {
                                        std::cout << "Unable to open a logical channel for SAM!" << std::endl;
                                        throw std::make_shared<IllegalStateException>("SAM channel opening failure");
                                    }
                                }
                                catch (const KeypleReaderException &e) {
                                    throw std::make_shared<IllegalStateException>("Reader exception: " + e->what());

                                }

                                // Setting up ourselves as an observer
                                std::shared_ptr<Demo_ValidationTransaction> observer = std::make_shared<Demo_ValidationTransaction>();
                                observer->poReader = poReader;
                                observer->samReader = samReader;

                                std::cout << "\nReady for PO presentation!" << std::endl;

                                // Set terminal as Observer of the first reader
                                (std::static_pointer_cast<ObservableReader>(poReader))->addObserver(observer);
//JAVA TO C++ CONVERTER TODO TASK: Multithread locking is not converted to native C++ unless you choose one of the options on the 'Modern C++ Options' dialog:
                                synchronized(waitForEnd) {
                                    waitForEnd->wait();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
