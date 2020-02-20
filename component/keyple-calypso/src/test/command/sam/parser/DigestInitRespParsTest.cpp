#include "DigestInitRespParsTest.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "SeResponseSet.h"
#include "ByteArrayUtil.h"
#include "DigestInitRespPars.h"
#include "AbstractApduResponseParser_Import.h"

using namespace keyple::calypso::command::sam::parser::security;

        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace parser {
                            namespace security {

                            using DigestInitRespPars = keyple::calypso::command::sam::parser::security::DigestInitRespPars;
                            using AbstractApduResponseParser = keyple::core::command::AbstractApduResponseParser;
                            using ApduResponse = keyple::core::seproxy::message::ApduResponse;
                            using SeResponse = keyple::core::seproxy::message::SeResponse;
                            using SeResponseSet = keyple::core::seproxy::message::SeResponseSet;
                            using SelectionStatus = keyple::core::seproxy::message::SelectionStatus;
                            using ByteArrayUtils = keyple::core::util::ByteArrayUtil;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void digestInitRespPars()
                            void DigestInitRespParsTest::digestInitRespPars() {
                                std::vector<std::shared_ptr<ApduResponse>> responses;
                                std::vector<char> cResp1 = {static_cast<char>(0x90), 0x00};
                                std::shared_ptr<ApduResponse> apduResponse = std::make_shared<ApduResponse>(cResp1, nullptr);
                                responses.push_back(apduResponse);
                                std::vector<char> cResp2 = ByteArrayUtils::fromHex("9000");
                                std::shared_ptr<SeResponseSet> seResponse = std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(true, true, std::make_shared<SelectionStatus>(nullptr, std::make_shared<ApduResponse>(cResp2, nullptr), true), responses));

                                std::shared_ptr<AbstractApduResponseParser> apduResponseParser = std::make_shared<DigestInitRespPars>(seResponse->getSingleResponse()->getApduResponses()[0]);
                                std::vector<char> cResp = {static_cast<char>(0x90), 0x00};
                                ASSERT_EQ(ByteArrayUtils::toHex(cResp), ByteArrayUtils::toHex(apduResponseParser->getApduResponse()->getBytes()));
                                }
                            }
                        }
                    }
                }
            }
        }

TEST(DigestInitRespParsTest, testA) 
{
    std::shared_ptr<DigestInitRespParsTest> LocalTest = std::make_shared<DigestInitRespParsTest>();
    LocalTest->digestInitRespPars();
}
