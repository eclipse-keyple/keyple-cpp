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
 * This class provides the dedicated constructor to build the SAM Digest Update
 * Multiple APDU command.
 *
 */
class DigestUpdateMultipleCmdBuild : public AbstractSamCommandBuilder {
public:
    /**
     * Instantiates a new DigestUpdateMultipleCmdBuild.
     *
     * @param revision the revision
     * @param digestData the digest data
     * @throws IllegalArgumentException - if the request is inconsistent
     */
    DigestUpdateMultipleCmdBuild(SamRevision revision,
                                 std::vector<uint8_t>& digestData);

protected:
    /**
     *
     */
    std::shared_ptr<DigestUpdateMultipleCmdBuild> shared_from_this()
    {
        return std::static_pointer_cast<DigestUpdateMultipleCmdBuild>(
            AbstractSamCommandBuilder::shared_from_this());
    }

private:
    /**
     * The command
     */
    CalypsoSamCommands& command = CalypsoSamCommands::DIGEST_UPDATE_MULTIPLE;
};

}
}
}
}
}
}
