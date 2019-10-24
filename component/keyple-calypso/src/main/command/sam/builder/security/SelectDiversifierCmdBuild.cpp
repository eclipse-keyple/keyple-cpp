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

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

SelectDiversifierCmdBuild::SelectDiversifierCmdBuild(
  SamRevision revision, std::vector<char> &diversifier)
: AbstractSamCommandBuilder(CalypsoSamCommands::SELECT_DIVERSIFIER, nullptr)
{
    this->defaultRevision = revision;

    if (diversifier.empty() ||
        (diversifier.size() != 4 && diversifier.size() != 8)) {
        throw std::invalid_argument("Bad diversifier value!");
    }

    char cla = this->defaultRevision.getClassByte();
    char p1 = 0x00;
    char p2 = 0x00;

    request = setApduRequest(cla, command, p1, p2, diversifier, -1);

}
}
}
}
}
}
}
