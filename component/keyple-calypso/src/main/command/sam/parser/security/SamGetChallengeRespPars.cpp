/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
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
#include "ApduResponse.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::core::seproxy::message;

SamGetChallengeRespPars::SamGetChallengeRespPars(
    std::shared_ptr<ApduResponse> response)
: AbstractSamResponseParser(response)
{
}

std::vector<uint8_t> SamGetChallengeRespPars::getChallenge() const
{
    return isSuccessful() ? response->getDataOut() : std::vector<uint8_t>();
}

}
}
}
}
}
}
