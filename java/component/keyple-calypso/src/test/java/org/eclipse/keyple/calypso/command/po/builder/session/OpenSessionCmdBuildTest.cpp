#include "OpenSessionCmdBuildTest.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/command/AbstractApduCommandBuilder.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"
#include "../../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/PoRevision.h"
#include "../../../../../../../../../../main/java/org/eclipse/keyple/calypso/command/po/builder/session/AbstractOpenSessionCmdBuild.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            namespace session {
                                using PoRevision = org::eclipse::keyple::calypso::command::po::PoRevision;
                                using AbstractApduCommandBuilder = org::eclipse::keyple::command::AbstractApduCommandBuilder;
                                using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                                using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                                using org::junit::Assert;
                                using org::junit::Test;
                                using org::junit::runner::RunWith;
                                using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void openSessionCmdBuild_rev_2_4_exception() throws IllegalArgumentException
                                void OpenSessionCmdBuildTest::openSessionCmdBuild_rev_2_4_exception() throw(std::invalid_argument) {
                                    char keyIndex = static_cast<char>(0x00);
                                    char recordNumberToRead = static_cast<char>(0x01);
                                    char sfiToSelect = static_cast<char>(0x08);


                                    apduCommandBuilder = AbstractOpenSessionCmdBuild::create(PoRevision::REV2_4, keyIndex, samChallenge, sfiToSelect, recordNumberToRead, "");
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void openSessionCmdBuild_rev_2_4() throws IllegalArgumentException
                                void OpenSessionCmdBuildTest::openSessionCmdBuild_rev_2_4() throw(std::invalid_argument) {
                                    char keyIndex = static_cast<char>(0x03);
                                    char recordNumberToRead = static_cast<char>(0x01);
                                    char sfiToSelect = static_cast<char>(0x08);
                                    char cla = static_cast<char>(0x94);
                                    char p1 = static_cast<char>(0x80 + (recordNumberToRead * 8) + keyIndex);
                                    char p2 = static_cast<char>(sfiToSelect * 8);
                                    char cmd = static_cast<char>(0x8A);
                                    std::vector<char> dataIn = samChallenge;
                                    // revision 2.4
                                    char le = 0; // case 4
                                    std::vector<char> request2_4 = {cla, cmd, p1, p2, static_cast<char>(dataIn.size()), static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), 0x3E, le};

                                    apduCommandBuilder = AbstractOpenSessionCmdBuild::create(PoRevision::REV2_4, keyIndex, dataIn, sfiToSelect, recordNumberToRead, "");
                                    apduRequest = apduCommandBuilder->getApduRequest();
                                    Assert::assertArrayEquals(request2_4, apduRequest->getBytes());
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void openSessionCmdBuild_rev_3_1() throws IllegalArgumentException
                                void OpenSessionCmdBuildTest::openSessionCmdBuild_rev_3_1() throw(std::invalid_argument) {
                                    char keyIndex = static_cast<char>(0x03);
                                    char recordNumberToRead = static_cast<char>(0x01);
                                    char sfiToSelect = static_cast<char>(0x08);
                                    char cla = static_cast<char>(0x00);
                                    char p1 = static_cast<char>((recordNumberToRead * 8) + keyIndex);
                                    char p2 = static_cast<char>((sfiToSelect * 8) + 1);
                                    char cmd = static_cast<char>(0x8A);
                                    std::vector<char> dataIn = samChallenge;
                                    char le = 0; // case 4

                                    // revision 3.1
                                    std::vector<char> request3_1 = {cla, cmd, p1, p2, static_cast<char>(dataIn.size()), static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), 0x3E, le};
                                    apduCommandBuilder = AbstractOpenSessionCmdBuild::create(PoRevision::REV3_1, keyIndex, dataIn, sfiToSelect, recordNumberToRead, "");
                                    apduRequest = apduCommandBuilder->getApduRequest();
                                    Assert::assertArrayEquals(request3_1, apduRequest->getBytes());
                                }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void openSessionCmdBuild_rev_3_2() throws IllegalArgumentException
                                void OpenSessionCmdBuildTest::openSessionCmdBuild_rev_3_2() throw(std::invalid_argument) {
                                    char keyIndex = static_cast<char>(0x03);
                                    char recordNumberToRead = static_cast<char>(0x01);
                                    char sfiToSelect = static_cast<char>(0x08);
                                    char cla = static_cast<char>(0x00);
                                    char p1 = static_cast<char>((recordNumberToRead * 8) + keyIndex);
                                    char p2 = static_cast<char>((sfiToSelect * 8) + 2);
                                    char cmd = static_cast<char>(0x8A);
                                    std::vector<char> dataIn(samChallenge.size() + 1);
                                    System::arraycopy(samChallenge, 0, dataIn, 1, samChallenge.size());
                                    char le = 0; // case 4
                                    // revision 3.2
                                    std::vector<char> request3_2 = {cla, cmd, p1, p2, static_cast<char>(samChallenge.size() + 1), static_cast<char>(0x00), static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), 0x3E, le};
                                    apduCommandBuilder = AbstractOpenSessionCmdBuild::create(PoRevision::REV3_2, keyIndex, samChallenge, sfiToSelect, recordNumberToRead, "");
                                    apduRequest = apduCommandBuilder->getApduRequest();
                                    Assert::assertEquals(ByteArrayUtils::toHex(request3_2), ByteArrayUtils::toHex(apduRequest->getBytes()));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
