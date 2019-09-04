#include "PoGetChallengeCmdBuild.h"
#include "PoGetChallengeRespPars.h"
#include "ApduResponse.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using namespace org::eclipse::keyple::calypso::command::po;

using PoClass                  = org::eclipse::keyple::calypso::command::PoClass;
using CalypsoPoCommands        = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
using PoGetChallengeRespPars   = org::eclipse::keyple::calypso::command::po::parser::security::PoGetChallengeRespPars;
using ApduResponse             = org::eclipse::keyple::core::seproxy::message::ApduResponse;

PoGetChallengeCmdBuild::PoGetChallengeCmdBuild(PoClass poClass) : AbstractPoCommandBuilder<PoGetChallengeRespPars>(command, nullptr)
{

    char p1 = static_cast<char>(0x01);
    char p2 = static_cast<char>(0x10);
    char le = static_cast<char>(0x08);
    std::vector<char> emptyVector;

    this->request = setApduRequest(poClass.getValue(), command, p1, p2, emptyVector, le);
}

std::shared_ptr<PoGetChallengeRespPars> PoGetChallengeCmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<PoGetChallengeRespPars>(apduResponse);
}

}
}
}
}
}
}
}
}
