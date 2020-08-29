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

#include "DigestInitRespPars.h"

/* Core */
#include "ApduResponse.h"

/* Calypso */
#include "CalypsoSamAccessForbiddenException.h"
#include "CalypsoSamCounterOverflowException.h"
#include "CalypsoSamDataAccessException.h"
#include "CalypsoSamIllegalParameterException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::exception;
using namespace keyple::core::seproxy::message;

const std::map<int, std::shared_ptr<StatusProperties>>
    DigestInitRespPars::STATUS_TABLE = {
    {
        0x6700,
        std::make_shared<StatusProperties>(
            "Incorrect Lc.",
            typeid(CalypsoSamIllegalParameterException))
    }, {
        0x6900,
        std::make_shared<StatusProperties>(
            "An event counter cannot be incremented.",
            typeid(CalypsoSamCounterOverflowException))
    }, {
        0x6985,
        std::make_shared<StatusProperties>(
            "Preconditions not satisfied",
            typeid(CalypsoSamAccessForbiddenException))
    }, {
        0x6A00,
        std::make_shared<StatusProperties>(
            "Incorrect P2.",
            typeid(CalypsoSamIllegalParameterException))
    }, {
        0x6A83,
        std::make_shared<StatusProperties>(
            "Record not found: signing key not found.",
            typeid(CalypsoSamDataAccessException))
    }
};

DigestInitRespPars::DigestInitRespPars(
  const std::shared_ptr<ApduResponse> response, DigestInitCmdBuild* builder)
: AbstractSamResponseParser(response, builder) {}

const std::map<int, std::shared_ptr<StatusProperties>>&
    DigestInitRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

}
}
}
}
}
}
