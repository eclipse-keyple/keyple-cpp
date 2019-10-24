/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#include <vector>

#include "GetDataFciCmdBuild.h"
#include "GetDataFciRespPars.h"
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;

GetDataFciCmdBuild::GetDataFciCmdBuild(PoClass poClass)
: AbstractPoCommandBuilder<GetDataFciRespPars>(CalypsoPoCommands::GET_DATA_FCI, nullptr)
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
