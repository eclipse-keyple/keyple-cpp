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
#include "SamRevision.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace sam {
    namespace parser { namespace security { class DigestInitRespPars; } } } }
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
 * Builder for the SAM Digest Init APDU command.
 */
class KEYPLECALYPSO_API DigestInitCmdBuild final
: public AbstractSamCommandBuilder<DigestInitRespPars> {
public:
    /**
     * Instantiates a new DigestInitCmdBuild.
     *
     * @param revision of the SAM
     * @param verificationMode the verification mode
     * @param confidentialSessionMode the confidential session mode (rev 3.2)
     * @param workKeyRecordNumber the work key record number
     * @param workKeyKif from the AbstractOpenSessionCmdBuild response
     * @param workKeyKVC from the AbstractOpenSessionCmdBuild response
     * @param digestData all data out from the AbstractOpenSessionCmdBuild
     *        response
     * @throw IllegalArgumentException - if the work key record number
     * @throw IllegalArgumentException - if the digest data is null
     * @throw IllegalArgumentException - if the request is inconsistent
     */
    DigestInitCmdBuild(const SamRevision& revision,
                       const bool verificationMode,
                       const bool confidentialSessionMode,
                       const uint8_t workKeyRecordNumber,
                       const uint8_t workKeyKif,
                       const uint8_t workKeyKVC,
                       const std::vector<uint8_t>& digestData);

    /**
     *
     */
    std::unique_ptr<DigestInitRespPars> createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse) override;

private:
    /**
     * The command.
     */
    const CalypsoSamCommand& mCommand = CalypsoSamCommand::DIGEST_INIT;
};

}
}
}
}
}
}
