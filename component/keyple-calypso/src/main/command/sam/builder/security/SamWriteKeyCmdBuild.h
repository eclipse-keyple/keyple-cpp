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

#include "AbstractSamCommandBuilder.h"
#include "CalypsoSamCommand.h"
#include "SamRevision.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace sam {
    namespace parser { namespace security { class SamWriteKeyRespPars; } } }
    } } }

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;

/**
 * Builder for the SAM Write Key APDU command.
 */
class SamWriteKeyCmdBuild final
: public AbstractSamCommandBuilder<SamWriteKeyRespPars> {
public:
    /**
     * Builder constructor
     *
     * @param revision the SAM revision
     * @param writingMode the writing mode (P1)
     * @param keyReference the key reference (P2)
     * @param keyData the key data
     */
    SamWriteKeyCmdBuild(const SamRevision& revision,
                        const uint8_t writingMode,
                        const uint8_t keyReference,
                        const std::vector<uint8_t>& keyData);

    /**
     *
     */
    std::shared_ptr<SamWriteKeyRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

private:
    /**
     * The command reference
     */
    const CalypsoSamCommand& mCommand = CalypsoSamCommand::WRITE_KEY;
};

}
}
}
}
}
}
