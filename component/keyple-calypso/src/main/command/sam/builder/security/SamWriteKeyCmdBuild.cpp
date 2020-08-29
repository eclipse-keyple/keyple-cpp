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

#include "SamWriteKeyCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::common;

const CalypsoSamCommands& SamWriteKeyCmdBuild::mCommand =
    CalypsoSamCommands::WRITE_KEY;

SamWriteKeyCmdBuild::SamWriteKeyCmdBuild(
  const SamRevision& revision, const uint8_t writingMode,
  const uint8_t keyReference, const std::vector<uint8_t>& keyData)
: AbstractSamCommandBuilder(CalypsoSamCommands::WRITE_KEY, nullptr)
{
    mDefaultRevision = revision;

    const uint8_t cla = mDefaultRevision.getClassByte();

    if (keyData.empty())
        throw IllegalArgumentException("Key data null!");

    if (static_cast<int>(keyData.size()) < 48 ||
        static_cast<int>(keyData.size()) > 80)
        throw IllegalArgumentException("Key data should be between 40 and 80 "
                                       "bytes long!");

    mRequest = setApduRequest(cla, command, writingMode, keyReference, keyData);
}

std::shared_ptr<SamWriteKeyRespPars> SamWriteKeyCmdBuild::createResponseParser(
    const std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SamWriteKeyRespPars>(apduResponse, this);
}

}
}
}
}
}
}
