#include "AppendRecordCmdBuild.h"
#include "AppendRecordRespPars.h"
#include "ApduResponse.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace org::eclipse::keyple::calypso::command;
using namespace org::eclipse::keyple::calypso::command::po::parser;
using namespace org::eclipse::keyple::core::seproxy::message;

AppendRecordCmdBuild::AppendRecordCmdBuild(PoClass poClass, char sfi, std::vector<char> &newRecordData, const std::string &extraInfo)
: AbstractPoCommandBuilder<AppendRecordRespPars>(CalypsoPoCommands::APPEND_RECORD, nullptr)
{
    char cla = poClass.getValue();
    char p1 = static_cast<char>(0x00);
    char p2 = (sfi == 0) ? static_cast<char>(0x00) : static_cast<char>(sfi * 8);

    this->request = setApduRequest(cla, command, p1, p2, newRecordData, -1);
    if (extraInfo != "") {
        this->addSubName(extraInfo);
    }
}

std::shared_ptr<AppendRecordRespPars> AppendRecordCmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<AppendRecordRespPars>(apduResponse);
}

}
}
}
}
}
}
}
