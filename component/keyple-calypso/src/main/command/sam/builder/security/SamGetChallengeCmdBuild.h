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

#pragma once

#include <stdexcept>
#include <memory>

#include "AbstractSamCommandBuilder.h"
#include "CalypsoSamCommand.h"
#include "SamRevision.h"

/* Common */
#include "stringhelper.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace sam {
    namespace parser { namespace security { class SamGetChallengeRespPars; } } }
    } } }

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;

/**
 * Builder for the SAM Get Challenge APDU command.
 */
class KEYPLECALYPSO_API SamGetChallengeCmdBuild final
: public AbstractSamCommandBuilder<SamGetChallengeRespPars> {
public:
    /**
     * Instantiates a new SamGetChallengeCmdBuild.
     *
     * @param revision of the SAM (SAM)
     * @param expectedResponseLength the expected response length
     * @throws IllegalArgumentException - if the expected response length has
     *         wrong value.
     */
    SamGetChallengeCmdBuild(const SamRevision& revision,
                            const uint8_t expectedResponseLength);

    /**
     *
     */
    std::unique_ptr<SamGetChallengeRespPars> createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse) override;

private:
    /**
     * The command reference
     */
    const CalypsoSamCommand& mCommand = CalypsoSamCommand::GET_CHALLENGE;
};

}
}
}
}
}
}
