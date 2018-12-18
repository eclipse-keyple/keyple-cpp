#include "CommandSetTestSuite.h"
#include "TestEngine.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadRecordsRespPars.h"
#include "../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/CalypsoPo.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoTransaction.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/TransmissionMode.h"
#include "PoFileStructureInfo.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/PoRevision.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace integration {
                namespace calypso {
                    using PoRevision = org::eclipse::keyple::calypso::command::po::PoRevision;
                    using ReadDataStructure = org::eclipse::keyple::calypso::command::po::parser::ReadDataStructure;
                    using ReadRecordsRespPars = org::eclipse::keyple::calypso::command::po::parser::ReadRecordsRespPars;
                    using CalypsoPo = org::eclipse::keyple::calypso::transaction::CalypsoPo;
                    using PoTransaction = org::eclipse::keyple::calypso::transaction::PoTransaction;
                    using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                    using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                    using org::junit::jupiter::api::Assertions;
                    using org::junit::jupiter::api::BeforeAll;
                    using org::junit::jupiter::api::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @BeforeAll public static void setUp()
                    void CommandSetTestSuite::setUp() {
                        try {
                            TestEngine::configureReaders();
                        }
                        catch (const IOException &e) {
                            e->printStackTrace();
                        }
                        catch (const InterruptedException &e) {
                            e->printStackTrace();
                        }
                        catch (const KeypleBaseException &e) {
                            e->printStackTrace();
                        }
                    }

                    int CommandSetTestSuite::getCounterValueFromByteArray(std::vector<char> &inByteArray, int inCounterIndex) {
                        int counterValue = 0;

                        for (int i = 0; i < 3; i++) {
                            counterValue <<= 8;
                            counterValue |= (inByteArray[i + (3 * (inCounterIndex - 1))] & 0xFF);
                        }

                        return counterValue;
                    }

                    std::shared_ptr<ReadRecordsRespPars> CommandSetTestSuite::readRecords(std::shared_ptr<CalypsoPo> calypsoPo, Byte fileSfi, Byte recordNumber, ReadDataStructure readDataStructureValue) throw(KeypleReaderException) {

                        std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(TestEngine::poReader, calypsoPo, TestEngine::samReader, nullptr);

                        std::shared_ptr<ReadRecordsRespPars> readRecordsRespPars = poTransaction->prepareReadRecordsCmd(fileSfi, readDataStructureValue, static_cast<char>(recordNumber), std::string::format("SFI=%02X, recnbr=%d", fileSfi, recordNumber));

                        poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT, static_cast<char>(0x00), static_cast<char>(0x00));

                        poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);
                        /*
                         * System.out.println("DataRead#: " +
                         * ByteArrayUtils.toHex(dataReadInSession.getApduResponses().get(1).getDataOut()));
                         * 
                         * System.out.println("SW1SW2: " +
                         * Integer.toHexString(dataReadInSession.getApduResponses().get(1).getStatusCode() &
                         * 0xFFFF));
                         */
                        return readRecordsRespPars;
                    }

                    void CommandSetTestSuite::updateRecord(std::shared_ptr<CalypsoPo> calypsoPo, Byte sfi, Byte recordNumber, std::vector<char> &dataToWrite) throw(KeypleReaderException) {

                        std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(TestEngine::poReader, calypsoPo, TestEngine::samReader, nullptr);

                        poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_LOAD, static_cast<char>(0x00), static_cast<char>(0x00));

                        poTransaction->prepareUpdateRecordCmd(sfi, recordNumber, dataToWrite, std::string::format("SFI=%02X, recnbr=%d", sfi, recordNumber));
                        poTransaction->processPoCommands(ChannelState::KEEP_OPEN);

                        poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);
                    }

                    void CommandSetTestSuite::decreaseCounter(std::shared_ptr<CalypsoPo> calypsoPo, Byte countersSfi, Byte counterIndex, int valueToDecrement) throw(KeypleReaderException) {

                        std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(TestEngine::poReader, calypsoPo, TestEngine::samReader, nullptr);

                        poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT, static_cast<char>(0x00), static_cast<char>(0x00));

                        poTransaction->prepareDecreaseCmd(countersSfi, counterIndex, valueToDecrement, std::string::format("SFI=%02X, index=%d, decvalue=%d", countersSfi, counterIndex, valueToDecrement));

                        poTransaction->processPoCommands(ChannelState::KEEP_OPEN);

                        poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);
                    }

                    void CommandSetTestSuite::increaseCounter(std::shared_ptr<CalypsoPo> calypsoPo, Byte countersSfi, Byte counterIndex, int valueToIncrement) throw(KeypleReaderException) {

                        std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(TestEngine::poReader, calypsoPo, TestEngine::samReader, nullptr);

                        poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_LOAD, static_cast<char>(0x00), static_cast<char>(0x00));

                        poTransaction->prepareIncreaseCmd(countersSfi, counterIndex, valueToIncrement, std::string::format("SFI=%02X, index=%d, decvalue=%d", countersSfi, counterIndex, valueToIncrement));

                        poTransaction->processPoCommands(ChannelState::KEEP_OPEN);

                        poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);
                    }

                    void CommandSetTestSuite::appendRecord(std::shared_ptr<CalypsoPo> calypsoPo, Byte sfi, std::vector<char> &dataToWrite) throw(KeypleReaderException) {

                        std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(TestEngine::poReader, calypsoPo, TestEngine::samReader, nullptr);

                        poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT, static_cast<char>(0x00), static_cast<char>(0x00));

                        poTransaction->prepareAppendRecordCmd(sfi, dataToWrite, std::string::format("SFI=%02X", sfi));

                        poTransaction->processPoCommands(ChannelState::KEEP_OPEN);

                        poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testWriteCounter()
                    void CommandSetTestSuite::testWriteCounter() {

                        try {

                            std::shared_ptr<PoFileStructureInfo> poData = TestEngine::selectPO();

                            std::string genericCounterData = "00000A 000100 000B00 010000 0C0000 0000B0 00C000 0F0000 00000D 0000";

                            std::vector<char> counterData(poData->getCountersFileData()->getRecSize());

                            System::arraycopy(ByteArrayUtils::fromHex(genericCounterData), 0, counterData, 0, counterData.size());

                            updateRecord(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getCountersFileData()->getSfi(), static_cast<char>(0x01), counterData);

                            std::shared_ptr<ReadRecordsRespPars> readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getCountersFileData()->getSfi(), static_cast<char>(0x01), ReadDataStructure::SINGLE_RECORD_DATA);
                            std::vector<char> updatedCounterData = readRecordsRespPars->getRecords()->get(1);

                            Assertions::assertArrayEquals(counterData, updatedCounterData);

                            if (poData->getSimulatedCountersFileData()->getRecNumb() > 0) {

                                for (int i = 0; i < poData->getSimulatedCountersFileData()->getRecNumb(); i++) {

                                    readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), static_cast<char>(poData->getSimulatedCountersFileData()->getSfi() + i), static_cast<char>(0x01), ReadDataStructure::SINGLE_COUNTER);

                                    int updatedCounterValue = readRecordsRespPars->getCounters()->get(0);

                                    int expectedCounterValue = getCounterValueFromByteArray(counterData, i + 1);

                                    Assertions::assertEquals(expectedCounterValue, updatedCounterValue);

                                }
                            }

                        }
                        catch (const std::runtime_error &e) {

                            Assertions::fail("Exception caught: " + e.what());
                            e.printStackTrace();
                        }

                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDecreaseCounter()
                    void CommandSetTestSuite::testDecreaseCounter() {

                        try {

                            std::shared_ptr<PoFileStructureInfo> poData = TestEngine::selectPO();

                            std::shared_ptr<ReadRecordsRespPars> readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getCountersFileData()->getSfi(), static_cast<char>(0x01), ReadDataStructure::MULTIPLE_COUNTER);

                            std::shared_ptr<SortedMap<Integer, Integer>> initialCounterData = readRecordsRespPars->getCounters();

                            for (int i = 0; i < (poData->getCountersFileData()->getRecSize() / 3); i++) {

                                int counterValue = initialCounterData->get(i);

                                if (counterValue > 0) {

                                    int valueToDecrement = counterValue / 2;
                                    decreaseCounter(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getCountersFileData()->getSfi(), static_cast<char>(i + 1), valueToDecrement);

                                    readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getCountersFileData()->getSfi(), static_cast<char>(0x01), ReadDataStructure::MULTIPLE_COUNTER);

                                    std::shared_ptr<SortedMap<Integer, Integer>> updatedCounters = readRecordsRespPars->getCounters();
                                    int finalValue = updatedCounters->get(i);

                                    Assertions::assertEquals(counterValue - valueToDecrement, finalValue);

                                    if (poData->getSimulatedCountersFileData()->getRecNumb() > 0) {

                                        readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), static_cast<char>(poData->getSimulatedCountersFileData()->getSfi() + i), static_cast<char>(0x01), ReadDataStructure::SINGLE_COUNTER);

                                        finalValue = readRecordsRespPars->getCounters()->get(0);

                                        Assertions::assertEquals(counterValue - valueToDecrement, finalValue);
                                    }
                                }
                            }

                        }
                        catch (const std::runtime_error &e) {

                            Assertions::fail("Exception caught: " + e.what());
                            e.printStackTrace();
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIncreaseCounter()
                    void CommandSetTestSuite::testIncreaseCounter() {

                        try {

                            std::shared_ptr<PoFileStructureInfo> poData = TestEngine::selectPO();

                            std::shared_ptr<ReadRecordsRespPars> readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getCountersFileData()->getSfi(), static_cast<char>(0x01), ReadDataStructure::MULTIPLE_COUNTER);

                            std::shared_ptr<SortedMap<Integer, Integer>> initialCounterData = readRecordsRespPars->getCounters();

                            for (int i = 0; i < (poData->getCountersFileData()->getRecSize() / 3); i++) {

                                int counterValue = initialCounterData->get(i);

                                int maxValue = 0xFFFFFF;

                                if (counterValue < maxValue) {

                                    int valueToIncrement = (maxValue - counterValue) / 2;
                                    increaseCounter(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getCountersFileData()->getSfi(), static_cast<char>(i + 1), valueToIncrement);

                                    readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getCountersFileData()->getSfi(), static_cast<char>(0x01), ReadDataStructure::MULTIPLE_COUNTER);

                                    std::shared_ptr<SortedMap<Integer, Integer>> updatedCounters = readRecordsRespPars->getCounters();

                                    int finalValue = updatedCounters->get(i);

                                    Assertions::assertEquals(counterValue + valueToIncrement, finalValue);

                                    if (poData->getSimulatedCountersFileData()->getRecNumb() > 0) {

                                        readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), static_cast<char>(poData->getSimulatedCountersFileData()->getSfi() + i), static_cast<char>(0x01), ReadDataStructure::SINGLE_COUNTER);

                                        finalValue = readRecordsRespPars->getCounters()->get(0);

                                        Assertions::assertEquals(counterValue + valueToIncrement, finalValue);
                                    }
                                }
                            }

                        }
                        catch (const std::runtime_error &e) {

                            Assertions::fail("Exception caught: " + e.what());
                            e.printStackTrace();
                        }

                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testReadRecords()
                    void CommandSetTestSuite::testReadRecords() {

                        try {

                            std::shared_ptr<PoFileStructureInfo> poData = TestEngine::selectPO();
                            ReadDataStructure readDataStructureValue = ReadDataStructure::MULTIPLE_RECORD_DATA;

                            PoRevision poRevision = (std::make_shared<PoTransaction>(TestEngine::poReader, std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), TestEngine::samReader, nullptr))->getRevision();


                            std::vector<std::vector<char>> recordsData;

                            for (int i = 0; i < poData->getContractFileData()->getRecNumb(); i++) {

                                std::vector<char> recordData(poData->getContractFileData()->getRecSize());
                                Arrays::fill(recordData, static_cast<char>(i + 1));
                                recordsData.push_back(recordData);

                                updateRecord(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getContractFileData()->getSfi(), static_cast<char>(i + 1), recordData);
                            }

                            if (poRevision == PoRevision::REV2_4) {
                                readDataStructureValue = ReadDataStructure::SINGLE_RECORD_DATA;
                            }

                            std::shared_ptr<ReadRecordsRespPars> readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getContractFileData()->getSfi(), static_cast<char>(0x01), readDataStructureValue);

                            if (poRevision == PoRevision::REV2_4) {

                                // Rev2.4 doesn't read multiple contracts. It will only read the first.
                                Assertions::assertEquals(poData->getContractFileData()->getRecSize(), readRecordsRespPars->getRecords()->get(1)->length);
                                Assertions::assertArrayEquals(recordsData[0], readRecordsRespPars->getRecords()->get(1));

                            }
                            else {

                                for (int i = 0; i < poData->getContractFileData()->getRecNumb(); i++) {

                                    Assertions::assertArrayEquals(recordsData[i], readRecordsRespPars->getRecords()->get(i + 1));
                                }

                            }

                        }
                        catch (const std::runtime_error &e) {

                            Assertions::fail("Exception caught: " + e.what());
                            e.printStackTrace();
                        }

                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAppendRecord()
                    void CommandSetTestSuite::testAppendRecord() {

                        try {

                            std::shared_ptr<PoFileStructureInfo> poData = TestEngine::selectPO();

                            std::vector<char> recordData(poData->getEventFileData()->getRecSize());
                            Arrays::fill(recordData, static_cast<char>(0x01));

                            updateRecord(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getEventFileData()->getSfi(), static_cast<char>(0x01), recordData);

                            std::vector<char> recordDataToAppend(poData->getEventFileData()->getRecSize());
                            Arrays::fill(recordDataToAppend, static_cast<char>(0x11));

                            appendRecord(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getEventFileData()->getSfi(), recordDataToAppend);

                            std::shared_ptr<ReadRecordsRespPars> readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getEventFileData()->getSfi(), static_cast<char>(0x01), ReadDataStructure::SINGLE_RECORD_DATA);

                            std::vector<char> firstEventRecord = readRecordsRespPars->getRecords()->get(1);

                            Assertions::assertArrayEquals(recordDataToAppend, firstEventRecord);

                            readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getEventFileData()->getSfi(), static_cast<char>(0x02), ReadDataStructure::SINGLE_RECORD_DATA);

                            std::vector<char> secondEventRecord = readRecordsRespPars->getRecords()->get(2);

                            Assertions::assertArrayEquals(recordData, secondEventRecord);

                        }
                        catch (const std::runtime_error &e) {

                            Assertions::fail("Exception caught: " + e.what());
                            e.printStackTrace();
                        }

                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testUpdateAndDecreaseCounterSingleSession()
                    void CommandSetTestSuite::testUpdateAndDecreaseCounterSingleSession() {

                        try {

                            std::shared_ptr<PoFileStructureInfo> poData = TestEngine::selectPO();

                            std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(TestEngine::poReader, std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), TestEngine::samReader, nullptr);

                            std::string genericCounterData = "00000A 000100 000B00 010000 0C0000 0000B0 00C000 0F0000 00000D 0000";

                            std::vector<char> counterData(poData->getCountersFileData()->getRecSize());

                            System::arraycopy(ByteArrayUtils::fromHex(genericCounterData), 0, counterData, 0, counterData.size());

                            poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_LOAD, static_cast<char>(0x00), static_cast<char>(0x00));

                            poTransaction->prepareUpdateRecordCmd(poData->getCountersFileData()->getSfi(), static_cast<char>(0x01), counterData, std::string::format("SFI=%02X, recnbr=1", poData->getCountersFileData()->getSfi()));

                            poTransaction->prepareDecreaseCmd(poData->getCountersFileData()->getSfi(), static_cast<char>(0x01), 0x01, std::string::format("SFI=%02X, index=1, decvalue=1", poData->getCountersFileData()->getSfi()));

                            poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);

                            counterData[2] = 0x09;

                            std::shared_ptr<ReadRecordsRespPars> readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getCountersFileData()->getSfi(), static_cast<char>(0x01), ReadDataStructure::SINGLE_RECORD_DATA);

                            std::vector<char> updatedCounterData = readRecordsRespPars->getRecords()->get(1);

                            Assertions::assertArrayEquals(counterData, updatedCounterData);

                        }
                        catch (const std::runtime_error &e) {

                            Assertions::fail("Exception caught: " + e.what());
                            e.printStackTrace();
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testUpdateAndIncreaseCounterSingleSession()
                    void CommandSetTestSuite::testUpdateAndIncreaseCounterSingleSession() {

                        try {

                            std::shared_ptr<PoFileStructureInfo> poData = TestEngine::selectPO();

                            std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(TestEngine::poReader, std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), TestEngine::samReader, nullptr);

                            std::string genericCounterData = "0A0000 000100 000B00 010000 0C0000 0000B0 00C000 0F0000 00000D 0000";

                            std::vector<char> counterData(poData->getCountersFileData()->getRecSize());

                            System::arraycopy(ByteArrayUtils::fromHex(genericCounterData), 0, counterData, 0, counterData.size());

                            poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_LOAD, static_cast<char>(0x00), static_cast<char>(0x00));

                            poTransaction->prepareUpdateRecordCmd(poData->getCountersFileData()->getSfi(), static_cast<char>(0x01), counterData, std::string::format("SFI=%02X, recnbr=1", poData->getCountersFileData()->getSfi()));

                            poTransaction->prepareIncreaseCmd(poData->getCountersFileData()->getSfi(), static_cast<char>(0x01), 0xFF, std::string::format("SFI=%02X, index=1, decvalue=255", poData->getCountersFileData()->getSfi()));

                            poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);

                            counterData[2] = static_cast<char>(0xFF);

                            std::shared_ptr<ReadRecordsRespPars> readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getCountersFileData()->getSfi(), static_cast<char>(0x01), ReadDataStructure::SINGLE_RECORD_DATA);

                            std::vector<char> updatedCounterData = readRecordsRespPars->getRecords()->get(1);

                            Assertions::assertArrayEquals(counterData, updatedCounterData);

                        }
                        catch (const std::runtime_error &e) {

                            Assertions::fail("Exception caught: " + e.what());
                            e.printStackTrace();
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testUpdateAndAppendSingleSession()
                    void CommandSetTestSuite::testUpdateAndAppendSingleSession() {

                        try {

                            std::shared_ptr<PoFileStructureInfo> poData = TestEngine::selectPO();

                            std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(TestEngine::poReader, std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), TestEngine::samReader, nullptr);

                            std::vector<char> recordData(poData->getEventFileData()->getRecSize());
                            Arrays::fill(recordData, static_cast<char>(0x01));

                            std::vector<char> recordDataToAppend(poData->getEventFileData()->getRecSize());
                            Arrays::fill(recordDataToAppend, static_cast<char>(0x11));

                            poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_LOAD, static_cast<char>(0x00), static_cast<char>(0x00));

                            poTransaction->prepareUpdateRecordCmd(poData->getEventFileData()->getSfi(), static_cast<char>(0x01), recordData, std::string::format("SFI=%02X, recnbr=1", poData->getEventFileData()->getSfi()));

                            poTransaction->prepareAppendRecordCmd(poData->getEventFileData()->getSfi(), recordDataToAppend, std::string::format("SFI=%02X", poData->getEventFileData()->getSfi()));

                            poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);

                            std::shared_ptr<ReadRecordsRespPars> readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getEventFileData()->getSfi(), static_cast<char>(0x01), ReadDataStructure::SINGLE_RECORD_DATA);

                            std::vector<char> firstEventRecord = readRecordsRespPars->getRecords()->get(1);

                            Assertions::assertArrayEquals(recordDataToAppend, firstEventRecord);

                            readRecordsRespPars = readRecords(std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), poData->getEventFileData()->getSfi(), static_cast<char>(0x02), ReadDataStructure::SINGLE_RECORD_DATA);

                            std::vector<char> secondEventRecord = readRecordsRespPars->getRecords()->get(2);

                            Assertions::assertArrayEquals(recordData, secondEventRecord);

                        }
                        catch (const std::runtime_error &e) {

                            Assertions::fail("Exception caught: " + e.what());
                            e.printStackTrace();
                        }

                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testReadDataInOpenSession()
                    void CommandSetTestSuite::testReadDataInOpenSession() {

                        try {

                            std::shared_ptr<PoFileStructureInfo> poData = TestEngine::selectPO();

                            std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(TestEngine::poReader, std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), TestEngine::samReader, nullptr);

                            std::vector<char> recordData(poData->getEventFileData()->getRecSize());
                            Arrays::fill(recordData, static_cast<char>(0xA5));

                            poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_LOAD, static_cast<char>(0x00), static_cast<char>(0x00));

                            poTransaction->prepareUpdateRecordCmd(poData->getContractFileData()->getSfi(), static_cast<char>(poData->getContractFileData()->getRecNumb()), recordData, std::string::format("SFI=%02X, recnbr=%02X", poData->getContractFileData()->getSfi(), poData->getContractFileData()->getRecNumb()));

                            poTransaction->processPoCommands(ChannelState::KEEP_OPEN);

                            poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);

                            poTransaction = std::make_shared<PoTransaction>(TestEngine::poReader, std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), TestEngine::samReader, nullptr);

                            poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT, poData->getContractFileData()->getSfi(), static_cast<char>(poData->getContractFileData()->getRecNumb()));

                            poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);

                            std::vector<char> dataReadInSession = poTransaction->getOpenRecordDataRead();

                            Assertions::assertArrayEquals(recordData, dataReadInSession);

                        }
                        catch (const std::runtime_error &e) {

                            Assertions::fail("Exception caught: " + e.what());
                            e.printStackTrace();
                        }

                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testWriteDataBeforeProcessOpening()
                    void CommandSetTestSuite::testWriteDataBeforeProcessOpening() {

                        try {

                            std::shared_ptr<PoFileStructureInfo> poData = TestEngine::selectPO();

                            std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(TestEngine::poReader, std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), TestEngine::samReader, nullptr);

                            std::vector<char> recordData(poData->getEventFileData()->getRecSize());
                            Arrays::fill(recordData, static_cast<char>(0xA9));

                            poTransaction->prepareUpdateRecordCmd(poData->getContractFileData()->getSfi(), static_cast<char>(poData->getContractFileData()->getRecNumb()), recordData, std::string::format("SFI=%02X, recnbr=%02X", poData->getContractFileData()->getSfi(), poData->getContractFileData()->getRecNumb()));

                            poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_LOAD, static_cast<char>(0x00), static_cast<char>(0x00));

                            poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);

                            poTransaction = std::make_shared<PoTransaction>(TestEngine::poReader, std::static_pointer_cast<CalypsoPo>(poData->getMatchingSe()), TestEngine::samReader, nullptr);

                            poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT, poData->getContractFileData()->getSfi(), static_cast<char>(poData->getContractFileData()->getRecNumb()));

                            poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);

                            std::vector<char> dataRead = poTransaction->getOpenRecordDataRead();

                            Assertions::assertArrayEquals(recordData, dataRead);

                        }
                        catch (const std::runtime_error &e) {

                            Assertions::fail("Exception caught: " + e.what());
                            e.printStackTrace();
                        }

                    }
                }
            }
        }
    }
}
