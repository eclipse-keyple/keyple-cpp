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
    namespace parser { namespace security { class DigestCloseRespPars; } } } }
    } }

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;

/**
 * Builder for the SAM Digest Close APDU command.
 */
class KEYPLECALYPSO_API DigestCloseCmdBuild final
: public AbstractSamCommandBuilder<DigestCloseRespPars> {
public:
    /**
     * Instantiates a new DigestCloseCmdBuild .
     *
     * @param revision of the SAM
     * @param expectedResponseLength the expected response length
     * @throw IllegalArgumentException - if the expected response length is
     *         wrong.
     */
    DigestCloseCmdBuild(const SamRevision& revision,
                        const uint8_t expectedResponseLength);

    /**
     *
     */
    std::shared_ptr<DigestCloseRespPars> createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse) override;

private:
    /**
     * The command
     */
    const CalypsoSamCommand& mCommand = CalypsoSamCommand::DIGEST_CLOSE;
};

}
}
}
}
}
}
