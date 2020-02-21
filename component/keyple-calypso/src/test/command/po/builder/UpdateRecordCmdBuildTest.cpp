#include "UpdateRecordCmdBuildTest.h"

using namespace keyple::calypso::command::po::builder;

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
using PoClass = keyple::calypso::command::PoClass;
using AbstractApduCommandBuilder =
    keyple::core::command::AbstractApduCommandBuilder;
using ApduRequest = keyple::core::seproxy::message::ApduRequest;

void UpdateRecordCmdBuildTest::updateRecordCmdBuild_rev2_4()
{

    // revision 2.4
    std::vector<char> request2_4 = {static_cast<char>(0x94),
                                    static_cast<char>(0xDC),
                                    static_cast<char>(0x01),
                                    0x44,
                                    static_cast<char>(0x05),
                                    0x00,
                                    0x01,
                                    0x02,
                                    0x03,
                                    0x04};
    apduCommandBuilder           = std::make_shared<UpdateRecordCmdBuild>(
        keyple::calypso::command::PoClass::LEGACY, static_cast<char>(0x08),
        record_number, newRecordData, "TestRev2_4");
    apduRequest = apduCommandBuilder->getApduRequest();
    ASSERT_EQ(request2_4, apduRequest->getBytes());
}

void UpdateRecordCmdBuildTest::updateRecordCmdBuild_rev3_1()
{
    // revision 3.1
    std::vector<char> request3_1 = {static_cast<char>(0x00),
                                    static_cast<char>(0xDC),
                                    static_cast<char>(0x01),
                                    0x44,
                                    static_cast<char>(0x05),
                                    0x00,
                                    0x01,
                                    0x02,
                                    0x03,
                                    0x04};
    apduCommandBuilder           = std::make_shared<UpdateRecordCmdBuild>(
        keyple::calypso::command::PoClass::ISO, static_cast<char>(0x08),
        record_number, newRecordData, "TestRev3_1");
    apduRequest = apduCommandBuilder->getApduRequest();
    ASSERT_EQ(request3_1, apduRequest->getBytes());
}

void UpdateRecordCmdBuildTest::updateRecordCmdBuild_rev3_2()
{
    // revision 3.2
    std::vector<char> request3_2 = {static_cast<char>(0x00),
                                    static_cast<char>(0xDC),
                                    static_cast<char>(0x01),
                                    0x44,
                                    static_cast<char>(0x05),
                                    0x00,
                                    0x01,
                                    0x02,
                                    0x03,
                                    0x04};
    apduCommandBuilder           = std::make_shared<UpdateRecordCmdBuild>(
        keyple::calypso::command::PoClass::ISO, static_cast<char>(0x08),
        record_number, newRecordData, "TestRev3_2");
    apduRequest = apduCommandBuilder->getApduRequest();
    ASSERT_EQ(request3_2, apduRequest->getBytes());
}
}
}
}
}
}

TEST(UpdateRecordCmdBuildTest, testA)
{
    std::shared_ptr<UpdateRecordCmdBuildTest> LocalTest =
        std::make_shared<UpdateRecordCmdBuildTest>();
    LocalTest->updateRecordCmdBuild_rev2_4();
}

TEST(UpdateRecordCmdBuildTest, testB)
{
    std::shared_ptr<UpdateRecordCmdBuildTest> LocalTest =
        std::make_shared<UpdateRecordCmdBuildTest>();
    LocalTest->updateRecordCmdBuild_rev3_1();
}

TEST(UpdateRecordCmdBuildTest, testC)
{
    std::shared_ptr<UpdateRecordCmdBuildTest> LocalTest =
        std::make_shared<UpdateRecordCmdBuildTest>();
    LocalTest->updateRecordCmdBuild_rev3_2();
}
