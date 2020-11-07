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
namespace keyple { namespace calypso { namespace command { namespace po { namespace parser {
    namespace security { class InvalidateRespPars; } } } } } }

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
 * The Class {@link InvalidateCmdBuild}. This class provides the dedicated constructor to build the
 * PO Invalidate command.
 */
class KEYPLECALYPSO_API InvalidateCmdBuild final
: public AbstractPoCommandBuilder<InvalidateRespPars> {
public:
    /**
     * Instantiates a new InvalidateCmdBuild.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     */
    InvalidateCmdBuild(const PoClass& poClass);

    /**
     *
     */
    std::shared_ptr<InvalidateRespPars> createResponseParser(
	    std::shared_ptr<ApduResponse> apduResponse) override;

    /**
     * This command can't be executed in session and therefore doesn't uses the session buffer.
     *
     * @return false
     */
    virtual bool isSessionBufferUsed() const override;

private:
    /**
     * The command
     */
    const CalypsoPoCommand& mCommand = CalypsoPoCommand::INVALIDATE;
};

}
}
}
}
}
}
