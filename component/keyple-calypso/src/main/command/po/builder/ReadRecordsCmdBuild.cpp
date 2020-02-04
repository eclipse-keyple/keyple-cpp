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

#include "AbstractPoCommandBuilder.h"
#include "ApduResponse.h"
#include "ReadRecordsCmdBuild.h"
#include "ReadRecordsRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;

ReadRecordsCmdBuild::ReadRecordsCmdBuild(
  PoClass poClass, char sfi, ReadDataStructure readDataStructure,
  char firstRecordNumber, bool readJustOneRecord, char expectedLength,
  const std::string& extraInfo)
: AbstractPoCommandBuilder<ReadRecordsRespPars>(CalypsoPoCommands::READ_RECORDS,
      nullptr), firstRecordNumber(firstRecordNumber),

  readDataStructure(readDataStructure)
{
    if (firstRecordNumber < 1) {
        throw std::invalid_argument("Bad record number (< 1)");
    }

    uint8_t p2 = sfi == 0x00 ? 0x05 : (sfi * 8) + 5;
    if (readJustOneRecord)
        p2 -= 0x01;

    this->request = setApduRequest(poClass.getValue(), command,
                                   firstRecordNumber, p2, expectedLength);
    if (extraInfo != "") {
        this->addSubName(extraInfo);
    }
}

ReadRecordsCmdBuild::ReadRecordsCmdBuild(
  PoClass poClass, char sfi, ReadDataStructure readDataStructure,
  char firstRecordNumber, bool readJustOneRecord, const std::string &extraInfo)
: ReadRecordsCmdBuild(poClass, sfi, readDataStructure, firstRecordNumber,
  readJustOneRecord, 0x00, extraInfo)
{
}

std::shared_ptr<ReadRecordsRespPars> ReadRecordsCmdBuild::createResponseParser(
    std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<ReadRecordsRespPars>(
               apduResponse, readDataStructure, firstRecordNumber);
}

}
}
}
}
}
