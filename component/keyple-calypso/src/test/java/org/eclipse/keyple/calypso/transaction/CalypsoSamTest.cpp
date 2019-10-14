#include "CalypsoSamTest.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeSelector.h"
#include "../../../../../../../main/java/org/eclipse/keyple/calypso/transaction/sam/SamSelectionRequest.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../main/java/org/eclipse/keyple/calypso/transaction/sam/CalypsoSam.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/AnswerToReset.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SelectionStatus.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
                    using namespace org::eclipse::keyple::calypso::command::sam;
//                    import static org.eclipse.keyple.calypso.command.sam.SamRevision.*;
//                    import static org.junit.Assert.*;
                    using CalypsoSam = org::eclipse::keyple::calypso::transaction::sam::CalypsoSam;
                    using SamSelectionRequest = org::eclipse::keyple::calypso::transaction::sam::SamSelectionRequest;
                    using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                    using SeSelector = org::eclipse::keyple::seproxy::SeSelector;
                    using AnswerToReset = org::eclipse::keyple::seproxy::message::AnswerToReset;
                    using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                    using SelectionStatus = org::eclipse::keyple::seproxy::message::SelectionStatus;
                    using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                    using org::junit::Test;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_CalypsoSam_1()
                    void CalypsoSamTest::test_CalypsoSam_1() {
                        std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(nullptr, nullptr, "Dummy SeSelector");
                        std::shared_ptr<SamSelectionRequest> samSelectionRequest = std::make_shared<SamSelectionRequest>(seSelector, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4);
                        std::shared_ptr<CalypsoSam> calypsoSam = std::make_shared<CalypsoSam>(samSelectionRequest);
                        std::shared_ptr<SelectionStatus> selectionStatus = std::make_shared<SelectionStatus>(std::make_shared<AnswerToReset>(ByteArrayUtils::fromHex("3B001122805A0180D002030411223344829000")), nullptr, true);
                        calypsoSam->setSelectionResponse(std::make_shared<SeResponse>(true, selectionStatus, nullptr));
                        assertEquals(S1D, calypsoSam->getSamRevision());
                        assertEquals(static_cast<char>(0x80), calypsoSam->getApplicationType());
                        assertEquals(static_cast<char>(0xD0), calypsoSam->getApplicationSubType());
                        assertEquals(static_cast<char>(0x01), calypsoSam->getPlatform());
                        assertEquals(static_cast<char>(0x02), calypsoSam->getSoftwareIssuer());
                        assertEquals(static_cast<char>(0x03), calypsoSam->getSoftwareVersion());
                        assertEquals(static_cast<char>(0x04), calypsoSam->getSoftwareRevision());
                        assertEquals("11223344", ByteArrayUtils::toHex(calypsoSam->getSerialNumber()));
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_CalypsoSam_2()
                    void CalypsoSamTest::test_CalypsoSam_2() {
                        std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(nullptr, nullptr, "Dummy SeSelector");
                        std::shared_ptr<SamSelectionRequest> samSelectionRequest = std::make_shared<SamSelectionRequest>(seSelector, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4);
                        std::shared_ptr<CalypsoSam> calypsoSam = std::make_shared<CalypsoSam>(samSelectionRequest);
                        std::shared_ptr<SelectionStatus> selectionStatus = std::make_shared<SelectionStatus>(std::make_shared<AnswerToReset>(ByteArrayUtils::fromHex("3B001122805A0180D102030411223344829000")), nullptr, true);
                        calypsoSam->setSelectionResponse(std::make_shared<SeResponse>(true, selectionStatus, nullptr));
                        assertEquals(S1D, calypsoSam->getSamRevision());
                        assertEquals(static_cast<char>(0xD1), calypsoSam->getApplicationSubType());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_CalypsoSam_3()
                    void CalypsoSamTest::test_CalypsoSam_3() {
                        std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(nullptr, nullptr, "Dummy SeSelector");
                        std::shared_ptr<SamSelectionRequest> samSelectionRequest = std::make_shared<SamSelectionRequest>(seSelector, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4);
                        std::shared_ptr<CalypsoSam> calypsoSam = std::make_shared<CalypsoSam>(samSelectionRequest);
                        std::shared_ptr<SelectionStatus> selectionStatus = std::make_shared<SelectionStatus>(std::make_shared<AnswerToReset>(ByteArrayUtils::fromHex("3B001122805A0180D202030411223344829000")), nullptr, true);
                        calypsoSam->setSelectionResponse(std::make_shared<SeResponse>(true, selectionStatus, nullptr));
                        assertEquals(S1D, calypsoSam->getSamRevision());
                        assertEquals(static_cast<char>(0xD2), calypsoSam->getApplicationSubType());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_CalypsoSam_4()
                    void CalypsoSamTest::test_CalypsoSam_4() {
                        std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(nullptr, nullptr, "Dummy SeSelector");
                        std::shared_ptr<SamSelectionRequest> samSelectionRequest = std::make_shared<SamSelectionRequest>(seSelector, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4);
                        std::shared_ptr<CalypsoSam> calypsoSam = std::make_shared<CalypsoSam>(samSelectionRequest);
                        std::shared_ptr<SelectionStatus> selectionStatus = std::make_shared<SelectionStatus>(std::make_shared<AnswerToReset>(ByteArrayUtils::fromHex("3B001122805A0180C102030411223344829000")), nullptr, true);
                        calypsoSam->setSelectionResponse(std::make_shared<SeResponse>(true, selectionStatus, nullptr));
                        assertEquals(C1, calypsoSam->getSamRevision());
                        assertEquals(static_cast<char>(0xC1), calypsoSam->getApplicationSubType());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_CalypsoSam_5()
                    void CalypsoSamTest::test_CalypsoSam_5() {
                        std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(nullptr, nullptr, "Dummy SeSelector");
                        std::shared_ptr<SamSelectionRequest> samSelectionRequest = std::make_shared<SamSelectionRequest>(seSelector, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4);
                        std::shared_ptr<CalypsoSam> calypsoSam = std::make_shared<CalypsoSam>(samSelectionRequest);
                        std::shared_ptr<SelectionStatus> selectionStatus = std::make_shared<SelectionStatus>(std::make_shared<AnswerToReset>(ByteArrayUtils::fromHex("3B001122805A0180E102030411223344829000")), nullptr, true);
                        calypsoSam->setSelectionResponse(std::make_shared<SeResponse>(true, selectionStatus, nullptr));
                        assertEquals(S1E, calypsoSam->getSamRevision());
                        assertEquals(static_cast<char>(0xE1), calypsoSam->getApplicationSubType());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalStateException.class) public void test_CalypsoSam_6()
                    void CalypsoSamTest::test_CalypsoSam_6() {
                        std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(nullptr, nullptr, "Dummy SeSelector");
                        std::shared_ptr<SamSelectionRequest> samSelectionRequest = std::make_shared<SamSelectionRequest>(seSelector, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4);
                        std::shared_ptr<CalypsoSam> calypsoSam = std::make_shared<CalypsoSam>(samSelectionRequest);
                        std::shared_ptr<SelectionStatus> selectionStatus = std::make_shared<SelectionStatus>(std::make_shared<AnswerToReset>(ByteArrayUtils::fromHex("3B001122805A0180E202030411223344829000")), nullptr, true);
                        calypsoSam->setSelectionResponse(std::make_shared<SeResponse>(true, selectionStatus, nullptr));
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalStateException.class) public void test_CalypsoSam_7()
                    void CalypsoSamTest::test_CalypsoSam_7() {
                        std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(nullptr, nullptr, "Dummy SeSelector");
                        std::shared_ptr<SamSelectionRequest> samSelectionRequest = std::make_shared<SamSelectionRequest>(seSelector, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4);
                        std::shared_ptr<CalypsoSam> calypsoSam = std::make_shared<CalypsoSam>(samSelectionRequest);
                        std::shared_ptr<SelectionStatus> selectionStatus = std::make_shared<SelectionStatus>(std::make_shared<AnswerToReset>(ByteArrayUtils::fromHex("3B001122805A0180E202030411223344820000")), nullptr, true);
                        calypsoSam->setSelectionResponse(std::make_shared<SeResponse>(true, selectionStatus, nullptr));
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalStateException.class) public void test_CalypsoSam_8()
                    void CalypsoSamTest::test_CalypsoSam_8() {
                        std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(nullptr, nullptr, "Dummy SeSelector");
                        std::shared_ptr<SamSelectionRequest> samSelectionRequest = std::make_shared<SamSelectionRequest>(seSelector, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4);
                        std::shared_ptr<CalypsoSam> calypsoSam = std::make_shared<CalypsoSam>(samSelectionRequest);
                        std::shared_ptr<SelectionStatus> selectionStatus = std::make_shared<SelectionStatus>(std::make_shared<AnswerToReset>(ByteArrayUtils::fromHex("")), nullptr, true);
                        calypsoSam->setSelectionResponse(std::make_shared<SeResponse>(true, selectionStatus, nullptr));
                    }
                }
            }
        }
    }
}
