#include "DigestUpdateRespParsTest.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SelectionStatus.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponse.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponseSet.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/sam/parser/session/DigestUpdateRespPars.h"
#include "AbstractApduResponseParser_Import.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace parser {
                            using DigestUpdateRespPars = org::eclipse::keyple::calypso::command::sam::parser::session::DigestUpdateRespPars;
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
//ORIGINAL LINE: @Test public void digestUpdateRespPars()
                            void DigestUpdateRespParsTest::digestUpdateRespPars() {
                                std::vector<std::shared_ptr<ApduResponse>> responses;
                                std::shared_ptr<ApduResponse> apduResponse = std::make_shared<ApduResponse>(std::vector<char> {90, 00}, nullptr);
                                responses.push_back(apduResponse);
                                std::shared_ptr<SeResponseSet> seResponse = std::make_shared<SeResponseSet>(std::make_shared<SeResponse>(true, std::make_shared<SelectionStatus>(nullptr, std::make_shared<ApduResponse>(ByteArrayUtils::fromHex("9000"), nullptr), true), responses));

                                std::shared_ptr<AbstractApduResponseParser> apduResponseParser = std::make_shared<DigestUpdateRespPars>(seResponse->getSingleResponse()->getApduResponses()[0]);
                                std::vector<char> responseActual = apduResponseParser->getApduResponse()->getBytes();
                                Assert::assertArrayEquals(std::vector<char> {90, 0}, responseActual);
                            }
                        }
                    }
                }
            }
        }
    }
}
