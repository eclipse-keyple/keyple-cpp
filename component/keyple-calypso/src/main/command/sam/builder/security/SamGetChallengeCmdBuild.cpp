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

#include "SamGetChallengeCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

SamGetChallengeCmdBuild::SamGetChallengeCmdBuild(SamRevision revision,
                                                 uint8_t expectedResponseLength)
: AbstractSamCommandBuilder(CalypsoSamCommands::GET_CHALLENGE, nullptr)
{
    this->defaultRevision = revision;

    if (expectedResponseLength != 0x04 && expectedResponseLength != 0x08) {
        throw std::invalid_argument(
            StringHelper::formatSimple("Bad challenge length! Expected "
                                       "4 or 8, got %s",
                                       expectedResponseLength));
    }

    uint8_t cla = this->defaultRevision.getClassByte();

    /* CalypsoRequest calypsoRequest = new CalypsoRequest(); */
    request = setApduRequest(cla, command, 0x00, 0x00, expectedResponseLength);
}

}
}
}
}
}
}
