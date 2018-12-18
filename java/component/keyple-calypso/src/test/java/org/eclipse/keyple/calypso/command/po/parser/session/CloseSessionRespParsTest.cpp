#include "CloseSessionRespParsTest.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SelectionStatus.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponse.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponseSet.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/parser/session/CloseSessionRespPars.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduResponseParser.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {
                            namespace session {
                                using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                                using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;
                                using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;
                                using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;
                                using SelectionStatus = org::eclipse::keyple::seproxy::message::SelectionStatus;
                                using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                                using org::junit::Assert;
                                using org::junit::Test;
                                using org::junit::runner::RunWith;
                                using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void closeSessionRespPars()
                                void CloseSessionRespParsTest::closeSessionRespPars() {
                                    std::vector<char> response = {0x4D, static_cast<char>(0xBD), static_cast<char>(0xC9), 0x60, static_cast<char>(0x90), 0x00};
                                    std::vector<std::shared_ptr<ApduResponse>> responses;
                                    std::shared_ptr<ApduResponse> apduResponse = std::make_shared<ApduResponse>(response, nullptr);
                                    responses.push_back(apduResponse);
                                    std::shared_ptr<SeResponseSet> seResponse = std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("9000"), nullptr), true), responses));

                                    std::shared_ptr<AbstractApduResponseParser> apduResponseParser = std::make_shared<CloseSessionRespPars>(seResponse->getSingleResponse()->getApduResponses()[0]);
                                    Assert::assertArrayEquals(response, apduResponseParser->getApduResponse()->getBytes());
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void TestToPOHalfSessionSignature()
                                void CloseSessionRespParsTest::TestToPOHalfSessionSignature() {

                                    std::vector<char> apduResponse = {static_cast<char>(0x4D), static_cast<char>(0xBD), static_cast<char>(0xC9), 0x60, static_cast<char>(0x90), 0x00};
                                    std::vector<char> apduResponseCaseTwo = {static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), 0x3E, static_cast<char>(0xA7), 0x21, static_cast<char>(0xC2), 0x2E, static_cast<char>(0x90), 0x00};
                                    std::vector<char> apduResponseCaseThree = {static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), static_cast<char>(0x90), 0x00};

                                    std::vector<char> sessionSignature = {static_cast<char>(0x4D), static_cast<char>(0xBD), static_cast<char>(0xC9), 0x60};
                                    std::vector<char> sessionSignatureCaseTwo = {static_cast<char>(0xA7), 0x21, static_cast<char>(0xC2), 0x2E};

                                    { // Case Length = 4
                                        std::shared_ptr<CloseSessionRespPars> pars = std::make_shared<CloseSessionRespPars>(std::make_shared<ApduResponse>(apduResponse, nullptr));
                                        Assert::assertArrayEquals(sessionSignature, pars->getSignatureLo());
                                    }

                                    { // Case Length = 8
                                        std::shared_ptr<CloseSessionRespPars> pars = std::make_shared<CloseSessionRespPars>(std::make_shared<ApduResponse>(apduResponseCaseTwo, nullptr));
                                        Assert::assertArrayEquals(sessionSignatureCaseTwo, pars->getSignatureLo());
                                    }

                                    { // Case Other
                                        try {
                                            std::shared_ptr<CloseSessionRespPars> pars = std::make_shared<CloseSessionRespPars>(std::make_shared<ApduResponse>(apduResponseCaseThree, nullptr));
                                            Assert::fail();
                                        }
                                        catch (const std::invalid_argument &ex) {
                                            /* expected case */
                                        }
                                    }
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void existingTestConverted()
                                void CloseSessionRespParsTest::existingTestConverted() {
                                    std::shared_ptr<CloseSessionRespPars> parser = std::make_shared<CloseSessionRespPars>(std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("9000h"), nullptr));
                                    // This assert wasn't passing
                                    Assert::assertEquals("", ByteArrayUtils::toHex(parser->getSignatureLo()));
                                    Assert::assertEquals("", ByteArrayUtils::toHex(parser->getPostponedData()));
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void abortingASession()
                                void CloseSessionRespParsTest::abortingASession() {
                                    std::shared_ptr<CloseSessionRespPars> parser = std::make_shared<CloseSessionRespPars>(std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("FEDCBA98 9000h"), nullptr));
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void lc4withoutPostponedData()
                                void CloseSessionRespParsTest::lc4withoutPostponedData() {
                                    std::shared_ptr<CloseSessionRespPars> parser = std::make_shared<CloseSessionRespPars>(std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("FEDCBA98 9000h"), nullptr));
                                    Assert::assertEquals("FEDCBA98", ByteArrayUtils::toHex(parser->getSignatureLo()));
                                    Assert::assertEquals("", ByteArrayUtils::toHex(parser->getPostponedData()));
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void lc4WithPostponedData()
                                void CloseSessionRespParsTest::lc4WithPostponedData() {
                                    std::shared_ptr<CloseSessionRespPars> parser = std::make_shared<CloseSessionRespPars>(std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("04 345678 FEDCBA98 9000h"), nullptr));
                                    Assert::assertEquals("FEDCBA98", ByteArrayUtils::toHex(parser->getSignatureLo()));
                                    Assert::assertEquals("04345678", ByteArrayUtils::toHex(parser->getPostponedData()));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
