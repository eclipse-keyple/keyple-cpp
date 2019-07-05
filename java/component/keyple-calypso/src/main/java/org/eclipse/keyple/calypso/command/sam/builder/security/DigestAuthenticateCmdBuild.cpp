#include "DigestAuthenticateCmdBuild.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using AbstractSamCommandBuilder = org::eclipse::keyple::calypso::command::sam::AbstractSamCommandBuilder;
using CalypsoSamCommands        = org::eclipse::keyple::calypso::command::sam::CalypsoSamCommands;
using SamRevision               = org::eclipse::keyple::calypso::command::sam::SamRevision;

DigestAuthenticateCmdBuild::DigestAuthenticateCmdBuild(SamRevision revision,
                                                       std::vector<char> &signature)
 : AbstractSamCommandBuilder(std::make_shared<CalypsoSamCommands>(command),
                             nullptr)
 {
    this->defaultRevision = revision;

    if (signature.empty()) {
        throw std::invalid_argument("Signature can't be null");
    }
    
    if (signature.size() != 4 && signature.size() != 8 && signature.size() != 16) {
        throw std::invalid_argument("Signature is not the right length : length is " + std::to_string(signature.size()));
    }
    
    char cla = SamRevision::S1D == (this->defaultRevision) ? static_cast<char>(0x94) : static_cast<char>(0x80);
    char p1 = 0x00;
    char p2 = static_cast<char>(0x00);

    request = setApduRequest(cla, std::make_shared<CalypsoSamCommands>(command), p1, p2, signature, -1);
}

}
}
}
}
}
}
}
}
