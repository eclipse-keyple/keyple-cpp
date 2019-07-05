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

#include <memory>

#include "AbstractPoCommandBuilder.h"
#include "CalypsoPoCommands.h"
#include "PoClass.h"
#include "PoGetChallengeRespPars.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using namespace org::eclipse::keyple::calypso::command::po;

using PoClass                = org::eclipse::keyple::calypso::command::PoClass;
using CalypsoPoCommands      = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
using PoGetChallengeRespPars = org::eclipse::keyple::calypso::command::po::parser::security::PoGetChallengeRespPars;
using ApduResponse           = org::eclipse::keyple::core::seproxy::message::ApduResponse;

/**
 * The Class PoGetChallengeCmdBuild. This class provides the dedicated constructor to build the PO
 * Get Challenge.
 */
class PoGetChallengeCmdBuild final : public AbstractPoCommandBuilder<PoGetChallengeRespPars> {

private:
    const CalypsoPoCommands command = CalypsoPoCommands::GET_CHALLENGE;

    /**
     * Instantiates a new PoGetChallengeCmdBuild.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     */
public:
    PoGetChallengeCmdBuild(PoClass poClass);

    std::shared_ptr<PoGetChallengeRespPars> createResponseParser(std::shared_ptr<ApduResponse> apduResponse) override;

protected:
    std::shared_ptr<PoGetChallengeCmdBuild> shared_from_this() {
        return std::static_pointer_cast<PoGetChallengeCmdBuild>(AbstractPoCommandBuilder<PoGetChallengeRespPars>::shared_from_this());
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
