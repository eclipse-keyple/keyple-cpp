#include "OpenSessionCmdBuildTest.h"

using namespace keyple::calypso::command::po::builder::security;
using namespace keyple::calypso::command::po::parser::security;

        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            namespace security {

                                using PoRevision = keyple::calypso::command::po::PoRevision;
                                using AbstractApduCommandBuilder = keyple::core::command::AbstractApduCommandBuilder;
                                using ApduRequest = keyple::core::seproxy::message::ApduRequest;
                                using ByteArrayUtil = keyple::core::util::ByteArrayUtil;

                                void OpenSessionCmdBuildTest::openSessionCmdBuild_rev_2_4_exception() {
                                    char keyIndex = static_cast<char>(0x00);
                                    char recordNumberToRead = static_cast<char>(0x01);
                                    char sfiToSelect = static_cast<char>(0x08);
                                    try
                                    {
                                        apduCommandBuilder = AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(PoRevision::REV2_4, keyIndex, samChallenge, sfiToSelect, recordNumberToRead, "");
                                    }
                                    catch(...)
                                    {
                                    }                                   
                                }

                                void OpenSessionCmdBuildTest::openSessionCmdBuild_rev_2_4() {
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

                                    apduCommandBuilder = AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(PoRevision::REV2_4, keyIndex, dataIn, sfiToSelect, recordNumberToRead, "");
                                    apduRequest = apduCommandBuilder->getApduRequest();
                                    ASSERT_EQ(request2_4, apduRequest->getBytes());
                                }

                                void OpenSessionCmdBuildTest::openSessionCmdBuild_rev_3_1() {
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
                                    apduCommandBuilder = AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(PoRevision::REV3_1, keyIndex, dataIn, sfiToSelect, recordNumberToRead, "");
                                    apduRequest = apduCommandBuilder->getApduRequest();
                                    ASSERT_EQ(request3_1, apduRequest->getBytes());
                                }

                                void OpenSessionCmdBuildTest::openSessionCmdBuild_rev_3_2() {
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
                                    apduCommandBuilder = AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(PoRevision::REV3_2, keyIndex, samChallenge, sfiToSelect, recordNumberToRead, "");
                                    apduRequest = apduCommandBuilder->getApduRequest();
                                    ASSERT_EQ(ByteArrayUtil::toHex(request3_2), ByteArrayUtil::toHex(apduRequest->getBytes()));
                                }
                            }
                        }
                    }
                }
            }
        }

TEST(OpenSessionCmdBuildTest, testA) 
{
    std::shared_ptr<OpenSessionCmdBuildTest> LocalTest = std::make_shared<OpenSessionCmdBuildTest>();
    LocalTest->openSessionCmdBuild_rev_2_4_exception();
}

TEST(OpenSessionCmdBuildTest, testB) 
{
    std::shared_ptr<OpenSessionCmdBuildTest> LocalTest = std::make_shared<OpenSessionCmdBuildTest>();
    LocalTest->openSessionCmdBuild_rev_2_4();
}

TEST(OpenSessionCmdBuildTest, testC) 
{
    std::shared_ptr<OpenSessionCmdBuildTest> LocalTest = std::make_shared<OpenSessionCmdBuildTest>();
    LocalTest->openSessionCmdBuild_rev_3_1();
}

TEST(OpenSessionCmdBuildTest, testD) 
{
    std::shared_ptr<OpenSessionCmdBuildTest> LocalTest = std::make_shared<OpenSessionCmdBuildTest>();
    LocalTest->openSessionCmdBuild_rev_3_2();
}
