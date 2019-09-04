#include <vector>

#include "GetDataFciCmdBuild.h"
#include "GetDataFciRespPars.h"
#include "ApduResponse.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace org::eclipse::keyple::calypso::command;
using namespace org::eclipse::keyple::calypso::command::po;
using namespace org::eclipse::keyple::calypso::command::po::parser;
using namespace org::eclipse::keyple::core::seproxy::message;

GetDataFciCmdBuild::GetDataFciCmdBuild(PoClass poClass)
: AbstractPoCommandBuilder<GetDataFciRespPars>(command, nullptr)
{
    std::vector<char> empty;

    request = setApduRequest(poClass.getValue(), command, static_cast<char>(0x00),  static_cast<char>(0x6F), empty,
                             static_cast<char>(0x00));
}

std::shared_ptr<GetDataFciRespPars> GetDataFciCmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<GetDataFciRespPars>(apduResponse);
}

}
}
}
}
}
}
}
