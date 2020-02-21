#include "PoGetChallengeRespParsTest.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "SeResponseSet.h"
#include "ByteArrayUtil.h"
#include "PoGetChallengeRespPars.h"
#include "AbstractApduResponseParser_Import.h"

using namespace keyple::calypso::command::po::parser::security;

        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {
                            namespace security {
                                using AbstractApduResponseParser = keyple::core::command::AbstractApduResponseParser;
                                using ApduResponse = keyple::core::seproxy::message::ApduResponse;
                                using SeResponse = keyple::core::seproxy::message::SeResponse;
                                using SeResponseSet = keyple::core::seproxy::message::SeResponseSet;
                                using SelectionStatus = keyple::core::seproxy::message::SelectionStatus;
                                using ByteArrayUtils = keyple::core::util::ByteArrayUtil;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void POGetChallengetRespPars()
                                void PoGetChallengeRespParsTest::POGetChallengetRespPars() {
                                    std::vector<char> response = {0x03, 0x0D, 0x0E, static_cast<char>(0xFA), static_cast<char>(0x9C), static_cast<char>(0x8C), static_cast<char>(0xB7), 0x27, static_cast<char>(0x90), 0x00};
                                    std::vector<std::shared_ptr<ApduResponse>> responses;
                                    std::shared_ptr<ApduResponse> apduResponse = std::make_shared<ApduResponse>(response, nullptr);
                                    responses.push_back(apduResponse);
                                    std::vector<char> cResp1 = ByteArrayUtils::fromHex("9000");
                                    std::shared_ptr<SeResponseSet> seResponse = std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(true, true, std::make_shared<SelectionStatus>(nullptr, std::make_shared<ApduResponse>(cResp1, nullptr), true), responses));

                                    std::shared_ptr<AbstractApduResponseParser> apduResponseParser = std::make_shared<PoGetChallengeRespPars>(seResponse->getSingleResponse()->getApduResponses()[0]);
                                    ASSERT_EQ(ByteArrayUtils::toHex(response), ByteArrayUtils::toHex(apduResponseParser->getApduResponse()->getBytes()));
                                    ASSERT_EQ("Success", apduResponseParser->getStatusInformation());
                                }
                            }
                        }
                    }
                }
            }
        }
