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

#pragma once

#include <vector>
#include <stdexcept>
#include <memory>

#include "AbstractSamCommandBuilder.h"
#include "CalypsoSamCommands.h"
#include "SamRevision_Import.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

/**
 * Builder for the SAM Digest Init APDU command.
 */
class DigestInitCmdBuild : public AbstractSamCommandBuilder {
public:
    /**
     * Instantiates a new DigestInitCmdBuild.
     *
     * @param revision of the SAM
     * @param verificationMode the verification mode
     * @param rev3_2Mode the rev 3 2 mode
     * @param workKeyRecordNumber the work key record number
     * @param workKeyKif from the AbstractOpenSessionCmdBuild response
     * @param workKeyKVC from the AbstractOpenSessionCmdBuild response
     * @param digestData all data out from the AbstractOpenSessionCmdBuild
     *        response
     * @throws IllegalArgumentException - if the work key record number
     * @throws IllegalArgumentException - if the digest data is null
     * @throws IllegalArgumentException - if the request is inconsistent
     */
    DigestInitCmdBuild(SamRevision revision, bool verificationMode,
                       bool rev3_2Mode, uint8_t workKeyRecordNumber,
                       uint8_t workKeyKif, uint8_t workKeyKVC,
                       std::vector<uint8_t>& digestData);

    /**
     *
     */
    virtual ~DigestInitCmdBuild()
    {
    }

protected:
    /**
     *
     */
    std::shared_ptr<DigestInitCmdBuild> shared_from_this()
    {
        return std::static_pointer_cast<DigestInitCmdBuild>(
            AbstractSamCommandBuilder::shared_from_this());
    }

private:
    /**
     * The command.
     */
    CalypsoSamCommands& command = CalypsoSamCommands::DIGEST_INIT;
};

}
}
}
}
}
}
