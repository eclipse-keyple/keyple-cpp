/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "SvGetCmdBuild.h"

/* Calypso */
#include "SvGetRespPars.h"

/* Common */
#include "IllegalArgumentException.h"
#include "System.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace storedvalue {

using namespace keyple::common;
using namespace keyple::common::exception;

SvGetCmdBuild::SvGetCmdBuild(const PoClass& poClass,
                             const PoRevision& poRevision,
                             const Operation svOperation)
: AbstractPoCommandBuilder<SvGetRespPars>(
      std::make_shared<CalypsoPoCommand>(CalypsoPoCommand::SV_GET),
      nullptr),
  mSvOperation(svOperation)
{
    const uint8_t cla = poClass.getValue();
    const uint8_t p1 = poRevision == PoRevision::REV3_2 ? 0x01 : 0x00;
    const uint8_t p2 = svOperation == Operation::RELOAD ? 0x07 : 0x09;

    mRequest = setApduRequest(cla, mCommand, p1, p2, 0x00);

    const std::string operation = svOperation == Operation::DEBIT ? "DEBIT" : "RELOAD";
    addSubName("OPERATION=" + operation);

    mHeader = std::vector<uint8_t>(4);
    mHeader[0] = mCommand.getInstructionByte();
    mHeader[1] = p1;
    mHeader[2] = p2;
    mHeader[3] = 0x00;
}

Operation SvGetCmdBuild::getSvOperation() const
{
    return mSvOperation;
}

std::shared_ptr<SvGetRespPars> SvGetCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SvGetRespPars>(mHeader, apduResponse, this);
}

bool SvGetCmdBuild::isSessionBufferUsed() const
{
    return false;
}

}
}
}
}
}
}
