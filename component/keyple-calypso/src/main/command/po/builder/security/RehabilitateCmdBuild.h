/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#pragma once

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "PoClass.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace po {
    namespace parser { namespace security { class RehabilitateRespPars; } } } }
    } }

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {
namespace security {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser::security;

/**
 * The Class {@link RehabilitateCmdBuild}. This class provides the dedicated constructor to build
 * the PO Rehabilitate command.
 */
class KEYPLECALYPSO_API RehabilitateCmdBuild final
: public AbstractPoCommandBuilder<RehabilitateRespPars> {
public:
    /**
     * Instantiates a new RehabilitateCmdBuild.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     */
    RehabilitateCmdBuild(const PoClass& poClass);

    /**
     *
     */
    std::shared_ptr<RehabilitateRespPars> createResponseParser(
	    std::shared_ptr<ApduResponse> apduResponse) override;

    /**
     * This command modified the contents of the PO and therefore uses the session buffer.
     *
     * @return true
     */
    virtual bool isSessionBufferUsed() const override;

private:
    /**
     * The command
     */
    const CalypsoPoCommand& mCommand = CalypsoPoCommand::REHABILITATE;
};

}
}
}
}
}
}
