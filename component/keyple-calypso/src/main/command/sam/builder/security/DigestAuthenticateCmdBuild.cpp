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

#include "DigestAuthenticateCmdBuild.h"

/* Common */
#include "stringhelper.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

DigestAuthenticateCmdBuild::DigestAuthenticateCmdBuild(
    SamRevision revision, std::vector<uint8_t>& signature)
: AbstractSamCommandBuilder(CalypsoSamCommands::DIGEST_AUTHENTICATE, nullptr)
{
    this->defaultRevision = revision;

    if (signature.empty()) {
        throw std::invalid_argument("Signature can't be null");
    }

    if (signature.size() != 4 && signature.size() != 8 &&
        signature.size() != 16) {
        throw std::invalid_argument("Signature is not the right length : "
                                    "length is " +
                                    StringHelper::to_string(signature.size()));
    }

    uint8_t cla = SamRevision::S1D == (this->defaultRevision) ? 0x94 : 0x80;

    request = setApduRequest(cla, command, 0x00, 0x00, signature);
}

}
}
}
}
}
}
