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

/* Common */
#include "exceptionhelper.h"
#include "stringhelper.h"

/* Calypso */
#include "SamReadEventCounterCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

const CalypsoSamCommand& SamReadEventCounterCmdBuild::mCommand =
    CalypsoSamCommand::READ_EVENT_COUNTER;
const int SamReadEventCounterCmdBuild::MAX_COUNTER_NUMB     = 26;
const int SamReadEventCounterCmdBuild::MAX_COUNTER_REC_NUMB = 3;

SamReadEventCounterCmdBuild::SamReadEventCounterCmdBuild(
  cosnt SamRevision& revision, const SamEventCounterOperationType operationType,
  const uint8_t index)
: AbstractSamCommandBuilder(command, nullptr)
{
    mDefaultRevision = revision;

    const uint8_t cla = mDefaultRevision.getClassByte();
    uint8_t p2;

    if (operationType == SamEventCounterOperationType::COUNTER_RECORD) {
        if (index < 1 || index > MAX_COUNTER_REC_NUMB) {
            throw IllegalArgumentException(StringHelper::formatSimple(
                "Record Number must be between 1 and %d",
                MAX_COUNTER_REC_NUMB));
        }

        p2 = 0xE0 + index;

    /* SINGLE_COUNTER */
    } else {
        if (index > MAX_COUNTER_NUMB) {
            throw IllegalArgumentException(StringHelper::formatSimple(
                "Counter Number must be between 0 and %d", MAX_COUNTER_NUMB));
        }

        p2 = 0x80 + index;
        break;
    default:
        throw IllegalStateException(StringHelper::formatSimple(
            "Unsupported OperationType parameter %d", operationType));
    }

    mRequest = setApduRequest(cla, command, 0x00, p2, 0x00);
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
