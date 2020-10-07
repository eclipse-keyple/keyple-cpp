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

#include "SamReadEventCounterCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"
#include "IllegalStateException.h"

/* Calypso */
#include "SamReadEventCounterRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::common;

SamReadEventCounterCmdBuild::SamReadEventCounterCmdBuild(
  const SamRevision& revision,
  const SamEventCounterOperationType operationType,
  const uint8_t index)
: AbstractSamCommandBuilder(
    std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::READ_EVENT_COUNTER),
    nullptr)
{
    if (revision != SamRevision::NO_REV)
        mDefaultRevision = revision;

    const uint8_t cla = mDefaultRevision.getClassByte();
    uint8_t p2;

    if (operationType == SamEventCounterOperationType::COUNTER_RECORD) {
        if (index < 1 || index > MAX_COUNTER_REC_NUMB) {
            throw IllegalArgumentException(
                      "Record Number must be between 1 and " +
                      std::to_string(MAX_COUNTER_REC_NUMB));
        }

        p2 = 0xE0 + index;

    /* SINGLE_COUNTER */
    } else {
        if (index > MAX_COUNTER_NUMB) {
            throw IllegalArgumentException(
                     "Counter Number must be between 0 and " +
                     std::to_string(MAX_COUNTER_NUMB));
        }

        p2 = 0x80 + index;
    }

    mRequest = setApduRequest(cla, mCommand, 0x00, p2, 0x00);
}

std::shared_ptr<SamReadEventCounterRespPars>
    SamReadEventCounterCmdBuild::createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SamReadEventCounterRespPars>(apduResponse, this);
}

}
}
}
}
}
}
