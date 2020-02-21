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
 * Builder for the SAM Give Random APDU command.
 */
class GiveRandomCmdBuild : public AbstractSamCommandBuilder {
public:
    /**
     * Instantiates a new DigestUpdateCmdBuild.
     *
     * @param revision of the SAM
     * @param random the random data
     * @throws IllegalArgumentException - if the random data is null or has a
     *         length not equal to 8
     *
     *         TODO implement specific settings for rev < 3
     */
    GiveRandomCmdBuild(SamRevision revision, std::vector<uint8_t>& random);

protected:
    /**
     *
     */
    std::shared_ptr<GiveRandomCmdBuild> shared_from_this()
    {
        return std::static_pointer_cast<GiveRandomCmdBuild>(
                   AbstractSamCommandBuilder::shared_from_this());
    }

private:
    /**
     * The command reference
     */
    CalypsoSamCommands& command = CalypsoSamCommands::GIVE_RANDOM;

};

}
}
}
}
}
}
