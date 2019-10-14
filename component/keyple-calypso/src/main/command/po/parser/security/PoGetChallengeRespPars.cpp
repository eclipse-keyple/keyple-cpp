#include "PoGetChallengeRespPars.h"
#include "ApduResponse.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using AbstractApduResponseParser = org::eclipse::keyple::core::command::AbstractApduResponseParser;
using ApduResponse               = org::eclipse::keyple::core::seproxy::message::ApduResponse;

PoGetChallengeRespPars::PoGetChallengeRespPars(std::shared_ptr<ApduResponse> response)
: AbstractApduResponseParser(response)
{
}

std::vector<char> PoGetChallengeRespPars::getPoChallenge()
{
    return getApduResponse()->getDataOut();
}

}
}
}
}
}
}
}
}
