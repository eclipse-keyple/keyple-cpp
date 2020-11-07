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

#include "AbstractSamCommandBuilder.h"
#include "CalypsoSamCommand.h"
#include "SamRevision.h"
#include "SvPrepareOperationRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;

/**
 * Builder for the SAM SV Undebit APDU command.
 */
class SvPrepareUndebitCmdBuild final : public AbstractSamCommandBuilder<SvPrepareOperationRespPars> {
public:
    /**
     * Instantiates a new SvPrepareUndebitCmdBuild to prepare a transaction to cancel a previous
     * debit transaction.
     *
     * @param samRevision the SAM revision
     * @param svGetHeader the SV Get command header
     * @param svGetData a byte array containing the data from the SV get command and response
     * @param svUndebitCmdBuildDebitCmdBuildData the SV undebit command builder data
     */
    SvPrepareUndebitCmdBuild(const SamRevision& samRevision,
                             const std::vector<uint8_t>& svGetHeader,
                             const std::vector<uint8_t>& svGetData,
                             const std::vector<uint8_t>& svUndebitCmdBuildDebitCmdBuildData);

    /**
     *
     */
    std::shared_ptr<SvPrepareOperationRespPars> createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse) override;

private:
    /**
     * The command
     */
    const CalypsoSamCommand& mCommand = CalypsoSamCommand::SV_PREPARE_UNDEBIT;
};

}
}
}
}
}
}
