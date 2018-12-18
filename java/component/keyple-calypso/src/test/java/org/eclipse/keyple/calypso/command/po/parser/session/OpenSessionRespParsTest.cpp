#include "OpenSessionRespParsTest.h"
#include "../../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/parser/session/AbstractOpenSessionRespPars.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SelectionStatus.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponse.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponseSet.h"
#include "../../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/PoRevision.h"
#include "../../../../util/TestsUtilsResponseTabByteGenerator.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {
                            namespace session {
                                using PoRevision = org::eclipse::keyple::calypso::command::po::PoRevision;
                                using TestsUtilsResponseTabByteGenerator = org::eclipse::keyple::calypso::util::TestsUtilsResponseTabByteGenerator;
                                using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;
                                using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                                using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;
                                using SelectionStatus = org::eclipse::keyple::seproxy::message::SelectionStatus;
                                using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                                using org::junit::Assert;
                                using org::junit::Test;
                                using org::junit::runner::RunWith;
                                using org::mockito::junit::MockitoJUnitRunner;

                                void OpenSessionRespParsTest::check(std::shared_ptr<AbstractOpenSessionRespPars> resp) {
                                    Assert::assertTrue(resp->isSuccessful());
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testgetResponse_rev2_4()
                                void OpenSessionRespParsTest::testgetResponse_rev2_4() {

                                    // expected response

                                    std::shared_ptr<ApduResponse> responseMockOpenSecureSession = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("CC 11223344 00112233445566778899AABBCCDDEEFF 00112233445566778899AABBCC 9000"), nullptr);
                                    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
                                    apduResponses.push_back(responseMockOpenSecureSession);

                                    std::shared_ptr<SeResponseSet> responseMock = std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, responseMockOpenSecureSession, true), apduResponses));
                                    std::shared_ptr<ApduResponse> response = responseMock->getSingleResponse()->getApduResponses()[0];

                                    check(AbstractOpenSessionRespPars::create(response, PoRevision::REV2_4));
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testgetResponse_rev2_4_no_data()
                                void OpenSessionRespParsTest::testgetResponse_rev2_4_no_data() {

                                    // expected response

                                    std::shared_ptr<ApduResponse> responseMockOpenSecureSession = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("CC 11223344 9000"), nullptr);
                                    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
                                    apduResponses.push_back(responseMockOpenSecureSession);

                                    std::shared_ptr<SeResponseSet> responseMock = std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, responseMockOpenSecureSession, true), apduResponses));
                                    std::shared_ptr<ApduResponse> response = responseMock->getSingleResponse()->getApduResponses()[0];

                                    check(AbstractOpenSessionRespPars::create(response, PoRevision::REV2_4));
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testgetResponse_rev2_4_non_ratified()
                                void OpenSessionRespParsTest::testgetResponse_rev2_4_non_ratified() {

                                    // expected response

                                    std::shared_ptr<ApduResponse> responseMockOpenSecureSession = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("CC 11223344 9999 00112233445566778899AABBCCDDEEFF 00112233445566778899AABBCC 9000"), nullptr);
                                    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
                                    apduResponses.push_back(responseMockOpenSecureSession);

                                    std::shared_ptr<SeResponseSet> responseMock = std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, responseMockOpenSecureSession, true), apduResponses));
                                    std::shared_ptr<ApduResponse> response = responseMock->getSingleResponse()->getApduResponses()[0];

                                    check(AbstractOpenSessionRespPars::create(response, PoRevision::REV2_4));
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testgetResponse_rev2_4_no_data_non_ratified()
                                void OpenSessionRespParsTest::testgetResponse_rev2_4_no_data_non_ratified() {

                                    // expected response

                                    std::shared_ptr<ApduResponse> responseMockOpenSecureSession = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("CC 11223344 9999 9000"), nullptr);
                                    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
                                    apduResponses.push_back(responseMockOpenSecureSession);

                                    std::shared_ptr<SeResponseSet> responseMock = std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, responseMockOpenSecureSession, true), apduResponses));
                                    std::shared_ptr<ApduResponse> response = responseMock->getSingleResponse()->getApduResponses()[0];

                                    check(AbstractOpenSessionRespPars::create(response, PoRevision::REV2_4));
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalStateException.class) public void testgetResponse_rev2_4_bad_length_inf()
                                void OpenSessionRespParsTest::testgetResponse_rev2_4_bad_length_inf() {

                                    // expected response

                                    std::shared_ptr<ApduResponse> responseMockOpenSecureSession = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("CC 11223344 9999 00112233445566778899AABBCCDDEEFF 00112233445566778899AABBCCDDEEFF 9000"), nullptr);
                                    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
                                    apduResponses.push_back(responseMockOpenSecureSession);

                                    std::shared_ptr<SeResponseSet> responseMock = std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, responseMockOpenSecureSession, true), apduResponses));
                                    std::shared_ptr<ApduResponse> response = responseMock->getSingleResponse()->getApduResponses()[0];

                                    check(AbstractOpenSessionRespPars::create(response, PoRevision::REV2_4));
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalStateException.class) public void testgetResponse_rev2_4_bad_length_sup()
                                void OpenSessionRespParsTest::testgetResponse_rev2_4_bad_length_sup() {

                                    // expected response

                                    std::shared_ptr<ApduResponse> responseMockOpenSecureSession = std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("CC 11223344 9999 00112233445566778899AABBCCDDEEFF 9000"), nullptr);
                                    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
                                    apduResponses.push_back(responseMockOpenSecureSession);

                                    std::shared_ptr<SeResponseSet> responseMock = std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, responseMockOpenSecureSession, true), apduResponses));
                                    std::shared_ptr<ApduResponse> response = responseMock->getSingleResponse()->getApduResponses()[0];

                                    check(AbstractOpenSessionRespPars::create(response, PoRevision::REV2_4));
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testgetResponse_rev3_1()
                                void OpenSessionRespParsTest::testgetResponse_rev3_1() {

                                    // expected response

                                    std::shared_ptr<ApduResponse> responseMockFci = TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev3_1();
                                    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
                                    apduResponses.push_back(responseMockFci);

                                    std::shared_ptr<SeResponseSet> responseMock = std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, responseMockFci, true), apduResponses));
                                    std::shared_ptr<ApduResponse> response = responseMock->getSingleResponse()->getApduResponses()[0];

                                    check(AbstractOpenSessionRespPars::create(response, PoRevision::REV3_1));
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testgetResponse_rev3_2()
                                void OpenSessionRespParsTest::testgetResponse_rev3_2() {

                                    // expected response

                                    std::shared_ptr<ApduResponse> responseMockOS = TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev3_2();
                                    std::shared_ptr<ApduResponse> responseMockFci = TestsUtilsResponseTabByteGenerator::generateApduResponseValidRev3_2();
                                    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
                                    apduResponses.push_back(responseMockOS);

                                    std::shared_ptr<SeResponseSet> responseMock = std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, responseMockFci, true), apduResponses));
                                    std::shared_ptr<ApduResponse> response = responseMock->getSingleResponse()->getApduResponses()[0];

                                    check(AbstractOpenSessionRespPars::create(response, PoRevision::REV3_2));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
