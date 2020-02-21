/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
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

/* Common */
#include "Export.h"

#include "AbstractSamCommandBuilder.h"
#include "CalypsoSamCommands.h"
#include "SamRevision_Import.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

/**
 * Builder for the SAM Unlock APDU command.
 */
class EXPORT SamReadEventCounterCmdBuild : public AbstractSamCommandBuilder {
public:
    /**
     *
     */
    static const int MAX_COUNTER_NUMB;

    /**
     *
     */
    static const int MAX_COUNTER_REC_NUMB;

    /**
     *
     */
    enum SamEventCounterOperationType { COUNTER_RECORD, SINGLE_COUNTER };

    /**
     *
     */
    SamReadEventCounterCmdBuild(SamRevision& revision,
                                SamEventCounterOperationType operationType,
                                int index);

private:
    /**
     * The command reference
     */
    CalypsoSamCommands command = CalypsoSamCommands::READ_EVENT_COUNTER;
};

}
}
}
}
}
}
