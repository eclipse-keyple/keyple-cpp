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

#include "UpdateRecordRespPars.h"

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

using namespace keyple::core::command;
using namespace keyple::calypso::command::po::exception;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

const std::map<int, std::shared_ptr<StatusProperties>>
    UpdateRecordRespPars::STATUS_TABLE = {
    {
        0x6400,
        std::make_shared<StatusProperties>(
            "Too many modifications in session",
            typeid(CalypsoPoSessionBufferOverflowException))
    }, {
        0x6700,
        std::make_shared<StatusProperties>(
            "Lc value not supported",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6981,
        std::make_shared<StatusProperties>(
            "Command forbidden on cyclic files when the record " \
            "exists and is not record 01h and on binary files",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6982,
        std::make_shared<StatusProperties>(
            "Security conditions not fulfilled (no " \
            "session, wrong key, encryption required)",
            typeid(CalypsoPoSecurityContextException))
    }, {
        0x6985,
        std::make_shared<StatusProperties>(
            "Access forbidden (Never access mode, DF is invalidated, etc..)",
            typeid(CalypsoPoAccessForbiddenException))
    }, {
        0x6986,
        std::make_shared<StatusProperties>(
            "Command not allowed (no current EF)",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6A82,
        std::make_shared<StatusProperties>(
            "File not found",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6A83,
        std::make_shared<StatusProperties>(
            "Record is not found (record index is 0 or above NumRec)",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6B00,
        std::make_shared<StatusProperties>(
            "P2 value not supported",
            typeid(CalypsoPoIllegalParameterException))
    }
};

const std::map<int, std::shared_ptr<StatusProperties>>&
    UpdateRecordRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

UpdateRecordRespPars::UpdateRecordRespPars(
  std::shared_ptr<ApduResponse> response, UpdateRecordCmdBuild* builder)
: AbstractPoResponseParser(
    response,
    dynamic_cast<AbstractPoCommandBuilder<AbstractPoResponseParser>*>(builder))
{}

}
}
}
}
}
