#include "OpenSession24CmdBuild.h"
#include "OpenSession24RespPars.h"
#include "PoClass.h"
#include "CalypsoPoCommands.h"
#include "PoRevision.h"

/* Core */
#include "ApduResponse.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using PoClass               = org::eclipse::keyple::calypso::command::PoClass;
using CalypsoPoCommands     = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
using PoRevision            = org::eclipse::keyple::calypso::command::po::PoRevision;
using OpenSession24RespPars = org::eclipse::keyple::calypso::command::po::parser::security::OpenSession24RespPars;
using ApduResponse          = org::eclipse::keyple::core::seproxy::message::ApduResponse;

OpenSession24CmdBuild::OpenSession24CmdBuild(char keyIndex, std::vector<char> &samChallenge, char sfiToSelect, char recordNumberToRead, const std::string &extraInfo)
: AbstractOpenSessionCmdBuild<OpenSession24RespPars>(PoRevision::REV2_4)
{

    if (keyIndex == 0x00) {
        throw std::invalid_argument("Key index can't be null for rev 2.4!");
    }

    char p1 = static_cast<char>(0x80 + (recordNumberToRead * 8) + keyIndex);
    char p2 = static_cast<char>(sfiToSelect * 8);
    /*
     * case 4: this command contains incoming and outgoing data. We define le = 0, the actual
     * length will be processed by the lower layers.
     */
    char le = 0;

    this->request = setApduRequest(PoClass::LEGACY.getValue(), CalypsoPoCommands::getOpenSessionForRev(PoRevision::REV2_4), p1, p2,
                                   samChallenge, le);
    if (extraInfo != "") {
        this->addSubName(extraInfo);
    }
}

std::shared_ptr<OpenSession24RespPars> OpenSession24CmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse) {
    return std::make_shared<OpenSession24RespPars>(apduResponse);
}

}
}
}
}
}
}
}
}
