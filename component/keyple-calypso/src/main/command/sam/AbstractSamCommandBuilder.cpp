#include "AbstractSamCommandBuilder.h"
#include "ApduRequest.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace org::eclipse::keyple::core::command;
using namespace org::eclipse::keyple::core::seproxy::message;

AbstractSamCommandBuilder::AbstractSamCommandBuilder(CalypsoSamCommands& reference, std::shared_ptr<ApduRequest> request)
: AbstractIso7816CommandBuilder(reference, request)
{
}

}
}
}
}
}
}
