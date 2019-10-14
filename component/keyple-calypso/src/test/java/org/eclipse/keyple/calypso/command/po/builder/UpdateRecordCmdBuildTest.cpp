#include "UpdateRecordCmdBuildTest.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduCommandBuilder.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/PoClass.h"
#include "../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/builder/UpdateRecordCmdBuild.h"

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
//ORIGINAL LINE: @Test public void updateRecordCmdBuild_rev2_4() throws IllegalArgumentException
                            void UpdateRecordCmdBuildTest::updateRecordCmdBuild_rev2_4() throw(std::invalid_argument) {

                                // revision 2.4
                                std::vector<char> request2_4 = {static_cast<char>(0x94), static_cast<char>(0xDC), static_cast<char>(0x01), 0x44, static_cast<char>(0x05), 0x00, 0x01, 0x02, 0x03, 0x04};
                                apduCommandBuilder = std::make_shared<UpdateRecordCmdBuild>(org->eclipse.keyple.calypso.command.PoClass::LEGACY, static_cast<char>(0x08), record_number, newRecordData, "TestRev2_4");
                                org->eclipse.keyple.seproxy.message->ApduRequest = apduCommandBuilder->getApduRequest();
                                org->junit.Assert.assertArrayEquals(request2_4, org->eclipse.keyple.seproxy.message.ApduRequest::getBytes());
                            }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void updateRecordCmdBuild_rev3_1() throws IllegalArgumentException
                            void UpdateRecordCmdBuildTest::updateRecordCmdBuild_rev3_1() throw(std::invalid_argument) {
                                // revision 3.1
                                std::vector<char> request3_1 = {static_cast<char>(0x00), static_cast<char>(0xDC), static_cast<char>(0x01), 0x44, static_cast<char>(0x05), 0x00, 0x01, 0x02, 0x03, 0x04};
                                apduCommandBuilder = std::make_shared<UpdateRecordCmdBuild>(org->eclipse.keyple.calypso.command.PoClass::ISO, static_cast<char>(0x08), record_number, newRecordData, "TestRev3_1");
                                org->eclipse.keyple.seproxy.message->ApduRequest = apduCommandBuilder->getApduRequest();
                                org->junit.Assert.assertArrayEquals(request3_1, org->eclipse.keyple.seproxy.message.ApduRequest::getBytes());
                            }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void updateRecordCmdBuild_rev3_2() throws IllegalArgumentException
                            void UpdateRecordCmdBuildTest::updateRecordCmdBuild_rev3_2() throw(std::invalid_argument) {
                                // revision 3.2
                                std::vector<char> request3_2 = {static_cast<char>(0x00), static_cast<char>(0xDC), static_cast<char>(0x01), 0x44, static_cast<char>(0x05), 0x00, 0x01, 0x02, 0x03, 0x04};
                                apduCommandBuilder = std::make_shared<UpdateRecordCmdBuild>(org->eclipse.keyple.calypso.command.PoClass::ISO, static_cast<char>(0x08), record_number, newRecordData, "TestRev3_2");
                                org->eclipse.keyple.seproxy.message->ApduRequest = apduCommandBuilder->getApduRequest();
                                org->junit.Assert.assertArrayEquals(request3_2, org->eclipse.keyple.seproxy.message.ApduRequest::getBytes());
                            }
                        }
                    }
                }
            }
        }
    }
}
