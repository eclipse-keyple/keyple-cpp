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

#include <memory>

#include "AbstractPoCommandBuilder.h"
#include "PoClass.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace po {
    namespace parser { namespace security { class PoGetChallengeRespPars; } } }
    } } }

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser::security;
using namespace keyple::core::seproxy::message;

/**
 * The Class PoGetChallengeCmdBuild. This class provides the dedicated
 * constructor to build the PO Get Challenge.
 */
class KEYPLECALYPSO_API PoGetChallengeCmdBuild final
: public AbstractPoCommandBuilder<PoGetChallengeRespPars> {
public:
    /**
     * Instantiates a new PoGetChallengeCmdBuild.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     */
    PoGetChallengeCmdBuild(PoClass poClass);

    /**
     *
     */
    std::shared_ptr<PoGetChallengeRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

    /**
     *
     * This command can't be executed in session and therefore doesn't uses the
     * session buffer.
     *
     * @return false
     */
    virtual bool isSessionBufferUsed() const override;

private:
    /**
     *
     */
    const CalypsoPoCommand& command = CalypsoPoCommand::GET_CHALLENGE;
};

}
}
}
}
}
}
