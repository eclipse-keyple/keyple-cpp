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

#include "GetDataTraceRespPars.h"

/* Common */
#include "ClassNotFoundException.h"

/* Calypso */
#include "CalypsoPoDataAccessException.h"
#include "CalypsoPoIllegalParameterException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::calypso::command::po::exception;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

const std::map<int, std::shared_ptr<StatusProperties>>
    GetDataTraceRespPars::STATUS_TABLE = {
    {
        0x6A88,
        std::make_shared<StatusProperties>(
            "Data object not found (optional mode not available).",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6B00,
        std::make_shared<StatusProperties>(
            "P1 or P2 value not supported (<>004fh, 0062h, 006Fh, 00C0h, "
            "00D0h, 0185h and 5F52h, according to availabl optional modes).",
            typeid(CalypsoPoIllegalParameterException))
    }, {
        0x6283,
        std::make_shared<StatusProperties>(
            "Successful execution, FCI request and DF is invalidated.",
            typeid(ClassNotFoundException))
    }
};

GetDataTraceRespPars::GetDataTraceRespPars(
  std::shared_ptr<ApduResponse> response, GetDataTraceCmdBuild* builder)
: AbstractPoResponseParser(
    response,
    dynamic_cast<AbstractPoCommandBuilder<AbstractPoResponseParser>*>(builder))
{}

const std::map<int, std::shared_ptr<StatusProperties>>&
    GetDataTraceRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

}
}
}
}
}
