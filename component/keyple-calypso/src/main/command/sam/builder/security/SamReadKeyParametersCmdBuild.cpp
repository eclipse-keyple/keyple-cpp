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

#include "SamReadKeyParametersCmdBuild.h"

/* Common */
#include "IllegalArgumentException.h"
#include "IllegalStateException.h"

/* Calypso */
#include "SamReadKeyParametersRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser::security;

SamReadKeyParametersCmdBuild::SamReadKeyParametersCmdBuild(
  const SamRevision& revision)
: AbstractSamCommandBuilder(
    std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::READ_KEY_PARAMETERS),
    nullptr)
{
    mDefaultRevision = revision;

    const uint8_t cla = mDefaultRevision.getClassByte();
    const uint8_t p2  = 0xE0;
    const std::vector<uint8_t> sourceKeyId(0x00, 0x00);

    mRequest = setApduRequest(cla, mCommand, 0x00, p2, sourceKeyId, 0x00);
}

SamReadKeyParametersCmdBuild::SamReadKeyParametersCmdBuild(
  const SamRevision& revision, const uint8_t kif)
: AbstractSamCommandBuilder(
    std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::READ_KEY_PARAMETERS),
    nullptr)
{
    mDefaultRevision = revision;

    const uint8_t cla = mDefaultRevision.getClassByte();
    const uint8_t p2  = 0xC0;

    std::vector<uint8_t> sourceKeyId(0x00, 0x00);
    sourceKeyId[0] = kif;

    mRequest = setApduRequest(cla, mCommand, 0x00, p2, sourceKeyId, 0x00);
}

SamReadKeyParametersCmdBuild::SamReadKeyParametersCmdBuild(
  const SamRevision& revision, const uint8_t kif, const uint8_t kvc)
: AbstractSamCommandBuilder(
    std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::READ_KEY_PARAMETERS),
    nullptr)
{
    mDefaultRevision = revision;

    const uint8_t cla = mDefaultRevision.getClassByte();
    const uint8_t p2  = 0xF0;

    std::vector<uint8_t> sourceKeyId(0x00, 0x00);
    sourceKeyId[0] = kif;
    sourceKeyId[1] = kvc;

    mRequest = setApduRequest(cla, mCommand, 0x00, p2, sourceKeyId, 0x00);
}

SamReadKeyParametersCmdBuild::SamReadKeyParametersCmdBuild(
  const SamRevision& revision,
  const SourceRef& sourceKeyRef,
  const uint8_t recordNumber)
: AbstractSamCommandBuilder(
    std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::READ_KEY_PARAMETERS),
    nullptr)
{

    if (revision != SamRevision::NO_REV)
        mDefaultRevision = revision;

    if (recordNumber < 1 || recordNumber > MAX_WORK_KEY_REC_NUMB)
        throw IllegalArgumentException(
                  "Record Number must be between 1 and " +
                  std::to_string(MAX_WORK_KEY_REC_NUMB));


    const uint8_t cla = mDefaultRevision.getClassByte();
    uint8_t p2 = 0x00;
    const std::vector<uint8_t> sourceKeyId(0x00, 0x00);

    switch (sourceKeyRef) {
    case WORK_KEY:
        p2 = recordNumber;
        break;
    case SYSTEM_KEY:
        p2 = 0xC0 + recordNumber;
        break;
    default:
        throw IllegalStateException(
                  "Unsupported SourceRef parameter " +
                  std::to_string(sourceKeyRef));
    }

    mRequest = setApduRequest(cla, mCommand, 0x00, p2, sourceKeyId, 0x00);
}

SamReadKeyParametersCmdBuild::SamReadKeyParametersCmdBuild(
  const SamRevision& revision, const uint8_t kif, const NavControl& navControl)
: AbstractSamCommandBuilder(
    std::make_shared<CalypsoSamCommand>(CalypsoSamCommand::READ_KEY_PARAMETERS),
    nullptr)
{
    mDefaultRevision = revision;

    const uint8_t cla = mDefaultRevision.getClassByte();
    uint8_t p2  = 0x00;
    std::vector<uint8_t> sourceKeyId(0x00, 0x00);

    switch (navControl) {
    case FIRST:
        p2 = 0xF8;
        break;
    case NEXT:
        p2 = 0xFA;
        break;
    default:
        throw IllegalStateException(
                  "Unsupported NavControl parameter " +
                  std::to_string(navControl));
    }

    sourceKeyId[0] = kif;

    mRequest = setApduRequest(cla, mCommand, 0x00, p2, sourceKeyId, 0x00);
}

std::shared_ptr<SamReadKeyParametersRespPars>
    SamReadKeyParametersCmdBuild::createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<SamReadKeyParametersRespPars>(apduResponse, this);
}

}
}
}
}
}
}
