#include "AbstractSamCommandBuilder.h"
#include "ApduRequest.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using AbstractIso7816CommandBuilder = org::eclipse::keyple::core::command::AbstractIso7816CommandBuilder;
using ApduRequest                   = org::eclipse::keyple::core::seproxy::message::ApduRequest;

AbstractSamCommandBuilder::AbstractSamCommandBuilder(std::shared_ptr<CalypsoSamCommands> reference,
                                                     std::shared_ptr<ApduRequest> request)
: AbstractIso7816CommandBuilder(reference, request)
{
}

}
}
}
}
}
}
