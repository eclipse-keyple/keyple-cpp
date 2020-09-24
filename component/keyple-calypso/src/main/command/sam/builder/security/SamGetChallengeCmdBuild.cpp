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

#include "SamGetChallengeCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"

/* Calypso */
#include "SamGetChallengeRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::common;

SamGetChallengeCmdBuild::SamGetChallengeCmdBuild(
  const SamRevision& revision, const uint8_t expectedResponseLength)
: AbstractSamCommandBuilder(
    std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::GET_CHALLENGE),
    nullptr)
{
    mDefaultRevision = revision;

    if (expectedResponseLength != 0x04 && expectedResponseLength != 0x08) {
        throw IllegalArgumentException(
                  StringHelper::formatSimple(
                      "Bad challenge length! Expected 4 or 8, got %d",
                      expectedResponseLength));
    }

    const uint8_t cla = mDefaultRevision.getClassByte();
    const uint8_t p1 = 0x00;
    const uint8_t p2 = 0x00;

    mRequest = setApduRequest(cla, mCommand, p1, p2, expectedResponseLength);
}

std::unique_ptr<SamGetChallengeRespPars>
    SamGetChallengeCmdBuild::createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse)
{
    return std::unique_ptr<SamGetChallengeRespPars>(
               new SamGetChallengeRespPars(apduResponse, this));
}

}
}
}
}
}
}
