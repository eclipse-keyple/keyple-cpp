#include "DigestUpdateMultipleRespPars.h"
#include "ApduResponse.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using AbstractSamResponseParser = org::eclipse::keyple::calypso::command::sam::AbstractSamResponseParser;
using ApduResponse              = org::eclipse::keyple::core::seproxy::message::ApduResponse;

DigestUpdateMultipleRespPars::DigestUpdateMultipleRespPars(std::shared_ptr<ApduResponse> response)
: org::eclipse::keyple::command::AbstractSamResponseParser(response)
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
