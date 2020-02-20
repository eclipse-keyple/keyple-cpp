#include "SelectDiversifierRespParsTest.h"
#include "ApduResponse.h"
#include "SelectDiversifierRespPars.h"
#include "AbstractApduResponseParser_Import.h"

        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace parser {
                            using SelectDiversifierRespPars = keyple::calypso::command::sam::parser::security::SelectDiversifierRespPars;
                            using AbstractApduResponseParser = keyple::core::command::AbstractApduResponseParser;
                            using ApduResponse = keyple::core::seproxy::message::ApduResponse;
                            //using org::hamcrest::core::IsNot;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void selectDiversifierResp()
                            void SelectDiversifierRespParsTest::selectDiversifierResp() {
                                // We check here that the value returned by getApduResponse matches the value provided at
                                // construct time
                                std::vector<char> cResp = {static_cast<char>(0x90), 0x00};
                                std::vector<char> cResp2 = {static_cast<char>(0x80), 0x00};
                                std::shared_ptr<ApduResponse> apduResponse = std::make_shared<ApduResponse>(cResp, nullptr);
                                std::shared_ptr<ApduResponse> apduResponse1 = std::make_shared<ApduResponse>(cResp2, nullptr);

                                std::shared_ptr<AbstractApduResponseParser> apduResponseParser = std::make_shared<SelectDiversifierRespPars>(apduResponse);
                                ASSERT_EQ(0x9000, apduResponseParser->getApduResponse()->getStatusCode());

                                apduResponseParser = std::make_shared<SelectDiversifierRespPars>(apduResponse1);
                                ASSERT_NE(apduResponseParser->getApduResponse()->getStatusCode(), 0x9000);
                            }
                        }
                    }
                }
            }
        }
