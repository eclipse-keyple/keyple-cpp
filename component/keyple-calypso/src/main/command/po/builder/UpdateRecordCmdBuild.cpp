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

#include "UpdateRecordCmdBuild.h"
#include "UpdateRecordRespPars.h"
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;

UpdateRecordCmdBuild::UpdateRecordCmdBuild(PoClass poClass, char sfi, char recordNumber, std::vector<char> &newRecordData,
                                           const std::string &extraInfo)
: AbstractPoCommandBuilder<UpdateRecordRespPars>(CalypsoPoCommands::UPDATE_RECORD, nullptr)
{
    if (recordNumber < 1) {
        throw std::invalid_argument("Bad record number (< 1)");
    }
    char p2 = (sfi == 0) ? static_cast<char>(0x04) : static_cast<char>(static_cast<char>(sfi * 8) + 4);

    this->request = setApduRequest(poClass.getValue(), command, recordNumber, p2, newRecordData, -1);
    if (!extraInfo.compare("")) {
        this->addSubName(extraInfo);
    }
}

std::shared_ptr<UpdateRecordRespPars> UpdateRecordCmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<UpdateRecordRespPars>(apduResponse);
}

}
}
}
}
}
