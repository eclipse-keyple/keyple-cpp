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

#include "SelectDiversifierCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"

/* Calypso */
#include "SelectDiversifierRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::common;

SelectDiversifierCmdBuild::SelectDiversifierCmdBuild(
  const SamRevision& revision,
  const std::vector<uint8_t>& diversifier)
: AbstractSamCommandBuilder(
    std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::SELECT_DIVERSIFIER),
    nullptr)
{
    if (revision != SamRevision::NO_REV)
        mDefaultRevision = revision;

    if (static_cast<int>(diversifier.size()) != 4 &&
        static_cast<int>(diversifier.size()) != 8)
        throw IllegalArgumentException("Bad diversifier value!");

    const uint8_t cla = mDefaultRevision.getClassByte();
    const uint8_t p1 = 0x00;
    const uint8_t p2 = 0x00;

    mRequest = setApduRequest(cla, mCommand, p1, p2, diversifier);
}

std::shared_ptr<SelectDiversifierRespPars>
    SelectDiversifierCmdBuild::createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SelectDiversifierRespPars>(apduResponse, this);
}

}
}
}
}
}
}
