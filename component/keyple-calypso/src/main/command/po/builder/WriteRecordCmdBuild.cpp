/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

/* Calypso */
#include "WriteRecordRespPars.h"

/* Core */
#include "ApduResponse.h"

/* Common */
#include "stringhelper.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;

WriteRecordCmdBuild::WriteRecordCmdBuild(
  const PoClass& poClass,
  const uint8_t sfi,
  const uint8_t recordNumber,
  const std::vector<uint8_t>& newRecordData)
: AbstractPoCommandBuilder<WriteRecordRespPars>(
      std::make_shared<CalypsoPoCommand>(CalypsoPoCommand::WRITE_RECORD),
      nullptr),
  mSfi(sfi),
  mRecordNumber(recordNumber),
  mData(newRecordData)
{
    const uint8_t cla = poClass.getValue();
    const uint8_t p2 = (sfi == 0) ? 0x04 : (sfi * 8) + 4;

    mRequest = setApduRequest(cla, command, recordNumber, p2, newRecordData);

    const std::string extraInfo =
        "SFI=" + StringHelper::uint8ToHexString(sfi) + ", " +
        "REC=" + std::to_string(recordNumber);
    addSubName(extraInfo);
}

std::shared_ptr<WriteRecordRespPars> WriteRecordCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<WriteRecordRespPars>(apduResponse, this);
}

bool WriteRecordCmdBuild::isSessionBufferUsed() const
{
    return true;
}

int WriteRecordCmdBuild::getSfi() const
{
    return mSfi;
}

int WriteRecordCmdBuild::getRecordNumber() const
{
    return mRecordNumber;
}

const std::vector<uint8_t>& WriteRecordCmdBuild::getData() const
{
    return mData;
}

}
}
}
}
}
