#include "PoGetChallengeCmdBuildTest.h"
#include "../../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/PoClass.h"
#include "../../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/builder/session/PoGetChallengeCmdBuild.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduCommandBuilder.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            namespace session {
                                using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                                using AbstractApduCommandBuilder = org::eclipse::keyple::command::AbstractApduCommandBuilder;
                                using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                                using org::junit::Assert;
                                using org::junit::Test;
                                using org::junit::runner::RunWith;
                                using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void POGetChallenge_Rev2_4()
                                void PoGetChallengeCmdBuildTest::POGetChallenge_Rev2_4() {

                                    std::vector<char> request = {static_cast<char>(0x94), static_cast<char>(0x84), 0x01, 0x10, 0x08};

                                    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder = std::make_shared<PoGetChallengeCmdBuild>(PoClass::LEGACY);
                                    std::shared_ptr<ApduRequest> apduRequest = apduCommandBuilder->getApduRequest();

                                    Assert::assertArrayEquals(request, apduRequest->getBytes());

                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void POGetChallenge_Rev3_1()
                                void PoGetChallengeCmdBuildTest::POGetChallenge_Rev3_1() {

                                    std::vector<char> request = {static_cast<char>(0x00), static_cast<char>(0x84), 0x01, 0x10, 0x08};

                                    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder = std::make_shared<PoGetChallengeCmdBuild>(PoClass::ISO);
                                    std::shared_ptr<ApduRequest> apduRequest = apduCommandBuilder->getApduRequest();

                                    Assert::assertArrayEquals(request, apduRequest->getBytes());

                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void POGetChallenge_Rev3_2()
                                void PoGetChallengeCmdBuildTest::POGetChallenge_Rev3_2() {

                                    std::vector<char> request = {static_cast<char>(0x00), static_cast<char>(0x84), 0x01, 0x10, 0x08};

                                    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder = std::make_shared<PoGetChallengeCmdBuild>(PoClass::ISO);
                                    std::shared_ptr<ApduRequest> apduRequest = apduCommandBuilder->getApduRequest();

                                    Assert::assertArrayEquals(request, apduRequest->getBytes());

                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
