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

#include <vector>
#include <stdexcept>
#include <memory>

#include "AbstractSamCommandBuilder.h"
#include "CalypsoSamCommands.h"
#include "SamRevision.h"


namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using AbstractSamCommandBuilder = org::eclipse::keyple::calypso::command::sam::AbstractSamCommandBuilder;
using CalypsoSamCommands = org::eclipse::keyple::calypso::command::sam::CalypsoSamCommands;
using SamRevision = org::eclipse::keyple::calypso::command::sam::SamRevision;

/**
 * Builder for the Digest Authenticate APDU command.
 */
class DigestAuthenticateCmdBuild : public AbstractSamCommandBuilder {

    /** The command. */
private:
    const CalypsoSamCommands command = CalypsoSamCommands::DIGEST_AUTHENTICATE;

    /**
     * Instantiates a new DigestAuthenticateCmdBuild .
     *
     * @param revision of the SAM
     * @param signature the signature
     * @throws IllegalArgumentException - if the signature is null or has a wrong length.
     */
public:
    DigestAuthenticateCmdBuild(SamRevision revision, std::vector<char> &signature);

protected:
    std::shared_ptr<DigestAuthenticateCmdBuild> shared_from_this() {
        return std::static_pointer_cast<DigestAuthenticateCmdBuild>(AbstractSamCommandBuilder::shared_from_this());
    }
};

}
}
}
}
}
}
}
}
