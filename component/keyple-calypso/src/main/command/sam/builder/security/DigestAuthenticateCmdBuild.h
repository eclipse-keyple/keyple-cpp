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

#include <vector>
#include <stdexcept>
#include <memory>

#include "AbstractSamCommandBuilder.h"
#include "CalypsoSamCommand.h"
#include "DigestAuthenticateRespPars.h"
#include "SamRevision.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

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
     * @throws IllegalArgumentException - if the signature is null or has a
     *         wrong length.
     */
    DigestAuthenticateCmdBuild(const SamRevision& revision,
                               const std::vector<uint8_t>& signature);

    /**
     *
     */
    std::shared_ptr<DigestAuthenticateRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

protected:
    /**
     *
     */
    std::shared_ptr<DigestAuthenticateCmdBuild> shared_from_this()
    {
        return std::static_pointer_cast<DigestAuthenticateCmdBuild>(
            AbstractSamCommandBuilder::shared_from_this());
    }

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
