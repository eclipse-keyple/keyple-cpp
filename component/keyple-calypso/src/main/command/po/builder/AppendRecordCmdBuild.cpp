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

#include "AppendRecordCmdBuild.h"

/* Calypso */
#include "AppendRecordRespPars.h"

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

AppendRecordCmdBuild::AppendRecordCmdBuild(
    PoClass poClass, uint8_t sfi, const std::vector<uint8_t>& newRecordData)
: AbstractPoCommandBuilder<AppendRecordRespPars>(
      CalypsoPoCommand::APPEND_RECORD, nullptr),
  mSfi(sfi), mData(newRecordData)
{
    const uint8_t cla = poClass.getValue();
    const uint8_t p1 = 0x00;
    const uint8_t p2 = (sfi == 0) ? 0x00 : (sfi * 8);

    request = setApduRequest(cla, command, p1, p2, newRecordData);

    const std::string extraInfo = StringHelper::formatSimple("SFI=%02X", sfi);
    addSubName(extraInfo);
}

std::shared_ptr<AppendRecordRespPars>
AppendRecordCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<AppendRecordRespPars>(apduResponse, this);
}

bool AppendRecordCmdBuild::isSessionBufferUsed() const
{
    return true;
}

uint8_t AppendRecordCmdBuild::getSfi() const
{
    return mSfi;
}

const std::vector<uint8_t>& AppendRecordCmdBuild::getData() const
{
    return mData;
}

}
}
}
}
}
