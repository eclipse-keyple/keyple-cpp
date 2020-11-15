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

#include "VerifyPinRespPars.h"

/* Calypso */
#include "CalypsoPoIllegalParameterException.h"
#include "CalypsoPoAccessForbiddenException.h"
#include "CalypsoPoDataAccessException.h"
#include "CalypsoPoPinException.h"
#include "CalypsoPoSecurityContextException.h"
#include "CalypsoPoSessionBufferOverflowException.h"
#include "CalypsoPoTerminatedException.h"

/* Common */
#include "IllegalStateException.h"
#include "stringhelper.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::calypso::command::po::exception;
using namespace keyple::common;
using namespace keyple::common::exception;

const std::map<int, std::shared_ptr<StatusProperties>> VerifyPinRespPars::STATUS_TABLE = {
    {
        0x6700,
        std::make_shared<StatusProperties>(
            "Lc value not supported (only 00h, 04h or 08h are supported).",
            typeid(CalypsoPoIllegalParameterException)),
    }, {
        0x6900,
        std::make_shared<StatusProperties>(
            "Transaction Counter is 0.",
            typeid(CalypsoPoTerminatedException)),
    }, {
        0x6982,
        std::make_shared<StatusProperties>(
            "Security conditions not fulfilled (Get Challenge not done: challenge unavailable).",
            typeid(CalypsoPoSecurityContextException)),
    }, {
        0x6985,
        std::make_shared<StatusProperties>(
            "Access forbidden (a session is open or DF is invalidated).",
            typeid(CalypsoPoAccessForbiddenException)),
    }, {
        0x63C1,
        std::make_shared<StatusProperties>(
            "Incorrect PIN (1 attempt remaining).",
            typeid(CalypsoPoPinException)),
    }, {
        0x63C2,
        std::make_shared<StatusProperties>(
            "Incorrect PIN (2 attempt remaining).",
            typeid(CalypsoPoPinException)),
    }, {
        0x6983,
        std::make_shared<StatusProperties>(
            "Presentation rejected (PIN is blocked).",
            typeid(CalypsoPoPinException)),
    }, {
        0x6D00,
        std::make_shared<StatusProperties>(
            "PIN function not present.",
            typeid(CalypsoPoIllegalParameterException)),
    }, {
        0x9000,
        std::make_shared<StatusProperties>(
            "Success")
    }
};

VerifyPinRespPars::VerifyPinRespPars(std::shared_ptr<ApduResponse> response,
                                     VerifyPinCmdBuild* builder)
: AbstractPoResponseParser(
    response,
    reinterpret_cast<AbstractPoCommandBuilder<AbstractPoResponseParser>*>(builder)) {}

int VerifyPinRespPars::getRemainingAttemptCounter() const
{
    int attemptCounter;

    switch (mResponse->getStatusCode()) {
    case 0x6983:
        attemptCounter = 0;
        break;
    case 0x63C1:
        attemptCounter = 1;
        break;
    case 0x63C2:
        attemptCounter = 2;
        break;
    case 0x9000:
        attemptCounter = 3;
        break;
    default:
        throw IllegalStateException("Incorrect status word: " +
                                    StringHelper::uint16ToHexString(mResponse->getStatusCode()));
    }

    return attemptCounter;
}

const std::map<int, std::shared_ptr<StatusProperties>>& VerifyPinRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

}
}
}
}
}
}
