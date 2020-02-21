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
#include "SamReadKeyParametersCmdBuild.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace builder {
namespace security {

using namespace keyple::calypso::command::sam;

const int SamReadKeyParametersCmdBuild::MAX_WORK_KEY_REC_NUMB = 126;

SamReadKeyParametersCmdBuild::SamReadKeyParametersCmdBuild(
  SamRevision& revision)
: AbstractSamCommandBuilder(CalypsoSamCommands::READ_KEY_PARAMETERS, nullptr)
{
    this->defaultRevision = revision;

    uint8_t cla = this->defaultRevision.getClassByte();
    uint8_t p2 = 0xE0;
    std::vector<uint8_t> sourceKeyId(0x00, 0x00);

    request = setApduRequest(cla, command, 0x00, p2, sourceKeyId, 0x00);
}

SamReadKeyParametersCmdBuild::SamReadKeyParametersCmdBuild(
  SamRevision& revision, uint8_t kif)
: AbstractSamCommandBuilder(CalypsoSamCommands::READ_KEY_PARAMETERS, nullptr)
{
    this->defaultRevision = revision;

    uint8_t cla = this->defaultRevision.getClassByte();
    uint8_t p2 = 0xC0;
    std::vector<uint8_t> sourceKeyId(0x00, 0x00);

    sourceKeyId[0] = kif;

    request = setApduRequest(cla, command, 0x00, p2, sourceKeyId, 0x00);
}

SamReadKeyParametersCmdBuild::SamReadKeyParametersCmdBuild(
  SamRevision& revision, uint8_t kif, uint8_t kvc)
: AbstractSamCommandBuilder(CalypsoSamCommands::READ_KEY_PARAMETERS, nullptr)
{
    this->defaultRevision = revision;

    uint8_t cla = this->defaultRevision.getClassByte();
    uint8_t p2 = 0xF0;
    std::vector<uint8_t> sourceKeyId(0x00, 0x00);

    sourceKeyId[0] = kif;
    sourceKeyId[1] = kvc;

    request = setApduRequest(cla, command, 0x00, p2, sourceKeyId, 0x00);
}

SamReadKeyParametersCmdBuild::SamReadKeyParametersCmdBuild(
  SamRevision& revision, SourceRef& sourceKeyRef, int recordNumber)
: AbstractSamCommandBuilder(CalypsoSamCommands::READ_KEY_PARAMETERS, nullptr)
{

    this->defaultRevision = revision;

    if (recordNumber < 1 || recordNumber > MAX_WORK_KEY_REC_NUMB) {
        throw IllegalArgumentException(
                  StringHelper::formatSimple(
                      "Record Number must be between 1 and %d",
                      MAX_WORK_KEY_REC_NUMB));
    }

    uint8_t cla = this->defaultRevision.getClassByte();

    uint8_t p2 = 0x00;
    std::vector<uint8_t> sourceKeyId(0x00, 0x00);

    switch (sourceKeyRef) {
    case WORK_KEY:
        p2 = recordNumber;
        break;
    case SYSTEM_KEY:
        p2 = 0xC0 + recordNumber;
        break;
    default:
        throw IllegalStateException(
                  StringHelper::formatSimple(
                      "Unsupported SourceRef parameter %d",
                      sourceKeyRef));
    }

    request = setApduRequest(cla, command, 0x00, p2, sourceKeyId, 0x00);
}

SamReadKeyParametersCmdBuild::SamReadKeyParametersCmdBuild(
  SamRevision& revision, uint8_t kif, NavControl& navControl)
: AbstractSamCommandBuilder(CalypsoSamCommands::READ_KEY_PARAMETERS, nullptr)
{

    this->defaultRevision = revision;

    uint8_t cla = this->defaultRevision.getClassByte();
    uint8_t p2 = 0x00;
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
                  StringHelper::formatSimple(
                      "Unsupported NavControl parameter %d",
                      navControl));
    }

    sourceKeyId[0] = kif;

    request = setApduRequest(cla, command, 0x00, p2, sourceKeyId, 0x00);
}

}
}
}
}
}
}
