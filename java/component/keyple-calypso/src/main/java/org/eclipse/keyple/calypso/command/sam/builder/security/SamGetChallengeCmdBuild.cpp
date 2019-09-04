#include "SamGetChallengeCmdBuild.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace org::eclipse::keyple::calypso::command::sam;

SamGetChallengeCmdBuild::SamGetChallengeCmdBuild(SamRevision revision, char expectedResponseLength)
: AbstractSamCommandBuilder(command, nullptr)
{
    this->defaultRevision = revision;

    if (expectedResponseLength != 0x04 && expectedResponseLength != 0x08) {
        throw std::invalid_argument(StringHelper::formatSimple("Bad challenge length! Expected 4 or 8, got %s", expectedResponseLength));
    }
    char cla = this->defaultRevision.getClassByte();
    char p1 = 0x00;
    char p2 = 0x00;

    // CalypsoRequest calypsoRequest = new CalypsoRequest();
    std::vector<char> emptyVector;
    request = setApduRequest(cla, command, p1, p2, emptyVector, expectedResponseLength);
}

}
}
}
}
}
}
}
}
