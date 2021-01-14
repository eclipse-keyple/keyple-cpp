/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "ReadRecordsCmdBuild.h"

/* Common */
#include "stringhelper.h"

/* Core */
#include "ApduResponse.h"

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "ReadRecordsRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::common;
using namespace keyple::core::seproxy::message;

ReadRecordsCmdBuild::ReadRecordsCmdBuild(
  const PoClass& poClass,
  const uint8_t sfi,
  const uint8_t firstRecordNumber,
  const ReadMode readMode,
  const uint8_t expectedLength)
: AbstractPoCommandBuilder<ReadRecordsRespPars>(
      std::make_shared<CalypsoPoCommand>(CalypsoPoCommand::READ_RECORDS),
      nullptr),
  mSfi(sfi),
  mFirstRecordNumber(firstRecordNumber),
  mReadMode(readMode)
{
    const uint8_t p1 = firstRecordNumber;
    uint8_t p2 = sfi == 0x00 ? 0x05 : (sfi * 8) + 5;
    if (readMode == ReadMode::ONE_RECORD)
        p2 -= 0x01;
    const uint8_t le = expectedLength;

    mRequest = setApduRequest(poClass.getValue(), command, p1, p2, le);

    const std::string extraInfo =
        "SFI=" + StringHelper::uint8ToHexString(sfi) + ", " +
        "REC=" + std::to_string(firstRecordNumber) + ", " +
        "READMODE=" + StringHelper::toString(readMode) +
        "EXPECTEDLENGTH=" + std::to_string(expectedLength);
    addSubName(extraInfo);
}

std::shared_ptr<ReadRecordsRespPars> ReadRecordsCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<ReadRecordsRespPars>(apduResponse, this);
}

bool ReadRecordsCmdBuild::isSessionBufferUsed() const
{
    return false;
}

uint8_t ReadRecordsCmdBuild::getSfi() const
{
    return mSfi;
}

uint8_t ReadRecordsCmdBuild::getFirstRecordNumber() const
{
    return mFirstRecordNumber;
}

ReadRecordsCmdBuild::ReadMode ReadRecordsCmdBuild::getReadMode() const
{
    return mReadMode;
}

std::ostream& operator<<(std::ostream& os,
                         const ReadRecordsCmdBuild::ReadMode& rm)
{
    switch (rm) {
    case ReadRecordsCmdBuild::ReadMode::ONE_RECORD:
        os << "ONE RECORD";
        break;
    case ReadRecordsCmdBuild::ReadMode::MULTIPLE_RECORD:
        os << "MULTIPLE RECORD";
        break;
    default:
        os << "UNKNOWN READ MODE";
        break;
    }

    return os;
}

}
}
}
}
}
