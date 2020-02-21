#include "UpdateRecordRespParsTest.h"
#include "ApduResponse.h"
#include "SelectionStatus.h"
#include "SeResponse.h"
#include "SeResponseSet.h"
#include "ByteArrayUtil.h"
#include "UpdateRecordRespPars.h"
#include "AbstractApduResponseParser_Import.h"

using namespace keyple::calypso::command::po::parser;

        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {
                            using AbstractApduResponseParser = keyple::calypso::command::po::AbstractApduResponseParser;
                            using ApduResponse = keyple::core::seproxy::message::ApduResponse;
                            using SeResponse = keyple::core::seproxy::message::SeResponse;
                            using SeResponseSet = keyple::core::seproxy::message::SeResponseSet;
                            using SelectionStatus = keyple::core::seproxy::message::SelectionStatus;
                            using ByteArrayUtils = keyple::core::util::ByteArrayUtil;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void updateRecordRespPars()
                            void UpdateRecordRespParsTest::updateRecordRespPars() {
                                std::vector<std::shared_ptr<ApduResponse>> responses;
                                std::vector<char> ApduRequest = {90, 0};
                                std::shared_ptr<ApduResponse> apduResponse = std::make_shared<ApduResponse>(ApduRequest, nullptr);
                                responses.push_back(apduResponse);
                                std::vector<char> Apdu1 = ByteArrayUtils::fromHex("9000");
                                std::shared_ptr<SeResponseSet> seResponse = std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(true, true, std::make_shared<SelectionStatus>(nullptr, std::make_shared<ApduResponse>(Apdu1, nullptr), true), responses));

                                std::shared_ptr<AbstractApduResponseParser> apduResponseParser = std::make_shared<UpdateRecordRespPars>(apduResponse);
                                apduResponseParser->setApduResponse(seResponse->getSingleResponse()->getApduResponses()[0]);
                                ASSERT_EQ(ByteArrayUtils::toHex(ApduRequest), ByteArrayUtils::toHex(apduResponseParser->getApduResponse()->getBytes()));
                            }
                        }
                    }
                }
            }
        }

TEST(UpdateRecordRespParsTest, testA) 
{
    std::shared_ptr<UpdateRecordRespParsTest> LocalTest = std::make_shared<UpdateRecordRespParsTest>();
    LocalTest->updateRecordRespPars();
}
