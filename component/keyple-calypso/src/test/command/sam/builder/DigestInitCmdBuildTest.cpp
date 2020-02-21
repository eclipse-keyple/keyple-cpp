#include "DigestInitCmdBuildTest.h"
//#include "SamRevision.h"
#include "DigestInitCmdBuild.h"
#include "AbstractApduCommandBuilder.h"

using namespace keyple::calypso::command::sam::builder::security;

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using SamRevision = keyple::calypso::command::sam::SamRevision;
using DigestInitCmdBuild =
    keyple::calypso::command::sam::builder::security::DigestInitCmdBuild;
using AbstractApduCommandBuilder =
    keyple::core::command::AbstractApduCommandBuilder;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void digestInitCmd_inconsistent() throws IllegalArgumentException
void DigestInitCmdBuildTest::digestInitCmd_inconsistent()
{

    std::vector<char> digestData = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

    bool verificationMode    = false;
    bool rev3_2Mode          = false;
    char workKeyRecordNumber = static_cast<char>(0x00);
    char workKeyKif          = static_cast<char>(0x00);
    char workKeyKVC          = static_cast<char>(0x7E);
    SamRevision revision     = SamRevision::S1D;
    try {
        /* code */
        std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
            std::make_shared<DigestInitCmdBuild>(
                revision, verificationMode, rev3_2Mode, workKeyRecordNumber,
                workKeyKif, workKeyKVC, digestData);
    } catch (...) {
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void digestInitCmd_inconsistent_digestNull() throws IllegalArgumentException
void DigestInitCmdBuildTest::digestInitCmd_inconsistent_digestNull()
{

    std::vector<char> digestData;

    bool verificationMode    = false;
    bool rev3_2Mode          = false;
    char workKeyRecordNumber = static_cast<char>(0x10);
    char workKeyKif          = static_cast<char>(0x30);
    char workKeyKVC          = static_cast<char>(0x7E);
    SamRevision revision     = SamRevision::S1D;

    try {
        /* code */
        std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
            std::make_shared<DigestInitCmdBuild>(
                revision, verificationMode, rev3_2Mode, workKeyRecordNumber,
                workKeyKif, workKeyKVC, digestData);
    } catch (...) {
    }
}

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void digestInitCmd() throws IllegalArgumentException
void DigestInitCmdBuildTest::digestInitCmd()
{

    std::vector<char> digestData = {static_cast<char>(0x80),
                                    static_cast<char>(0x8A), 0x00};
    char cla                     = static_cast<char>(0x94);
    char zero                    = static_cast<char>(0x00);
    char p1                      = static_cast<char>(zero + 1);
    char p1_2                    = static_cast<char>(p1 + 2);
    char p2                      = static_cast<char>(0xFF);

    bool verificationMode    = true;
    bool rev3_2Mode          = true;
    char workKeyRecordNumber = static_cast<char>(0xFF);
    char workKeyKif          = static_cast<char>(0x30);
    char workKeyKVC          = static_cast<char>(0x7E);
    SamRevision revision     = SamRevision::S1D;

    int size                  = digestData.size() + 2;
    std::vector<char> request = {
        cla,        static_cast<char>(0x8A), p1_2,
        p2,         static_cast<char>(size), workKeyKif,
        workKeyKVC, static_cast<char>(0x80), static_cast<char>(0x8A),
        0x00};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<DigestInitCmdBuild>(
            revision, verificationMode, rev3_2Mode, workKeyRecordNumber,
            workKeyKif, workKeyKVC, digestData);

    ASSERT_EQ(request, apduCommandBuilder->getApduRequest()->getBytes());
}
}
}
}
}
}
}

TEST(DigestInitCmdBuildTest, testA)
{
    std::shared_ptr<DigestInitCmdBuildTest> LocalTest =
        std::make_shared<DigestInitCmdBuildTest>();
    LocalTest->digestInitCmd_inconsistent();
}

TEST(DigestInitCmdBuildTest, testB)
{
    std::shared_ptr<DigestInitCmdBuildTest> LocalTest =
        std::make_shared<DigestInitCmdBuildTest>();
    LocalTest->digestInitCmd_inconsistent_digestNull();
}

TEST(DigestInitCmdBuildTest, testC)
{
    std::shared_ptr<DigestInitCmdBuildTest> LocalTest =
        std::make_shared<DigestInitCmdBuildTest>();
    LocalTest->digestInitCmd();
}
