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

#include "CardGenerateKeyRespPars.h"

/* Core */
#include "ApduResponse.h"

/* Calypso */
#include "CalypsoSamAccessForbiddenException.h"
#include "CalypsoSamDataAccessException.h"
#include "CalypsoSamIllegalParameterException.h"
#include "CalypsoSamIncorrectInputDataException.h"
#include "CalypsoSamSecurityDataException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::command;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

const std::map<int, std::shared_ptr<StatusProperties>>
    CardGenerateKeyRespPars::STATUS_TABLE = {
    {
        0x6700,
        std::make_shared<StatusProperties>(
            "Incorrect Lc.",
            typeid(CalypsoSamIllegalParameterException))
    }, {
        0x6985,
        std::make_shared<StatusProperties>(
            "Preconditions not satisfied",
            typeid(CalypsoSamAccessForbiddenException))
    }, {
        0x6A00,
        std::make_shared<StatusProperties>(
            "Incorrect P1 or P2",
            typeid(CalypsoSamIllegalParameterException))
    }, {
        0x6A80,
        std::make_shared<StatusProperties>(
            "Incorrect incoming data: unknown or incorrect format",
            typeid(CalypsoSamIncorrectInputDataException))
    }, {
        0x6A83,
        std::make_shared<StatusProperties>(
            "Record not found: ciphering key or key to cipher not found",
            typeid(CalypsoSamDataAccessException))
    }, {
        0x9000,
        std::make_shared<StatusProperties>("Success")
    }
};

CardGenerateKeyRespPars::CardGenerateKeyRespPars(
  const std::shared_ptr<ApduResponse> response,
  CardGenerateKeyCmdBuild* builder)
: AbstractSamResponseParser(
   response,
   reinterpret_cast<AbstractSamCommandBuilder<AbstractSamResponseParser>*>(
       builder)) {}

const std::map<int, std::shared_ptr<StatusProperties>>&
    CardGenerateKeyRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

const std::vector<uint8_t> CardGenerateKeyRespPars::getCipheredData() const
{
    return isSuccessful() ? mResponse->getDataOut() : std::vector<uint8_t>();
}

}
}
}
}
}
}
