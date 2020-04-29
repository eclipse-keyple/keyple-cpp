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

#include "DigestUpdateCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

DigestUpdateCmdBuild::DigestUpdateCmdBuild(SamRevision revision,
                                           bool encryptedSession,
                                           std::vector<uint8_t>& digestData)
: AbstractSamCommandBuilder(CalypsoSamCommands::DIGEST_UPDATE, nullptr)
{
    this->defaultRevision = revision;

    uint8_t cla = this->defaultRevision.getClassByte();
    uint8_t p2  = encryptedSession ? 0x80 : 0x00;

    if (digestData.size() > 0 && digestData.size() > 255) {
        throw std::invalid_argument("Digest data null or too long!");
    }

    /*
     * CalypsoRequest calypsoRequest = new CalypsoRequest(cla, command, p1, p2,
     * digestData);
     */
    request = setApduRequest(cla, command, 0x00, p2, digestData);
}

}
}
}
}
}
}
