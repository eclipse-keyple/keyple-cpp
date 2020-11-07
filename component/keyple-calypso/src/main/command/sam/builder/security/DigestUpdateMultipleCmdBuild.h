/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

#include <vector>
#include <stdexcept>
#include <memory>

#include "AbstractSamCommandBuilder.h"
#include "CalypsoSamCommand.h"
#include "SamRevision.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace sam {
    namespace parser { namespace security { class DigestUpdateMultipleRespPars;
    } } } } } }

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;

/**
 * This class provides the dedicated constructor to build the SAM Digest Update
 * Multiple APDU command.
 */
class DigestUpdateMultipleCmdBuild final
: public AbstractSamCommandBuilder<DigestUpdateMultipleRespPars> {
public:
    /**
     * Instantiates a new DigestUpdateMultipleCmdBuild.
     *
     * @param revision the revision
     * @param encryptedSession the encrypted session flag, true if encrypted
     * @param digestData the digest data
     */
    DigestUpdateMultipleCmdBuild(const SamRevision revision,
                                 const bool encryptedSession,
                                 const std::vector<uint8_t>& digestData);

    /**
     *
     */
    std::shared_ptr<DigestUpdateMultipleRespPars> createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse) override;

private:
    /**
     * The command
     */
    const CalypsoSamCommand& mCommand =
        CalypsoSamCommand::DIGEST_UPDATE_MULTIPLE;
};

}
}
}
}
}
}
