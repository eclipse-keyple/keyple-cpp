#include "SelectFileCmdBuild.h"
#include "SelectFileRespPars.h"
#include "ApduResponse.h"

#include "stringhelper.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace org::eclipse::keyple::calypso::command::po;

using PoClass                  = org::eclipse::keyple::calypso::command::PoClass;
using CalypsoPoCommands        = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
using PoSendableInSession      = org::eclipse::keyple::calypso::command::po::PoSendableInSession;
using SelectFileRespPars       = org::eclipse::keyple::calypso::command::po::parser::SelectFileRespPars;
using ApduResponse             = org::eclipse::keyple::core::seproxy::message::ApduResponse;
using SelectControl            = org::eclipse::keyple::calypso::command::po::builder::SelectFileCmdBuild::SelectControl;
SelectFileCmdBuild::SelectFileCmdBuild(PoClass poClass, SelectControl selectControl)
: AbstractPoCommandBuilder<SelectFileRespPars>(std::make_shared<CalypsoPoCommands>(command),
                                               nullptr)
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
            throw IllegalStateException(StringHelper::formatSimple(
                                                          "Unsupported selectControl parameter %s",
                                                          ""));//selectControl.toString()));
    }

    request = setApduRequest(poClass.getValue(), std::make_shared<CalypsoPoCommands>(command), p1, p2, selectData, static_cast<char>(0x00));
}

SelectFileCmdBuild::SelectFileCmdBuild(PoClass poClass, std::vector<char> &selectionPath)
: AbstractPoCommandBuilder<SelectFileRespPars>(std::make_shared<CalypsoPoCommands>(command),
                                               nullptr)
{
    request = setApduRequest(poClass.getValue(), std::make_shared<CalypsoPoCommands>(command),
                             static_cast<char>(0x09), static_cast<char>(0x00), selectionPath,
                             static_cast<char>(0x00));
}

std::shared_ptr<SelectFileRespPars> SelectFileCmdBuild::createResponseParser(
                                                        std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SelectFileRespPars>(apduResponse);
}

}
}
}
}
}
}
}
