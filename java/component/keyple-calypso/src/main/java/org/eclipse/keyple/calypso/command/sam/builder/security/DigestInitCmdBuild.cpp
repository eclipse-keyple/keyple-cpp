#include "DigestInitCmdBuild.h"

#include "System.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {
using AbstractSamCommandBuilder = org::eclipse::keyple::calypso::command::sam::AbstractSamCommandBuilder;
using CalypsoSamCommands = org::eclipse::keyple::calypso::command::sam::CalypsoSamCommands;
using SamRevision = org::eclipse::keyple::calypso::command::sam::SamRevision;

DigestInitCmdBuild::DigestInitCmdBuild(SamRevision revision, bool verificationMode, bool rev3_2Mode, char workKeyRecordNumber, char workKeyKif, char workKeyKVC, std::vector<char> &digestData)
: AbstractSamCommandBuilder(std::make_shared<CalypsoSamCommands>(command), nullptr)
{
    this->defaultRevision = revision;

    if (workKeyRecordNumber == 0x00 && (workKeyKif == 0x00 || workKeyKVC == 0x00)) {
        throw std::invalid_argument("Bad key record number, kif or kvc!");
    }
    if (digestData.empty()) {
        throw std::invalid_argument("Digest data is null!");
    }
    char cla = SamRevision::S1D == (this->defaultRevision) ? static_cast<char>(0x94) : static_cast<char>(0x80);
    char p1 = 0x00;
    if (verificationMode) {
        p1 = static_cast<char>(p1 + 1);
    }
    if (rev3_2Mode) {
        p1 = static_cast<char>(p1 + 2);
    }

    char p2 = static_cast<char>(0xFF);
    if (workKeyKif == static_cast<char>(0xFF)) {
        p2 = workKeyRecordNumber;
    }

    std::vector<char> dataIn;

    if (p2 == static_cast<char>(0xFF)) {
        dataIn = std::vector<char>(2 + digestData.size());
        dataIn[0] = workKeyKif;
        dataIn[1] = workKeyKVC;
        System::arraycopy(digestData, 0, dataIn, 2, digestData.size());
    }
    else {
        dataIn.clear();
    }
    // CalypsoRequest calypsoRequest = new CalypsoRequest(cla, CalypsoCommands.SAM_DIGEST_INIT,
    // p1, p2, dataIn);
    request = setApduRequest(cla,
                             std::make_shared<CalypsoSamCommands>(CalypsoSamCommands::DIGEST_INIT),
                             p1, p2, dataIn, -1);

}

}
}
}
}
}
}
}
}
