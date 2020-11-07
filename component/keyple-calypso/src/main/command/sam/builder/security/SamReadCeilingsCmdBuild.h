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
#include "AbstractSamCommandBuilder.h"
#include "CalypsoSamCommand.h"
#include "SamRevision.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace sam {
    namespace parser { namespace security { class SamReadCeilingsRespPars; } } }
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
 * Builder for the SAM Read Ceilings APDU command.
 */
class SamReadCeilingsCmdBuild final
: public AbstractSamCommandBuilder<SamReadCeilingsRespPars> {
public:
    /**
     *
     */
    static const int MAX_CEILING_NUMB = 26;

    /**
     *
     */
    static const int MAX_CEILING_REC_NUMB = 3;

    /**
     *
     */
    enum class CeilingsOperationType {
        CEILING_RECORD,
        SINGLE_CEILING
    };

    /**
     * Instantiates a new SamReadCeilingsCmdBuild.
     *
     * @param revision revision of the SAM
     * @param operationType the counter operation type
     * @param index the counter index
     */
    SamReadCeilingsCmdBuild(const SamRevision& revision,
                            const CeilingsOperationType operationType,
                            const uint8_t index);

    /**
     *
     */
    std::shared_ptr<SamReadCeilingsRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

private:
    /**
     * The command reference
     */
    const CalypsoSamCommand& mCommand = CalypsoSamCommand::READ_CEILINGS;
};

}
}
}
}
}
}