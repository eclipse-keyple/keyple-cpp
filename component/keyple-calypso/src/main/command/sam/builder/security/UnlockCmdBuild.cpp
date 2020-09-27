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

#include "UnlockCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"

/* Calypso */
#include "UnlockRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::common;

UnlockCmdBuild::UnlockCmdBuild(
  const SamRevision& revision, const std::vector<uint8_t>& unlockData)
: AbstractSamCommandBuilder(
    std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::UNLOCK), nullptr)
{
    mDefaultRevision = revision;

    const uint8_t cla = mDefaultRevision.getClassByte();
    const uint8_t p1  = 0x00;
    const uint8_t p2  = 0x00;

    if (unlockData.empty())
        throw IllegalArgumentException("Unlock data null!");

    if (unlockData.size() != 8 && unlockData.size() != 16)
        throw IllegalArgumentException("Unlock data should be 8 ou 16 " \
                                       "bytes long!");

    mRequest = setApduRequest(cla, mCommand, p1, p2, unlockData);
}

std::shared_ptr<UnlockRespPars> UnlockCmdBuild::createResponseParser(
    const std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<UnlockRespPars>(apduResponse, this);
}

}
}
}
}
}
}
