#include "OpenSession31CmdBuild.h"
#include "OpenSession31RespPars.h"
#include "PoClass.h"
#include "CalypsoPoCommands.h"
#include "PoRevision.h"
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
using OpenSession31RespPars = org::eclipse::keyple::calypso::command::po::parser::security::OpenSession31RespPars;
using ApduResponse          = org::eclipse::keyple::core::seproxy::message::ApduResponse;

OpenSession31CmdBuild::OpenSession31CmdBuild(char keyIndex, std::vector<char> &samChallenge, char sfiToSelect, char recordNumberToRead, const std::string &extraInfo)
: AbstractOpenSessionCmdBuild<OpenSession31RespPars>(PoRevision::REV3_1)
{

    char p1 = static_cast<char>((recordNumberToRead * 8) + keyIndex);
    char p2 = static_cast<char>((sfiToSelect * 8) + 1);
    /*
     * case 4: this command contains incoming and outgoing data. We define le = 0, the actual
     * length will be processed by the lower layers.
     */
    char le = 0;

    this->request = setApduRequest(PoClass::ISO.getValue(), std::make_shared<CalypsoPoCommands>(CalypsoPoCommands::getOpenSessionForRev(PoRevision::REV3_1)), p1, p2, samChallenge, le);
    if (extraInfo != "") {
        this->addSubName(extraInfo);
    }
}

std::shared_ptr<OpenSession31RespPars> OpenSession31CmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse) {
    return std::make_shared<OpenSession31RespPars>(apduResponse);
}

}
}
}
}
}
}
}
}
