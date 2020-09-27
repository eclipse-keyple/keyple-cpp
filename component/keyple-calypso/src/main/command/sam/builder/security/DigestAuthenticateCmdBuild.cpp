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

#include "DigestAuthenticateCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"
#include "stringhelper.h"

/* Calypso */
#include "DigestAuthenticateRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::common;

const CalypsoSamCommand& DigestAuthenticateCmdBuild::mCommand =
    CalypsoSamCommand::DIGEST_AUTHENTICATE;

DigestAuthenticateCmdBuild::DigestAuthenticateCmdBuild(
  const SamRevision& revision, const std::vector<uint8_t>& signature)
: AbstractSamCommandBuilder(
    std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::DIGEST_AUTHENTICATE),
    nullptr)
{
    mDefaultRevision = revision;

    if (signature.empty()) {
        throw IllegalArgumentException("Signature can't be null");
    }

    if (signature.size() != 4 && signature.size() != 8 &&
        signature.size() != 16) {
        throw IllegalArgumentException(
                  "Signature is not the right length : length is " +
                  StringHelper::to_string(signature.size()));
    }

    const uint8_t cla = SamRevision::S1D == (mDefaultRevision) ? 0x94 : 0x80;
    const uint8_t p1 = 0x00;
    const uint8_t p2 = 0x00;

    mRequest = setApduRequest(cla, mCommand, p1, p2, signature);
}

std::shared_ptr<DigestAuthenticateRespPars>
    DigestAuthenticateCmdBuild::createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<DigestAuthenticateRespPars>(apduResponse, this);
}

}
}
}
}
}
}
