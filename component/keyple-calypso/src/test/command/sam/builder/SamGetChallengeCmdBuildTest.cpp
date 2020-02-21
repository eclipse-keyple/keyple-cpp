#include "SamGetChallengeCmdBuildTest.h"
//#include "SamRevision.h"
#include "SamGetChallengeCmdBuild.h"
#include "AbstractApduCommandBuilder.h"
#include "ApduRequest.h"

using namespace keyple::calypso::command::sam::builder::security;

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using SamRevision = keyple::calypso::command::sam::SamRevision;
using SamGetChallengeCmdBuild =
    keyple::calypso::command::sam::builder::security::SamGetChallengeCmdBuild;
using AbstractApduCommandBuilder =
    keyple::core::command::AbstractApduCommandBuilder;
using ApduRequest = keyple::core::seproxy::message::ApduRequest;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getChallengeCmdBuild() throws IllegalArgumentException
void SamGetChallengeCmdBuildTest::getChallengeCmdBuild()
{
    std::vector<char> request = {static_cast<char>(0x94),
                                 static_cast<char>(0x84), 0x00, 0x00, 0x04};

    std::shared_ptr<AbstractApduCommandBuilder> apduCommandBuilder =
        std::make_shared<SamGetChallengeCmdBuild>(
            SamRevision::S1D, static_cast<char>(0x04)); // 94
    std::shared_ptr<ApduRequest> apduRequest =
        apduCommandBuilder->getApduRequest();

    ASSERT_EQ(request, apduRequest->getBytes());
}
}
}
}
}
}
}
