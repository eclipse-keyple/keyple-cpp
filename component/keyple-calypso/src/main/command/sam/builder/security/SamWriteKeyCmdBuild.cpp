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

/* Common */
#include "exceptionhelper.h"

/* Calypso */
#include "SamWriteKeyCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

SamWriteKeyCmdBuild::SamWriteKeyCmdBuild(SamRevision& revision,
                                         uint8_t writingMode,
                                         uint8_t keyReference,
                                         std::vector<uint8_t>& keyData)
: AbstractSamCommandBuilder(CalypsoSamCommands::WRITE_KEY, nullptr)
{
    this->defaultRevision = revision;

    uint8_t cla = this->defaultRevision.getClassByte();

    if (keyData.empty())
        throw IllegalArgumentException("Key data null!");

    if (keyData.size() < 48 && keyData.size() > 80)
        throw IllegalArgumentException("Key data should be between 40 and 80 "
                                       "bytes long!");

    request = setApduRequest(cla, command, writingMode, keyReference, keyData);
}

}
}
}
}
}
}
