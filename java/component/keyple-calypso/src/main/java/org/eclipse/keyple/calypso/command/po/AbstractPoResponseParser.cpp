#include "AbstractPoResponseParser.h"
#include "ApduResponse.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {

using AbstractApduResponseParser = org::eclipse::keyple::core::command::AbstractApduResponseParser;
using ApduResponse = org::eclipse::keyple::core::seproxy::message::ApduResponse;

AbstractPoResponseParser::AbstractPoResponseParser(std::shared_ptr<ApduResponse> response)
: AbstractApduResponseParser(response)
{

}

}
}
}
}
}
}
