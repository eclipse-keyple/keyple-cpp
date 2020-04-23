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

/* Calypso */
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
 * Builder for the SAM Read Ceilings APDU command.
 */
class SamReadCeilingsCmdBuild : public AbstractSamCommandBuilder {
public:
    /**
     *
     */
    static const int MAX_CEILING_NUMB;

    /**
     *
     */
    static const int MAX_CEILING_REC_NUMB;

    /**
     *
     */
    enum CeilingsOperationType {
        CEILING_RECORD,

        SINGLE_CEILING
    };

    /**
     *
     */
    SamReadCeilingsCmdBuild(SamRevision& revision,
                            CeilingsOperationType operationType, int index);

private:
    /**
     * The command reference
     */
    static const CalypsoSamCommands& command;
};

}
}
}
}
}
}