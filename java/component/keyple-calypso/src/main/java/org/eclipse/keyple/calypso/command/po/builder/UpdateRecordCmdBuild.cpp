#include "UpdateRecordCmdBuild.h"
#include "UpdateRecordRespPars.h"
#include "ApduResponse.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using PoClass              = org::eclipse::keyple::calypso::command::PoClass;
using UpdateRecordRespPars = org::eclipse::keyple::calypso::command::po::parser::UpdateRecordRespPars;
using ApduResponse         = org::eclipse::keyple::core::seproxy::message::ApduResponse;

UpdateRecordCmdBuild::UpdateRecordCmdBuild(PoClass poClass, char sfi, char recordNumber,
                                           std::vector<char> &newRecordData,
                                           const std::string &extraInfo)
: AbstractPoCommandBuilder<UpdateRecordRespPars>(std::make_shared<CalypsoPoCommands>(command),
                                                 nullptr)
{
    if (recordNumber < 1) {
        throw std::invalid_argument("Bad record number (< 1)");
    }
    char p2 = (sfi == 0) ? static_cast<char>(0x04) : static_cast<char>(static_cast<char>(sfi * 8) + 4);

    this->request = setApduRequest(poClass.getValue(),
                                   std::make_shared<CalypsoPoCommands>(command),
                                   recordNumber, p2, newRecordData, -1);
    if (!extraInfo.compare("")) {
        this->addSubName(extraInfo);
    }
}

std::shared_ptr<UpdateRecordRespPars> UpdateRecordCmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse) {
    return std::make_shared<UpdateRecordRespPars>(apduResponse);
}

}
}
}
}
}
}
}
