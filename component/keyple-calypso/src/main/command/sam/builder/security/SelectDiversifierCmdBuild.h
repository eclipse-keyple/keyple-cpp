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
#include "KeypleCalypsoExport.h"
#include "SamRevision.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace sam {
    namespace parser { namespace security { class SelectDiversifierRespPars; } } } }
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
 * This class provides the dedicated constructor to build the SAM Select
 * Diversifier APDU command.
 */
class KEYPLECALYPSO_API SelectDiversifierCmdBuild final
: public AbstractSamCommandBuilder<SelectDiversifierRespPars> {
public:
    /**
     * Instantiates a new SelectDiversifierCmdBuild.
     *
     * @param revision the SAM revision
     * @param diversifier the application serial number
     * @throw IllegalArgumentException - if the diversifier is null or has a
     *        wrong length
     */
    SelectDiversifierCmdBuild(const SamRevision& revision,
                              const std::vector<uint8_t>& diversifier);

    /**
     *
     */
    std::shared_ptr<SelectDiversifierRespPars> createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse) override;

private:
    /**
     * The command
     */
    const CalypsoSamCommand& mCommand = CalypsoSamCommand::SELECT_DIVERSIFIER;
};

}
}
}
}
}
}
