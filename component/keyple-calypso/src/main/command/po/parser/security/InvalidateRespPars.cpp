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

#include "InvalidateRespPars.h"

/* Calypso */
#include "CalypsoPoAccessForbiddenException.h"
#include "CalypsoPoDataAccessException.h"
#include "CalypsoPoSecurityContextException.h"
#include "CalypsoPoSessionBufferOverflowException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::calypso::command::po::exception;

const std::map<int, std::shared_ptr<StatusProperties>> InvalidateRespPars::STATUS_TABLE = {
    {
         0x6400,
         std::make_shared<StatusProperties>(
             "Too many modifications in session.",
             typeid(CalypsoPoSessionBufferOverflowException))
    }, {
        0x6700,
        std::make_shared<StatusProperties>(
            "Lc value not supported.",
            typeid(CalypsoPoDataAccessException)),
    }, {
        0x6982,
        std::make_shared<StatusProperties>(
            "Security conditions not fulfilled (no session, wrong key).",
            typeid(CalypsoPoSecurityContextException)),
    }, {
        0x6985,
        std::make_shared<StatusProperties>(
            "Access forbidden (DF context is invalid).",
            typeid(CalypsoPoAccessForbiddenException)),
    }, {
        0x9000,
        std::make_shared<StatusProperties>("Success")
    }
};

InvalidateRespPars::InvalidateRespPars(std::shared_ptr<ApduResponse> response,
                                       InvalidateCmdBuild* builder)
: AbstractPoResponseParser(
    response,
    reinterpret_cast<AbstractPoCommandBuilder<AbstractPoResponseParser>*>(builder)) {}

const std::map<int, std::shared_ptr<StatusProperties>>& InvalidateRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

}
}
}
}
}
}
