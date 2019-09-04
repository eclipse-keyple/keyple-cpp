#include "DigestUpdateMultipleCmdBuild.h"

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

DigestUpdateMultipleCmdBuild::DigestUpdateMultipleCmdBuild(SamRevision revision, std::vector<char> &digestData)
 : AbstractSamCommandBuilder(command, nullptr)
 {
    this->defaultRevision = revision;

    char cla = this->defaultRevision.getClassByte();
    char p1 = static_cast<char>(0x80);
    char p2 = static_cast<char>(0x00);

    request = setApduRequest(cla, command, p1, p2, digestData, -1);
}

}
}
}
}
}
}
}
}
