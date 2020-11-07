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

#include "InvalidateCmdBuild.h"

/* Calypso */
#include "InvalidateRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using namespace keyple::calypso::command::po::parser;

InvalidateCmdBuild::InvalidateCmdBuild(const PoClass& poClass)
: AbstractPoCommandBuilder<InvalidateRespPars>(
    std::make_shared<CalypsoPoCommand>(CalypsoPoCommand::INVALIDATE),
    nullptr)
{
    const uint8_t p1 = 0x00;
    const uint8_t p2 = 0x00;

    mRequest = setApduRequest(poClass.getValue(), mCommand, p1, p2, {});
}

std::shared_ptr<InvalidateRespPars> InvalidateCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<InvalidateRespPars>(apduResponse, this);
}

bool InvalidateCmdBuild::isSessionBufferUsed() const
{
    return true;
}

}
}
}
}
}
}
