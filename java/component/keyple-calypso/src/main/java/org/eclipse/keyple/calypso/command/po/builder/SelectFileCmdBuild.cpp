#include "SelectFileCmdBuild.h"
#include "SelectFileRespPars.h"
#include "ApduResponse.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using PoClass                  = org::eclipse::keyple::calypso::command::PoClass;
using AbstractPoCommandBuilder = org::eclipse::keyple::calypso::command::po::AbstractPoCommandBuilder;
using CalypsoPoCommands        = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
using PoSendableInSession      = org::eclipse::keyple::calypso::command::po::PoSendableInSession;
using SelectFileRespPars       = org::eclipse::keyple::calypso::command::po::parser::SelectFileRespPars;
using ApduResponse             = org::eclipse::keyple::core::seproxy::message::ApduResponse;

SelectFileCmdBuild::SelectFileCmdBuild(PoClass poClass, SelectControl selectControl)
: AbstractPoCommandBuilder<SelectFileRespPars>(command, nullptr)
{
    char p1;
    char p2;
    std::vector<char> selectData = {0x00, 0x00};
    switch (selectControl) {
        case org::eclipse::keyple::calypso::command::po::builder::SelectFileCmdBuild::SelectControl::FIRST:
             p1 = static_cast<char>(0x02);
            p2 = static_cast<char>(0x00);
            break;
        case org::eclipse::keyple::calypso::command::po::builder::SelectFileCmdBuild::SelectControl::NEXT:
            p1 = static_cast<char>(0x02);
            p2 = static_cast<char>(0x02);
            break;
        case org::eclipse::keyple::calypso::command::po::builder::SelectFileCmdBuild::SelectControl::CURRENT_DF:
            p1 = static_cast<char>(0x09);
            p2 = static_cast<char>(0x00);
            break;
        default:
            throw std::make_shared<IllegalStateException>("Unsupported selectControl parameter " + selectControl.toString());
    }

    request = setApduRequest(poClass.getValue(), command, p1, p2, selectData, static_cast<char>(0x00));
}

SelectFileCmdBuild::SelectFileCmdBuild(PoClass poClass, std::vector<char> &selectionPath) : org::eclipse::keyple::calypso::command::po::AbstractPoCommandBuilder<org::eclipse::keyple::calypso::command::po::parser::SelectFileRespPars>(command, nullptr) {
    request = setApduRequest(poClass.getValue(), command, static_cast<char>(0x09), static_cast<char>(0x00), selectionPath, static_cast<char>(0x00));
    }

std::shared_ptr<SelectFileRespPars> SelectFileCmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse) {
    return std::make_shared<SelectFileRespPars>(apduResponse);
}
}
}
}
}
}
}
}
