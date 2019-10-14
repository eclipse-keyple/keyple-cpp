#include "CloseSessionCmdBuildTest.h"
#include "../../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/PoClass.h"
#include "../../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/builder/session/CloseSessionCmdBuild.h"
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
//ORIGINAL LINE: @Test public void closeSessionCmdBuild() throws IllegalArgumentException
                                void CloseSessionCmdBuildTest::closeSessionCmdBuild() throw(std::invalid_argument) {
                                    std::vector<char> request2_4 = {static_cast<char>(0x94), static_cast<char>(0x8E), 0x00, 0x00, static_cast<char>(0x04), static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), 0x3E, 0x00};
                                    std::vector<char> request3_1 = {static_cast<char>(0x00), static_cast<char>(0x8E), static_cast<char>(0x80), 0x00, static_cast<char>(0x04), static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), 0x3E, 0x00};
                                    std::vector<char> terminalSessionSignature = {static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), 0x3E};
                                    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder = std::make_shared<CloseSessionCmdBuild>(PoClass::LEGACY, false, terminalSessionSignature);
                                    std::shared_ptr<ApduRequest> reqApdu = apduCommandBuilder->getApduRequest();

                                    Assert::assertArrayEquals(request2_4, reqApdu->getBytes());

                                    apduCommandBuilder = std::make_shared<CloseSessionCmdBuild>(PoClass::ISO, true, terminalSessionSignature);
                                    reqApdu = apduCommandBuilder->getApduRequest();

                                    Assert::assertArrayEquals(request3_1, reqApdu->getBytes());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
