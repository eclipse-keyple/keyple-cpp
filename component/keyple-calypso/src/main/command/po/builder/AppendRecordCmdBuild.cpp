/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include "AppendRecordCmdBuild.h"
#include "AppendRecordRespPars.h"
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;

AppendRecordCmdBuild::AppendRecordCmdBuild(
    PoClass poClass, uint8_t sfi, const std::vector<uint8_t>& newRecordData,
    const std::string& extraInfo)
: AbstractPoCommandBuilder<AppendRecordRespPars>(
      CalypsoPoCommands::APPEND_RECORD, nullptr)
{
    this->request = setApduRequest(poClass.getValue(), command, 0x00,
                                   sfi == 0 ? 0x00 : sfi * 8, newRecordData);
    if (extraInfo != "") {
        this->addSubName(extraInfo);
    }
}

std::shared_ptr<AppendRecordRespPars>
AppendRecordCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<AppendRecordRespPars>(apduResponse);
}

}
}
}
}
}
