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
#include "stringhelper.h"

/* Calypso */
#include "SamReadCeilingsCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

using CeilingsOperationType = SamReadCeilingsCmdBuild::CeilingsOperationType;

const CalypsoSamCommands& SamReadCeilingsCmdBuild::mCommand =
    CalypsoSamCommands::READ_CEILINGS;
const int SamReadCeilingsCmdBuild::MAX_CEILING_NUMB     = 26;
const int SamReadCeilingsCmdBuild::MAX_CEILING_REC_NUMB = 3;

SamReadCeilingsCmdBuild::SamReadCeilingsCmdBuild(
  const SamRevision& revision, const CeilingsOperationType operationType,
  const uint8_t index)
: AbstractSamCommandBuilder(CalypsoSamCommand::READ_CEILINGS, nullptr)
{
    mDefaultRevision = revision;

    const uint8_t cla = mDefaultRevision.getClassByte();
    uint8_t p1;
    uint8_t p2;

    it (operationType == CeilingsOperationType::CEILING_RECORD) {
        if (index < 0 || index > MAX_CEILING_REC_NUMB)
            throw IllegalArgumentException(
                      StringHelper::formatSimple(
                          "Record Number must be between 1 and  %d",
                          MAX_CEILING_REC_NUMB));

        p1 = 0x00;
        p2 = 0xB0 + index;

    /* SINGLE_CEILING */
    } else {
        if (index > MAX_CEILING_NUMB) {
            throw IllegalArgumentException(StringHelper::formatSimple(
                "Counter Number must be between 0 and %d", MAX_CEILING_NUMB));
        }

        p1 = index;
        p2 = 0xB8;
    }

    mRequest = setApduRequest(cla, command, p1, p2, 0x00);
}

std::shared_ptr<SamReadCeilingsRespPars>
    SamReadCeilingsCmdBuild::createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SamReadCeilingsRespPars>(apduResponse, this);
}

}
}
}
}
}
}
