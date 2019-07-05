#include "AbstractIso7816CommandBuilder.h"
#include "ApduRequest.h"
#include "PoCustomModificationCommandBuilder.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {

using AbstractIso7816CommandBuilder = org::eclipse::keyple::core::command::AbstractIso7816CommandBuilder;
using ApduRequest                   = org::eclipse::keyple::core::seproxy::message::ApduRequest;

PoCustomModificationCommandBuilder::PoCustomModificationCommandBuilder(const std::string &name,
                                                              std::shared_ptr<ApduRequest> request)
: AbstractIso7816CommandBuilder("PO Custom Modification Command: " + name, request)
{
}

}
}
}
}
}
}
