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

#include "AppendRecordRespPars.h"

/* Core */
#include "AbstractApduResponseParser.h"
#include "ApduResponse.h"

/* Calypso */
#include "CalypsoPoAccessForbiddenException.h"
#include "CalypsoPoDataAccessException.h"
#include "CalypsoPoIllegalParameterException.h"
#include "CalypsoPoSecurityContextException.h"
#include "CalypsoPoSessionBufferOverflowException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::exception;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

const std::map<int, std::shared_ptr<StatusProperties>>
    AppendRecordRespPars::STATUS_TABLE = {
    {
        0x6B00,
        std::make_shared<StatusProperties>(
            "P1 or P2 vaue not supported.",
            typeid(CalypsoPoIllegalParameterException))
    }, {
        0x6700,
        std::make_shared<StatusProperties>(
             "Lc value not supported.",
             typeid(CalypsoPoDataAccessException))
    }, {
        0x6400,
        std::make_shared<StatusProperties>(
            "Too many modifications in session.",
            typeid(CalypsoPoSessionBufferOverflowException))
    }, {
        0x6981,
        std::make_shared<StatusProperties>(
            "The current EF is not a Cyclic EF.",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6982,
        std::make_shared<StatusProperties>(
            "Security conditions not fulfilled (no session, wrong key).",
            typeid(CalypsoPoSecurityContextException))
    }, {
        0x6985,
        std::make_shared<StatusProperties>(
            "Access forbidden (Never access mode, DF is invalidated, etc..).",
            typeid(CalypsoPoAccessForbiddenException))
    }, {
        0x6986,
        std::make_shared<StatusProperties>(
            "Command not allowed (no current EF).",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6A82,
        std::make_shared<StatusProperties>(
            "File not found.", typeid(CalypsoPoDataAccessException))
    }
};

AppendRecordRespPars::AppendRecordRespPars(
  std::shared_ptr<ApduResponse> response, AppendRecordCmdBuild* builder)
: AbstractPoResponseParser(
    response,
    dynamic_cast<AbstractPoCommandBuilder<AbstractPoResponseParser>*>(builder))
{}

const std::map<int, std::shared_ptr<StatusProperties>>&
    AppendRecordRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

}
}
}
}
}
