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

/* Calypso */
#include "AbstractSamCommandBuilder.h"
#include "CalypsoSamCommand.h"
#include "KeypleCalypsoExport.h"
#include "SamRevision.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace sam {
    namespace parser { namespace security { class SamReadEventCounterRespPars; }
    } } } } }

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;

/**
 * Builder for the SAM Unlock APDU command.
 */
class KEYPLECALYPSO_API SamReadEventCounterCmdBuild final
: public AbstractSamCommandBuilder<SamReadEventCounterRespPars> {
public:
    /**
     *
     */
    static const int MAX_COUNTER_NUMB = 26;

    /**
     *
     */
    static const int MAX_COUNTER_REC_NUMB = 3;

    /**
     *
     */
    enum SamEventCounterOperationType { COUNTER_RECORD, SINGLE_COUNTER };

    /**
     * Instantiate a new SamReadEventCounterCmdBuild
     *
     * @param revision revision of the SAM
     * @param operationType the counter operation type
     * @param index the counter index
     */
    SamReadEventCounterCmdBuild(
        const SamRevision& revision,
        const SamEventCounterOperationType operationType, const uint8_t index);

    /**
     *
     */
    std::shared_ptr<SamReadEventCounterRespPars> createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse) override;

private:
    /**
     * The command reference
     */
    const CalypsoSamCommand& mCommand = CalypsoSamCommand::READ_EVENT_COUNTER;
};

}
}
}
}
}
}
