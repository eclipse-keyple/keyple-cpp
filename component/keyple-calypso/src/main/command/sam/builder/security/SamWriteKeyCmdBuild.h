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
 * Builder for the SAM Write Key APDU command.
 */
class SamWriteKeyCmdBuild : public AbstractSamCommandBuilder {
public:
    /**
     * Builder constructor
     *
     * @param revision the SAM revision
     * @param writingMode the writing mode (P1)
     * @param keyReference the key reference (P2)
     * @param keyData the key data
     */
    SamWriteKeyCmdBuild(SamRevision& revision, uint8_t writingMode,
                        uint8_t keyReference, std::vector<uint8_t>& keyData);

private:
    /**
     * The command reference
     */
    CalypsoSamCommands command = CalypsoSamCommands::WRITE_KEY;

};

}
}
}
}
}
}
