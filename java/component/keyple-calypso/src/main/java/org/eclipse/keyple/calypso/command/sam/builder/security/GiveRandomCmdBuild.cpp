#include "GiveRandomCmdBuild.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace org::eclipse::keyple::calypso::command::sam;

GiveRandomCmdBuild::GiveRandomCmdBuild(SamRevision revision, std::vector<char> &random) : AbstractSamCommandBuilder(command, nullptr)
{
    //if (revision != nullptr) {
        this->defaultRevision = revision;
    //}
    char cla = this->defaultRevision.getClassByte();
    char p1 = static_cast<char>(0x00);
    char p2 = static_cast<char>(0x00);

    if (random.size() > 0 && random.size() != 8) {
        throw std::invalid_argument("Random value should be an 8 bytes long");
    }

    request = setApduRequest(cla, command, p1, p2, random, -1);
}

}
}
}
}
}
}
}
}
