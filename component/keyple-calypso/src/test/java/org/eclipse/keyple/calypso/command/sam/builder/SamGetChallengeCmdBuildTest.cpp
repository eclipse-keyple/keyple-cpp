#include "SamGetChallengeCmdBuildTest.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/sam/SamRevision.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/sam/builder/session/SamGetChallengeCmdBuild.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduCommandBuilder.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace builder {
                            using SamRevision = org::eclipse::keyple::calypso::command::sam::SamRevision;
                            using SamGetChallengeCmdBuild = org::eclipse::keyple::calypso::command::sam::builder::session::SamGetChallengeCmdBuild;
                            using AbstractApduCommandBuilder = org::eclipse::keyple::command::AbstractApduCommandBuilder;
                            using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                            using org::junit::Assert;
                            using org::junit::Test;
                            using org::junit::runner::RunWith;
                            using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getChallengeCmdBuild() throws IllegalArgumentException
                            void SamGetChallengeCmdBuildTest::getChallengeCmdBuild() throw(std::invalid_argument) {
                                std::vector<char> request = {static_cast<char>(0x94), static_cast<char>(0x84), 0x00, 0x00, 0x04};

                                std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder = std::make_shared<SamGetChallengeCmdBuild>(SamRevision::S1D, static_cast<char>(0x04)); // 94
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
