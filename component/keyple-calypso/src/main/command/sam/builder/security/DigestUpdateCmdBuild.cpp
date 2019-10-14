#include "DigestUpdateCmdBuild.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace org::eclipse::keyple::calypso::command::sam;

DigestUpdateCmdBuild::DigestUpdateCmdBuild(SamRevision revision, bool encryptedSession, std::vector<char> &digestData)
: AbstractSamCommandBuilder(CalypsoSamCommands::DIGEST_UPDATE, nullptr)
{
    this->defaultRevision = revision;
    char cla = this->defaultRevision.getClassByte();
    char p1 = static_cast<char>(0x00);
    char p2 = static_cast<char>(0x00);
    if (encryptedSession) {
        p2 = static_cast<char>(0x80);
    }

    if (digestData.size() > 0 && digestData.size() > 255) {
        throw std::invalid_argument("Digest data null or too long!");
    }

    // CalypsoRequest calypsoRequest = new CalypsoRequest(cla, command, p1, p2, digestData);
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
