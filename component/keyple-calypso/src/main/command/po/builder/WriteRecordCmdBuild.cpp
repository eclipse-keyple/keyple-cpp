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

#include "WriteRecordCmdBuild.h"
#include "WriteRecordRespPars.h"
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;

WriteRecordCmdBuild::WriteRecordCmdBuild(
  PoClass poClass, uint8_t sfi, uint8_t recordNumber,
  const std::vector<uint8_t>& newRecordData, const std::string& extraInfo)
: AbstractPoCommandBuilder<WriteRecordRespPars>(
      CalypsoPoCommands::WRITE_RECORD, nullptr)
{
    if (recordNumber < 1) {
        throw std::invalid_argument("Bad record number (< 1)");
    }

    uint8_t p2 = (sfi == 0) ? 0x04 : (sfi * 8) + 4;

    this->request = setApduRequest(poClass.getValue(), command, recordNumber,
                                   p2, newRecordData);
    if (!extraInfo.compare("")) {
        this->addSubName(extraInfo);
    }
}

std::shared_ptr<WriteRecordRespPars>
    WriteRecordCmdBuild::createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<WriteRecordRespPars>(apduResponse);
}

}
}
}
}
}
