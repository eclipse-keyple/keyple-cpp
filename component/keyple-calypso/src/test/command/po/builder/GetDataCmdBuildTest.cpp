#include "GetDataCmdBuildTest.h"

using namespace keyple::calypso::command::po::builder;

        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace builder {

                            using PoClass = keyple::calypso::command::PoClass;
                            using AbstractApduCommandBuilder = keyple::core::command::AbstractApduCommandBuilder;
                            using ApduRequest = keyple::core::seproxy::message::ApduRequest;

                            void GetDataCmdBuildTest::getDataFCICmdBuild() 
                            {
                                std::vector<char> request = {static_cast<char>(0x94), static_cast<char>(0xCA), static_cast<char>(0x00), 0x6F, 0x00};
                                std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder = std::make_shared<GetDataFciCmdBuild>(PoClass::LEGACY);
                                std::shared_ptr<ApduRequest> apduReq = apduCommandBuilder->getApduRequest();
                                ASSERT_EQ( request, apduReq->getBytes() );
                            }
                            void GetDataCmdBuildTest::getDataFCICmdBuild2() 
                            {
                                std::vector<char> request2 = {static_cast<char>(0x00), static_cast<char>(0xCA), static_cast<char>(0x00), 0x6F, 0x00};
                                std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder = std::make_shared<GetDataFciCmdBuild>(PoClass::ISO);
                                std::shared_ptr<ApduRequest> apduReq = apduCommandBuilder->getApduRequest();
                                ASSERT_EQ(request2, apduReq->getBytes());
                            }
                        }
                    }
                }
            }
        }

TEST(GetDataCmdBuildTest, testA) 
{
    std::shared_ptr<GetDataCmdBuildTest> LocalTest = std::make_shared<GetDataCmdBuildTest>();
    LocalTest->getDataFCICmdBuild();
}

TEST(GetDataCmdBuildTest, testB) 
{
    std::shared_ptr<GetDataCmdBuildTest> LocalTest = std::make_shared<GetDataCmdBuildTest>();
    LocalTest->getDataFCICmdBuild2();
}
