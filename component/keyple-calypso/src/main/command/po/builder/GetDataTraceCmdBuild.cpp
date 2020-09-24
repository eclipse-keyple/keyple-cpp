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

#include "GetDataTraceCmdBuild.h"

/* Calypso */
#include "GetDataTraceRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command::po::parser;

GetDataTraceCmdBuild::GetDataTraceCmdBuild(const PoClass poClass)
: AbstractPoCommandBuilder<GetDataTraceRespPars>(
      std::make_shared<CalypsoPoCommand>(CalypsoPoCommand::GET_DATA_TRACE),
      nullptr)
{
    mRequest = setApduRequest(poClass.getValue(), command, 0x01, 0x85, 0x00);
}

std::unique_ptr<GetDataTraceRespPars>
    GetDataTraceCmdBuild::createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse)
{
    return std::unique_ptr<GetDataTraceRespPars>(
               new GetDataTraceRespPars(apduResponse, this));
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
