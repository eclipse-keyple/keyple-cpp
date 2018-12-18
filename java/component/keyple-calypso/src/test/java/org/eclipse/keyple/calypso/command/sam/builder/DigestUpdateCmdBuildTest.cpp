#include "DigestUpdateCmdBuildTest.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/sam/SamRevision.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/sam/builder/session/DigestUpdateCmdBuild.h"
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
                            using DigestUpdateCmdBuild = org::eclipse::keyple::calypso::command::sam::builder::session::DigestUpdateCmdBuild;
                            using AbstractApduCommandBuilder = org::eclipse::keyple::command::AbstractApduCommandBuilder;
                            using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                            using org::junit::Assert;
                            using org::junit::Test;
                            using org::junit::runner::RunWith;
                            using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void digestUpdateCmdBuild() throws IllegalArgumentException
                            void DigestUpdateCmdBuildTest::digestUpdateCmdBuild() throw(std::invalid_argument) {
                                std::vector<char> digestDAta = {static_cast<char>(0x94), static_cast<char>(0xAE), 0x01, 0x02};
                                std::vector<char> request = {static_cast<char>(0x94), static_cast<char>(0x8C), 0x00, static_cast<char>(0x80), static_cast<char>(digestDAta.size()), static_cast<char>(0x94), static_cast<char>(0xAE), 0x01, 0x02};

                                std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder = std::make_shared<DigestUpdateCmdBuild>(SamRevision::S1D, true, digestDAta);
                                ApduRequest ApduRequest = apduCommandBuilder->getApduRequest();

                                Assert::assertArrayEquals(request, ApduRequest::getBytes());

                                std::vector<char> request2 = {static_cast<char>(0x80), static_cast<char>(0x8C), 0x00, static_cast<char>(0x80), static_cast<char>(digestDAta.size()), static_cast<char>(0x94), static_cast<char>(0xAE), 0x01, 0x02};

                                std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder2 = std::make_shared<DigestUpdateCmdBuild>(SamRevision::C1, true, digestDAta);
                                std::shared_ptr<ApduRequest> apduReq = apduCommandBuilder2->getApduRequest();
                                Assert::assertArrayEquals(request2, apduReq->getBytes());
                            }
                        }
                    }
                }
            }
        }
    }
}
