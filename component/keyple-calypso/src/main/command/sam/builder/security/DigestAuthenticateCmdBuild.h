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
    namespace parser { namespace security { class DigestAuthenticateRespPars; }
    } } } } }

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;

/**
 * Builder for the Digest Authenticate APDU command.
 */
class KEYPLECALYPSO_API DigestAuthenticateCmdBuild final
: public AbstractSamCommandBuilder<DigestAuthenticateRespPars> {
public:
    /**
     * Instantiates a new DigestAuthenticateCmdBuild .
     *
     * @param revision of the SAM
     * @param signature the signature
     * @throw IllegalArgumentException - if the signature is null or has a
     *         wrong length.
     */
    DigestAuthenticateCmdBuild(const SamRevision& revision,
                               const std::vector<uint8_t>& signature);

    /**
     *
     */
    std::shared_ptr<DigestAuthenticateRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

private:
    /**
     * The command
     */
    static const CalypsoSamCommand& mCommand;
};

}
}
}
}
}
}
