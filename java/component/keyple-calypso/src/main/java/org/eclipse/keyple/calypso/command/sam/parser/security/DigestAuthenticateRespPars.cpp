#include "ApduResponse.h"
#include "DigestAuthenticateRespPars.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
using ApduResponse               = org::eclipse::keyple::seproxy::message::ApduResponse;

DigestAuthenticateRespPars::DigestAuthenticateRespPars(std::shared_ptr<ApduResponse> response)
: AbstractSamResponseParser(response)
{

}

}
}
}
}
}
}
}
}
