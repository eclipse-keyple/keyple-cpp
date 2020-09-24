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

#include "SamReadCeilingsCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"
#include "stringhelper.h"

/* Calypso */
#include "SamReadCeilingsRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::common;

using CeilingsOperationType = SamReadCeilingsCmdBuild::CeilingsOperationType;

SamReadCeilingsCmdBuild::SamReadCeilingsCmdBuild(
  const SamRevision& revision,
  const CeilingsOperationType operationType,
  const uint8_t index)
: AbstractSamCommandBuilder(
    std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::READ_CEILINGS),
    nullptr)
{
    mDefaultRevision = revision;

    const uint8_t cla = mDefaultRevision.getClassByte();
    uint8_t p1;
    uint8_t p2;

    if (operationType == CeilingsOperationType::CEILING_RECORD) {
        if (static_cast<int8_t>(index) < 0 ||
            static_cast<int8_t>(index) > MAX_CEILING_REC_NUMB)
            throw IllegalArgumentException(
                      StringHelper::formatSimple(
                          "Record Number must be between 1 and  %d",
                          MAX_CEILING_REC_NUMB));

        p1 = 0x00;
        p2 = 0xB0 + index;

    /* SINGLE_CEILING */
    } else {
        if (index > MAX_CEILING_NUMB) {
            throw IllegalArgumentException(
                      StringHelper::formatSimple(
                          "Counter Number must be between 0 and %d",
                          MAX_CEILING_NUMB));
        }

        p1 = index;
        p2 = 0xB8;
    }

    mRequest = setApduRequest(cla, mCommand, p1, p2, 0x00);
}

std::unique_ptr<SamReadCeilingsRespPars>
    SamReadCeilingsCmdBuild::createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse)
{
    return std::unique_ptr<SamReadCeilingsRespPars>(
               new SamReadCeilingsRespPars(apduResponse, this));
}

}
}
}
}
}
}
