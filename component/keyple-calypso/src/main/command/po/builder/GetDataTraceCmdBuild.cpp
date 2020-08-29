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

#pragma once

#include "GetDataTraceCmdBuild.h

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

GetDataTraceCmdBuild::GetDataTraceCmdBuild(PoClass poClass)
: AbstractPoCommandBuilder<GetDataTraceRespPars>(
      CalypsoPoCommand::GET_DATA_TRACE, nullptr)
{
    request = setApduRequest(poClass.getValue(), command, 0x01, 0x85, 0x00);
}

std::shared_ptr<GetDataTraceRespPars>
    GetDataTraceCmdBuild::createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<GetDataTraceRespPars>(apduResponse, this);
}

bool GetDataTraceCmdBuild::isSessionBufferUsed() const
{
    return false;
}

}
}
}
}
}
}
