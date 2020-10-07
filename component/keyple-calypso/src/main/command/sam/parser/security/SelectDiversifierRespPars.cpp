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

#include "SelectDiversifierRespPars.h"

/* Core */
#include "ApduResponse.h"

/* Calypso */
#include "CalypsoSamAccessForbiddenException.h"
#include "CalypsoSamIllegalParameterException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::builder::exception;
using namespace keyple::calypso::command::sam::exception;
using namespace keyple::core::seproxy::message;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

const std::map<int, std::shared_ptr<StatusProperties>>
    SelectDiversifierRespPars::STATUS_TABLE = {
    {
        0x6700,
        std::make_shared<StatusProperties>(
            "Incorrect Lc.",
            typeid(CalypsoSamIllegalParameterException))
    }, {
        0x6985,
        std::make_shared<StatusProperties>(
            "Preconditions not satisfied.",
            typeid(CalypsoSamAccessForbiddenException))
    }, {
        0x9000,
        std::make_shared<StatusProperties>("Success")
    }
};

SelectDiversifierRespPars::SelectDiversifierRespPars(
  const std::shared_ptr<ApduResponse> response,
  SelectDiversifierCmdBuild* builder)
: AbstractSamResponseParser(
    response,
    reinterpret_cast<AbstractSamCommandBuilder<AbstractSamResponseParser>*>(
        builder)) {}

const std::map<int, std::shared_ptr<StatusProperties>>&
    SelectDiversifierRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

}
}
}
}
}
}
