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
 * Builder for the SAM Digest Update APDU command. This command have to be sent twice for each
 * command executed during a session. First time for the command sent and second time for the answer
 * received
 */
class DigestUpdateCmdBuild : public AbstractSamCommandBuilder {
private:
    /**
     * The command reference
     */
    const CalypsoSamCommands command = CalypsoSamCommands::DIGEST_UPDATE;

public:
    /**
     * Instantiates a new DigestUpdateCmdBuild.
     *
     * @param revision of the SAM
     * @param encryptedSession the encrypted session
     * @param digestData all bytes from command sent by the PO or response from the command
     * @throws IllegalArgumentException - if the digest data is null or has a length &gt; 255
     * @throws IllegalArgumentException - if the request is inconsistent
     */
    DigestUpdateCmdBuild(SamRevision revision, bool encryptedSession, std::vector<char> &digestData);

    /**
     *
     */
    virtual ~DigestUpdateCmdBuild() {}

protected:
    /**
     *
     */
    std::shared_ptr<DigestUpdateCmdBuild> shared_from_this()
    {
        return std::static_pointer_cast<DigestUpdateCmdBuild>(AbstractSamCommandBuilder::shared_from_this());
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
