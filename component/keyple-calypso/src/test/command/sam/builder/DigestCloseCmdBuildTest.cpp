#include "DigestCloseCmdBuildTest.h"
//#include "SamRevision.h"
#include "DigestCloseCmdBuild.h"
#include "AbstractApduCommandBuilder.h"
#include "message/ApduRequest.h"

using namespace keyple::calypso::command::sam::builder::security;

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using SamRevision = keyple::calypso::command::sam::SamRevision;
using DigestCloseCmdBuild =
    keyple::calypso::command::sam::builder::security::DigestCloseCmdBuild;
using AbstractApduCommandBuilder =
    keyple::core::command::AbstractApduCommandBuilder;
using ApduRequest = keyple::core::seproxy::message::ApduRequest;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void digestCloseCmdBuild() throws IllegalArgumentException
void DigestCloseCmdBuildTest::digestCloseCmdBuild()
{

    std::vector<char> request = {static_cast<char>(0x94),
                                 static_cast<char>(0x8E), 0x00, 0x00,
                                 static_cast<char>(0x04)};
    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<DigestCloseCmdBuild>(SamRevision::S1D,
                                              static_cast<char>(0x04)); // 94
    std::shared_ptr<ApduRequest> apduReq = apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request, apduReq->getBytes());

    std::vector<char> request1 = {static_cast<char>(0x80),
                                  static_cast<char>(0x8E), 0x00, 0x00,
                                  static_cast<char>(0x04)};
    apduCommandBuilder         = std::make_shared<DigestCloseCmdBuild>(
        SamRevision::C1, static_cast<char>(0x04)); // 94
    apduReq = apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request1, apduReq->getBytes());
}
}
}
}
}
}
}

TEST(DigestCloseCmdBuildTest, testA)
{
    std::shared_ptr<DigestCloseCmdBuildTest> LocalTest =
        std::make_shared<DigestCloseCmdBuildTest>();
    LocalTest->digestCloseCmdBuild();
}
