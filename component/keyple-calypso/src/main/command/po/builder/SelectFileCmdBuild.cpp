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

#include "SelectFileCmdBuild.h"
#include "SelectFileRespPars.h"
#include "ApduResponse.h"

/* Common */
#include "stringhelper.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;
using namespace keyple::calypso::command::po::builder;

SelectFileCmdBuild::SelectFileCmdBuild(PoClass poClass, SelectControl selectControl)
: AbstractPoCommandBuilder<SelectFileRespPars>(CalypsoPoCommands::SELECT_FILE, nullptr)
{
    char p1;
    char p2;
    std::vector<char> selectData = {0x00, 0x00};
    switch (selectControl) {
        case keyple::calypso::command::po::builder::SelectFileCmdBuild::SelectControl::FIRST:
            p1 = static_cast<char>(0x02);
            p2 = static_cast<char>(0x00);
            break;
        case keyple::calypso::command::po::builder::SelectFileCmdBuild::SelectControl::NEXT:
            p1 = static_cast<char>(0x02);
            p2 = static_cast<char>(0x02);
            break;
        case keyple::calypso::command::po::builder::SelectFileCmdBuild::SelectControl::CURRENT_DF:
            p1 = static_cast<char>(0x09);
            p2 = static_cast<char>(0x00);
            break;
        default:
            throw IllegalStateException(StringHelper::formatSimple("Unsupported selectControl parameter %s",
                                                                   ""));//selectControl.toString()));
    }

    request = setApduRequest(poClass.getValue(), command, p1, p2, selectData, static_cast<char>(0x00));
}

SelectFileCmdBuild::SelectFileCmdBuild(PoClass poClass, std::vector<char> &selectionPath)
: AbstractPoCommandBuilder<SelectFileRespPars>(CalypsoPoCommands::SELECT_FILE, nullptr)
{
    request = setApduRequest(poClass.getValue(), command, static_cast<char>(0x09),
                             static_cast<char>(0x00), selectionPath, static_cast<char>(0x00));
}

std::shared_ptr<SelectFileRespPars> SelectFileCmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SelectFileRespPars>(apduResponse);
}

}
}
}
}
}
