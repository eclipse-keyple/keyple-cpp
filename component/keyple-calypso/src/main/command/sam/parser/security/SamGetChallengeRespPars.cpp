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

#include "SamGetChallengeRespPars.h"

/* Core */
#include "ApduResponse.h"

/* Calypso */
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

using StatusProperties = AbstractApduResponseParser::StatusProperties;

const std::map<int, std::shared_ptr<StatusProperties>>
    SamGetChallengeRespPars::STATUS_TABLE = {
    {
        0x6700,
        std::make_shared<StatusProperties>(
            "Incorrect Le.",
            typeid(CalypsoSamIllegalParameterException))
    }, {
        0x9000,
        std::make_shared<StatusProperties>("Success")
    }
};

SamGetChallengeRespPars::SamGetChallengeRespPars(
  const std::shared_ptr<ApduResponse> response,
  SamGetChallengeCmdBuild* builder)
: AbstractSamResponseParser(
   response,
   reinterpret_cast<AbstractSamCommandBuilder<AbstractSamResponseParser>*>(
       builder)) {}

const std::map<int, std::shared_ptr<StatusProperties>>&
    SamGetChallengeRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

const std::vector<uint8_t> SamGetChallengeRespPars::getChallenge() const
{
    return isSuccessful() ? mResponse->getDataOut() : std::vector<uint8_t>();
}

}
}
}
}
}
}
