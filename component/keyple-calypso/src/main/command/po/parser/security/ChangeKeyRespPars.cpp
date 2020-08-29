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

#include "ChangeKeyRespPars.h"

/* Core */
#include "AbstractApduResponseParser.h"
#include "ApduResponse.h"

/* Calypso */
#include "CalypsoPoAccessForbiddenException.h"
#include "CalypsoPoIllegalParameterException.h"
#include "CalypsoPoSecurityContextException.h"
#include "CalypsoPoSecurityDataException.h"
#include "CalypsoPoTerminatedException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::exception;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

std::map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
    ChangeKeyRespPars::STATUS_TABLE = {
    {
        0x6700,
        std::make_shared<AbstractApduResponseParser::StatusProperties>(
            "Lc value not supported (not 04h, 10h, 18h, 20h).",
            typeid(CalypsoPoIllegalParameterException))
    }, {
        0x6900,
        std::make_shared<AbstractApduResponseParser::StatusProperties>(
            "Transaction Counter is 0.",
            typeid(CalypsoPoTerminatedException))
    }, {
        0x6982,
        std::make_shared<AbstractApduResponseParser::StatusProperties>(
            "Security conditions not fulfilled (Get Challenge not " \
            "done: challenge unavailable).",
            typeid(CalypsoPoSecurityContextException))
    }, {
        0x6985,
        std::make_shared<AbstractApduResponseParser::StatusProperties>(
            "Access forbidden (a session is open or DF is invalidated).",
            typeid(CalypsoPoAccessForbiddenException))
    }, {
        0x6988,
        std::make_shared<AbstractApduResponseParser::StatusProperties>(
            "Incorrect Cryptogram.",
            typeid(CalypsoPoSecurityDataException))
    }, {
        0x6A80,
        std::make_shared<AbstractApduResponseParser::StatusProperties>(
            "Decrypted message incorrect (key algorithm not " \
            "supported, incorrect padding, etc.).",
            typeid(CalypsoPoSecurityDataException))
    }, {
        0x6A87,
        std::make_shared<AbstractApduResponseParser::StatusProperties>(
            "Lc not compatible with P2.",
            typeid(CalypsoPoIllegalParameterException))
    }, {
        0x6B00,
        std::make_shared<AbstractApduResponseParser::StatusProperties>(
            "Incorrect P1, P2.",
            typeid(CalypsoPoIllegalParameterException))
    }
};

ChangeKeyRespPars::ChangeKeyRespPars(
  std::shared_ptr<ApduResponse> response, ChangeKeyCmdBuild *builder
: AbstractPoResponseParser(response, builder)
{
}

std::map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
    ChangeKeyRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

}
}
}
}
}
}
