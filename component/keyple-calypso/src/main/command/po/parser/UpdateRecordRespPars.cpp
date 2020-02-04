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

#include "UpdateRecordRespPars.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::core::command;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

std::unordered_map<int, std::shared_ptr<StatusProperties>> UpdateRecordRespPars::STATUS_TABLE;

UpdateRecordRespPars::StaticConstructor::StaticConstructor()
{
    std::unordered_map<int, std::shared_ptr<StatusProperties>>
        m(AbstractApduResponseParser::STATUS_TABLE);

    m.emplace(0x6400, std::make_shared<StatusProperties>(false, "Too many modifications in session"));
    m.emplace(0x6700, std::make_shared<StatusProperties>(false, "Lc value not supported"));
    m.emplace(0x6981, std::make_shared<StatusProperties>(false, "Command forbidden on cyclic files when the record exists and is not record 01h and on binary files"));
    m.emplace(0x6982, std::make_shared<StatusProperties>(false, "Security conditions not fulfilled (no session, wrong key, encryption required)"));
    m.emplace(0x6985, std::make_shared<StatusProperties>(false, "Access forbidden (Never access mode, DF is invalidated, etc..)"));
    m.emplace(0x6986, std::make_shared<StatusProperties>(false, "Command not allowed (no current EF)"));
    m.emplace(0x6A82, std::make_shared<StatusProperties>(false, "File not found"));
    m.emplace(0x6A83, std::make_shared<StatusProperties>(false, "Record is not found (record index is 0 or above NumRec)"));
    m.emplace(0x6B00, std::make_shared<StatusProperties>(false, "P2 value not supported"));
    m.emplace(0x9000, std::make_shared<StatusProperties>(true, "Successful execution"));

    STATUS_TABLE = m;
}

UpdateRecordRespPars::StaticConstructor UpdateRecordRespPars::staticConstructor;

std::unordered_map<int, std::shared_ptr<
   AbstractApduResponseParser::StatusProperties>>
      UpdateRecordRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

UpdateRecordRespPars::UpdateRecordRespPars(
  std::shared_ptr<ApduResponse> response)
: AbstractPoResponseParser(response)
{
}

}
}
}
}
}
