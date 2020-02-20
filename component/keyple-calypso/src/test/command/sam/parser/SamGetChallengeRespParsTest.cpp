#include "SamGetChallengeRespParsTest.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "SeResponseSet.h"
#include "ByteArrayUtil.h"
#include "SamGetChallengeRespPars.h"
#include "AbstractApduResponseParser_Import.h"

using namespace keyple::calypso::command::sam::parser::security;

        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace parser {
                            namespace security {
                            using SamGetChallengeRespPars = keyple::calypso::command::sam::parser::security::SamGetChallengeRespPars;
                            using AbstractApduResponseParser = keyple::core::command::AbstractApduResponseParser;
                            using ApduResponse = keyple::core::seproxy::message::ApduResponse;
                            using SeResponse = keyple::core::seproxy::message::SeResponse;
                            using SeResponseSet = keyple::core::seproxy::message::SeResponseSet;
                            using SelectionStatus = keyple::core::seproxy::message::SelectionStatus;
                            using ByteArrayUtils = keyple::core::util::ByteArrayUtil;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getChallengeRespPars()
                            void SamGetChallengeRespParsTest::getChallengeRespPars() {
                                std::vector<std::shared_ptr<ApduResponse>> responses;
                                std::vector<char> cRep1 = std::vector<char> {static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), 0x3E, static_cast<char>(0x90), 0x00};
                                std::shared_ptr<ApduResponse> apduResponse = std::make_shared<ApduResponse>(cRep1, nullptr);
                                responses.push_back(apduResponse);
                                std::vector<char> cRep2 = ByteArrayUtils::fromHex("9000");
                                std::shared_ptr<SeResponseSet> seResponse = std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(true, true, std::make_shared<SelectionStatus>(nullptr, std::make_shared<ApduResponse>(cRep2, nullptr), true), responses));

                                std::shared_ptr<AbstractApduResponseParser> apduResponseParser = std::make_shared<SamGetChallengeRespPars>(seResponse->getSingleResponse()->getApduResponses()[0]);
                                std::vector<char> responseActual = apduResponseParser->getApduResponse()->getBytes();
                                std::vector<char> cRep = {static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), 0x3E, static_cast<char>(0x90), 0x00};
                                ASSERT_EQ(ByteArrayUtils::toHex(cRep), ByteArrayUtils::toHex(responseActual));
                                }
                            }
                        }
                    }
                }
            }
        }

TEST(SamGetChallengeRespParsTest, testA) 
{
    std::shared_ptr<SamGetChallengeRespParsTest> LocalTest = std::make_shared<SamGetChallengeRespParsTest>();
    LocalTest->getChallengeRespPars();
}
