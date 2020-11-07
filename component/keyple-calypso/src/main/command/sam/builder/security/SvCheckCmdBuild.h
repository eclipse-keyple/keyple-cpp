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

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace sam {
    namespace parser { namespace security { class SvCheckRespPars; } } } }
    } }

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;

/**
 * Builder for the SAM SV Check APDU command.
 */
class SvCheckCmdBuild final : public AbstractSamCommandBuilder<SvCheckRespPars> {
public:
    /**
     * Instantiates a new SvCheckCmdBuild to authenticate a card SV transaction.
     *
     * @param revision of the SAM
     * @param svPoSignature null if the operation is to abort the SV transaction, a 3 or 6-byte
     *        array containing the PO signature from SV Debit, SV Load or SV Undebit.
     */
    SvCheckCmdBuild(const SamRevision& revision, const std::vector<uint8_t>& svPoSignature);

    /**
     *
     */
    std::shared_ptr<SvCheckRespPars> createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse) override;

private:
    /**
     * The command
     */
    const CalypsoSamCommand& mCommand = CalypsoSamCommand::SV_CHECK;
};

}
}
}
}
}
}
