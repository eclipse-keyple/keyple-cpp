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

#include "GiveRandomCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::common;

GiveRandomCmdBuild::GiveRandomCmdBuild(
  const SamRevision& revision, const std::vector<uint8_t>& random)
: AbstractSamCommandBuilder(CalypsoSamCommands::GIVE_RANDOM, nullptr)
{
    //if (revision != nullptr) {
    mDefaultRevision = revision;
    //}

    const uint8_t cla = mDefaultRevision.getClassByte();
    const uint8_t p1 = 0x00;
    const uint8_t p2 = 0x00;

    if (static_cast<int>(random.size()) == 0 ||
        static_cast<int>(random.size()) != 8)
        throw IllegalArgumentException(
                  "Random value should be an 8 bytes long");

    mRequest = setApduRequest(cla, command, p1, p2, random);
}

std::shared_ptr<GiveRandomRespPars> GiveRandomCmdBuild::createResponseParser(
    const std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<GiveRandomRespPars>(apduResponse, this);
}

}
}
}
}
}
}
