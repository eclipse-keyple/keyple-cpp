/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <stdexcept>
#include <memory>

#include "AbstractSamCommandBuilder.h"
#include "CalypsoSamCommands.h"
#include "SamRevision.h"

/* Common */
#include "stringhelper.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

/**
 * Builder for the SAM Get Challenge APDU command.
 */
class SamGetChallengeCmdBuild : public AbstractSamCommandBuilder {
public:
    /**
     * Instantiates a new SamGetChallengeCmdBuild.
     *
     * @param revision of the SAM (SAM)
     * @param expectedResponseLength the expected response length
     * @throws IllegalArgumentException - if the expected response length has
     *         wrong value.
     */
    SamGetChallengeCmdBuild(SamRevision revision,
                            uint8_t expectedResponseLength);

    /**
     *
     */
    virtual ~SamGetChallengeCmdBuild()
    {
    }

protected:
    /**
     *
     */
    std::shared_ptr<SamGetChallengeCmdBuild> shared_from_this()
    {
        return std::static_pointer_cast<SamGetChallengeCmdBuild>(
            AbstractSamCommandBuilder::shared_from_this());
    }

private:
    /**
     * The command reference
     */
    CalypsoSamCommands& command = CalypsoSamCommands::GET_CHALLENGE;
};

}
}
}
}
}
}
