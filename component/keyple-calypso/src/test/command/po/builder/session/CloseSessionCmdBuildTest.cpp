#include "CloseSessionCmdBuildTest.h"

using namespace keyple::calypso::command::po::builder::security;

        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {
                            namespace security {
                                using PoClass = keyple::calypso::command::PoClass;
                                using AbstractApduCommandBuilder = keyple::core::command::AbstractApduCommandBuilder;
                                using ApduRequest = keyple::core::seproxy::message::ApduRequest;

                                void CloseSessionCmdBuildTest::closeSessionCmdBuild() {
                                    std::vector<char> request2_4 = {static_cast<char>(0x94), static_cast<char>(0x8E), 0x00, 0x00, static_cast<char>(0x04), static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), 0x3E, 0x00};
                                    std::vector<char> request3_1 = {static_cast<char>(0x00), static_cast<char>(0x8E), static_cast<char>(0x80), 0x00, static_cast<char>(0x04), static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), 0x3E, 0x00};
                                    std::vector<char> terminalSessionSignature = {static_cast<char>(0xA8), 0x31, static_cast<char>(0xC3), 0x3E};
                                    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder = std::make_shared<CloseSessionCmdBuild>(PoClass::LEGACY, false, terminalSessionSignature);
                                    std::shared_ptr<ApduRequest> reqApdu = apduCommandBuilder->getApduRequest();

                                    ASSERT_EQ(request2_4, reqApdu->getBytes());

                                    apduCommandBuilder = std::make_shared<CloseSessionCmdBuild>(PoClass::ISO, true, terminalSessionSignature);
                                    reqApdu = apduCommandBuilder->getApduRequest();

                                    ASSERT_EQ(request3_1, reqApdu->getBytes());
                                }
                            }
                        }
                    }
                }
            }
        }

TEST(CloseSessionCmdBuildTest, testA) 
{
    std::shared_ptr<CloseSessionCmdBuildTest> LocalTest = std::make_shared<CloseSessionCmdBuildTest>();
    LocalTest->closeSessionCmdBuild();
}
