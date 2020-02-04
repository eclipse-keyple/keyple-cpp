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

#include "DigestUpdateMultipleCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

DigestUpdateMultipleCmdBuild::DigestUpdateMultipleCmdBuild(
  SamRevision revision, std::vector<uint8_t>& digestData)
: AbstractSamCommandBuilder(
      CalypsoSamCommands::DIGEST_UPDATE_MULTIPLE, nullptr)
 {
    this->defaultRevision = revision;

    uint8_t cla = this->defaultRevision.getClassByte();

    request = setApduRequest(cla, command, 0x80, 0x00, digestData);
}

}
}
}
}
}
}
