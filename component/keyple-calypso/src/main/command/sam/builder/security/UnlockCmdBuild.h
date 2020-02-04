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
 * Builder for the SAM Unlock APDU command.
 */
class UnlockCmdBuild : public AbstractSamCommandBuilder {
public:
    /**
     * Builder constructor
     *
     * @param revision the SAM revision
     * @param unlockData the unlock data
     */
     UnlockCmdBuild(const SamRevision& revision,
                    const std::vector<uint8_t>& unlockData);

private:
    /**
     * The command reference
     */
    CalypsoSamCommands& command = CalypsoSamCommands::UNLOCK;
};

}
}
}
}
}
}
