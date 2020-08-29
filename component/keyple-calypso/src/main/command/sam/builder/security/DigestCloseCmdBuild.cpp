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

#include "DigestCloseCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

const CalypsoSamCommand& DigestCloseCmdBuild::mCommand =
    CalypsoSamCommand::DIGEST_CLOSE;

DigestCloseCmdBuild::DigestCloseCmdBuild(
  const SamRevision& revision, const uint8_t expectedResponseLength)
: AbstractSamCommandBuilder(CalypsoSamCommands::DIGEST_CLOSE, nullptr)
{
    mDefaultRevision = revision;

    if (expectedResponseLength != 0x04 && expectedResponseLength != 0x08) {
        throw IllegalArgumentException(
                  StringHelper::formatSimple(
                      "Bad digest length! Expected 4 or 8, got %d",
                      expectedResponseLength));
    }

    const uint8_t cla = mDefaultRevision.getClassByte();
    const uint8_t p1 = 0x00;
    const uint8_t p2 = 0x00;

    mRequest = setApduRequest(cla, command, p1, p2, expectedResponseLength);
}

std::shared_ptr<DigestCloseRespPars> DigestCloseCmdBuild::createResponseParser(
    const std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<DigestCloseRespPars>(apduResponse, this);
}

}
}
}
}
}
}
