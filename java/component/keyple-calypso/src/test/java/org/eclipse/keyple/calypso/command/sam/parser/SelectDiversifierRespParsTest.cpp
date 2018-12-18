#include "SelectDiversifierRespParsTest.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/sam/parser/session/SelectDiversifierRespPars.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduResponseParser.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace parser {
                            using SelectDiversifierRespPars = org::eclipse::keyple::calypso::command::sam::parser::session::SelectDiversifierRespPars;
                            using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                            using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;
                            using org::hamcrest::core::IsNot;
                            using org::junit::Assert;
                            using org::junit::Test;
                            using org::junit::runner::RunWith;
                            using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void selectDiversifierResp()
                            void SelectDiversifierRespParsTest::selectDiversifierResp() {
                                // We check here that the value returned by getApduResponse matches the value provided at
                                // construct time
                                std::shared_ptr<ApduResponse> apduResponse = std::make_shared<ApduResponse>(std::vector<char> {static_cast<char>(0x90), 0x00}, nullptr);
                                std::shared_ptr<ApduResponse> apduResponse1 = std::make_shared<ApduResponse>(std::vector<char> {static_cast<char>(0x80), 0x00}, nullptr);

                                std::shared_ptr<AbstractApduResponseParser> apduResponseParser = std::make_shared<SelectDiversifierRespPars>(apduResponse);

                                Assert::assertEquals(0x9000, apduResponseParser->getApduResponse()->getStatusCode());

                                apduResponseParser = std::make_shared<SelectDiversifierRespPars>(apduResponse1);

                                Assert::assertThat(apduResponseParser->getApduResponse()->getStatusCode(), IsNot::not(0x9000));
                            }
                        }
                    }
                }
            }
        }
    }
}
