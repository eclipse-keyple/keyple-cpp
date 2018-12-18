#include "GetDataCmdBuildTest.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/PoClass.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/builder/GetDataFciCmdBuild.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduCommandBuilder.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                            using AbstractApduCommandBuilder = org::eclipse::keyple::command::AbstractApduCommandBuilder;
                            using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                            using org::junit::Assert;
                            using org::junit::Test;
                            using org::junit::runner::RunWith;
                            using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getDataFCICmdBuild()
                            void GetDataCmdBuildTest::getDataFCICmdBuild() {
                                std::vector<char> request = {static_cast<char>(0x94), static_cast<char>(0xCA), static_cast<char>(0x00), 0x6F, 0x00};
                                std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder = std::make_shared<GetDataFciCmdBuild>(PoClass::LEGACY);
                                std::shared_ptr<ApduRequest> apduReq = apduCommandBuilder->getApduRequest();
                                Assert::assertArrayEquals(request, apduReq->getBytes());
                            }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getDataFCICmdBuild2()
                            void GetDataCmdBuildTest::getDataFCICmdBuild2() {
                                std::vector<char> request2 = {static_cast<char>(0x00), static_cast<char>(0xCA), static_cast<char>(0x00), 0x6F, 0x00};
                                std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder = std::make_shared<GetDataFciCmdBuild>(PoClass::ISO);
                                std::shared_ptr<ApduRequest> apduReq = apduCommandBuilder->getApduRequest();
                                Assert::assertArrayEquals(request2, apduReq->getBytes());
                            }
                        }
                    }
                }
            }
        }
    }
}
