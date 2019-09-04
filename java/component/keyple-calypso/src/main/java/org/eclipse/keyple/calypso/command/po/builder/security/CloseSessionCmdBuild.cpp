#include "CloseSessionCmdBuild.h"
#include "CloseSessionRespPars.h"

/* Core */
#include "ByteArrayUtil.h"
#include "ApduResponse.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {


using namespace org::eclipse::keyple::calypso::command;
using namespace org::eclipse::keyple::calypso::command::po;
using namespace org::eclipse::keyple::calypso::command::po::parser;
using namespace org::eclipse::keyple::core::seproxy::message;
using namespace org::eclipse::keyple::core::util;

CloseSessionCmdBuild::CloseSessionCmdBuild(PoClass poClass, bool ratificationAsked, std::vector<char> &terminalSessionSignature)
: AbstractPoCommandBuilder<CloseSessionRespPars>(command, nullptr)
{
    // The optional parameter terminalSessionSignature could contain 4 or 8
    // bytes.
    if (terminalSessionSignature.size() > 0 && terminalSessionSignature.size() != 4 && terminalSessionSignature.size() != 8) {
        throw std::invalid_argument("Invalid terminal sessionSignature: " + ByteArrayUtil::toHex(terminalSessionSignature));
    }

    char p1 = ratificationAsked ? static_cast<char>(0x80) : static_cast<char>(0x00);
    /*
     * case 4: this command contains incoming and outgoing data. We define le = 0, the actual
     * length will be processed by the lower layers.
     */
    char le = 0;

    request = setApduRequest(poClass.getValue(), command, p1, static_cast<char>(0x00), terminalSessionSignature, le);
}

CloseSessionCmdBuild::CloseSessionCmdBuild(PoClass poClass) : AbstractPoCommandBuilder<CloseSessionRespPars>(command, nullptr)
{
    std::vector<char> emptyVector;
    request = setApduRequest(poClass.getValue(), command, static_cast<char>(0x00), static_cast<char>(0x00), emptyVector,
                             static_cast<char>(0));

    /* Add "Abort session" to command name for logging purposes */
    this->addSubName("Abort session");
}

std::shared_ptr<CloseSessionRespPars> CloseSessionCmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<CloseSessionRespPars>(apduResponse);
}

}
}
}
}
}
}
}
}
