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
    namespace parser { namespace security { class GiveRandomRespPars; } } } } }
    }

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;

/**
 * Builder for the SAM Give Random APDU command.
 */
class GiveRandomCmdBuild final
: public AbstractSamCommandBuilder<GiveRandomRespPars> {
public:
    /**
     * Instantiates a new DigestUpdateCmdBuild.
     *
     * @param revision of the SAM
     * @param random the random data
     * @throw IllegalArgumentException - if the random data is null or has a
     *        length not equal to 8
     *        TODO implement specific settings for rev < 3
     */
    GiveRandomCmdBuild(const SamRevision& revision,
                       const std::vector<uint8_t>& random);

    /**
     *
     */
    std::shared_ptr<GiveRandomRespPars> createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse) override;

private:
    /**
     * The command reference
     */
    const CalypsoSamCommand& mCommand = CalypsoSamCommand::GIVE_RANDOM;
};

}
}
}
}
}
}
