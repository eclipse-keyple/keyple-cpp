#include "SelectDiversifierCmdBuild.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace org::eclipse::keyple::calypso::command::sam;

SelectDiversifierCmdBuild::SelectDiversifierCmdBuild(SamRevision revision, std::vector<char> &diversifier)
: AbstractSamCommandBuilder(CalypsoSamCommands::SELECT_DIVERSIFIER, nullptr)
{
    this->defaultRevision = revision;

    if (diversifier.empty() || (diversifier.size() != 4 && diversifier.size() != 8)) {
        throw std::invalid_argument("Bad diversifier value!");
    }
    char cla = this->defaultRevision.getClassByte();
    char p1 = 0x00;
    char p2 = 0x00;

    request = setApduRequest(cla, command, p1, p2, diversifier, -1);

}
}
}
}
}
}
}
}
}
