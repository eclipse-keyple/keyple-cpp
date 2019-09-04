#include "OpenSession10CmdBuild.h"
#include "OpenSession10RespPars.h"
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
using OpenSession10RespPars = org::eclipse::keyple::calypso::command::po::parser::security::OpenSession10RespPars;
using ApduResponse          = org::eclipse::keyple::core::seproxy::message::ApduResponse;

OpenSession10CmdBuild::OpenSession10CmdBuild(char keyIndex, std::vector<char> &samChallenge, char sfiToSelect, char recordNumberToRead, const std::string &extraInfo)
: AbstractOpenSessionCmdBuild<OpenSession10RespPars>(PoRevision::REV1_0)
{

    if (keyIndex == 0x00) {
        throw std::invalid_argument("Key index can't be null for rev 1.0!");
    }

    char p1 = static_cast<char>((recordNumberToRead * 8) + keyIndex);
    char p2 = static_cast<char>(sfiToSelect * 8);
    /*
     * case 4: this command contains incoming and outgoing data. We define le = 0, the actual
     * length will be processed by the lower layers.
     */
    char le = 0;

    this->request = setApduRequest(PoClass::LEGACY.getValue(), CalypsoPoCommands::getOpenSessionForRev(PoRevision::REV1_0), p1, p2, samChallenge, le);
    if (extraInfo != "") {
        this->addSubName(extraInfo);
    }
}

std::shared_ptr<OpenSession10RespPars> OpenSession10CmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse) {
    return std::make_shared<OpenSession10RespPars>(apduResponse);
}

}
}
}
}
}
}
}
}
