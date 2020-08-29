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
#include "DigestUpdateRespPars.h"
#include "SamRevision.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

/**
 * Builder for the SAM Digest Update APDU command. This command have to be sent
 * twice for each command executed during a session. First time for the command
 * sent and second time for the answer received
 */
class KEYPLECALYPSO_API DigestUpdateCmdBuild final
: public AbstractSamCommandBuilder<DigestUpdateRespPars> {
public:
    /**
     * Instantiates a new DigestUpdateCmdBuild.
     *
     * @param revision of the SAM
     * @param encryptedSession the encrypted session flag, true if encrypted
     * @param digestData all bytes from command sent by the PO or response from
     *        the command
     * @throw IllegalArgumentException - if the digest data is null or has a
     *        length &gt; 255
     */
    DigestUpdateCmdBuild(const SamRevision revision,
                         const bool encryptedSession,
                         const std::vector<uint8_t>& digestData);

    /**
     *
     */
    std::shared_ptr<DigestUpdateRespPars> createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse) override;

protected:
    /**
     *
     */
    std::shared_ptr<DigestUpdateCmdBuild> shared_from_this()
    {
        return std::static_pointer_cast<DigestUpdateCmdBuild>(
            AbstractSamCommandBuilder::shared_from_this());
    }

private:
    /**
     * The command reference
     */
    static const CalypsoSamCommand& mCommand;
};

}
}
}
}
}
}
