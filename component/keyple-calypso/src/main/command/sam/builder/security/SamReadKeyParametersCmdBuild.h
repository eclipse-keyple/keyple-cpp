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
 * Builder for the SAM Read Key Parameters APDU command.
 */
class SamReadKeyParametersCmdBuild : public  AbstractSamCommandBuilder {
public:
    /**
     *
     */
    static const int MAX_WORK_KEY_REC_NUMB;

    /**
     *
     */
    enum SourceRef {
        WORK_KEY,
        SYSTEM_KEY
    };

    /**
     *
     */
    enum NavControl {
        FIRST,
        NEXT
    };

    /**
     *
     */
    SamReadKeyParametersCmdBuild(SamRevision& revision);

    /**
     *
     */
    SamReadKeyParametersCmdBuild(SamRevision& revision, uint8_t kif);

    /**
     *
     */
    SamReadKeyParametersCmdBuild(SamRevision& revision, uint8_t kif,
                                 uint8_t kvc);

    /**
     *
     */
    SamReadKeyParametersCmdBuild(SamRevision& revision, SourceRef& sourceKeyRef,
                                 int recordNumber);

    /**
     *
     */
    SamReadKeyParametersCmdBuild(SamRevision& revision, uint8_t kif,
                                 NavControl& navControl);

private:
    /**
     * The command reference
     */
    CalypsoSamCommands& command = CalypsoSamCommands::READ_KEY_PARAMETERS;
};

}
}
}
}
}
}
