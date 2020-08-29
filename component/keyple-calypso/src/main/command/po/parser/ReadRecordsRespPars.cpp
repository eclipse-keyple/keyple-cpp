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

#include <algorithm>
#include <functional>
#include <iterator>

#include "ReadRecordsRespPars.h"
#include "ByteArrayUtil.h"

/* Common */
#include "stringhelper.h"
#include "Arrays.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::calypso::command::po;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

const std::map<int, std::shared_ptr<StatusProperties>>
    ReadRecordsRespPars::STATUS_TABLE = {
    {
        0x6981,
        std::make_shared<StatusProperties>(
            "Command forbidden on binary files",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6982,
        std::make_shared<StatusProperties>(
            "Security conditions not fulfilled (PIN code not presented," \
            " encryption required).",
            typeid(CalypsoPoSecurityContextException))
    }, {
        0x6985,
        std::make_shared<StatusProperties>(
            "Access forbidden (Never access mode, stored value "
            "log file and a stored value operation was done during the "
            "current session).",
            typeid(CalypsoPoAccessForbiddenException))
    }, {
        0x6986,
        std::make_shared<StatusProperties>(
            "Command not allowed (no current EF)",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6A82,
        std::make_shared<StatusProperties>(
            "File not found", typeid(CalypsoPoDataAccessException))
    }, {
        0x6A83,
        std::make_shared<StatusProperties>(
            "Record not found (record index is 0, or above NumRec",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6B00,
        std::make_shared<StatusProperties>(
            "P2 value not supported",
            typeid(CalypsoPoIllegalParameterException))
    }
};

ReadRecordsRespPars::ReadRecordsRespPars(
  std::shared_ptr<ApduResponse> apduResponse, ReadRecordCmdBuilder* builder)
: AbstractPoResponseParser(apduResponse, builder) {}

const std::map<int, std::shared_ptr<StatusProperties>>&
    ReadRecordsRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

std::shared_ptr<std::map<int, std::vector<uint8_t>>>
    ReadRecordsRespPars::getRecords()
{
    std::shared_ptr<std::map<int, std::vector<uint8_t>>> records =
        std::make_shared<std::map<int, std::vector<uint8_t>>>();

    if (builder->getReadMode() == ReadRecordsCmdBuild::ReadMode::ONE_RECORD) {
        records.insert({builder->getFirstRecordNumber(),
                        mResponse->getDataOut()});
    } else {
        std::vector<uint8_t> apdu = mResponse->getDataOut();
        int apduLen               = apdu.size();
        int index                 = 0;
        while (apduLen > 0) {
            char recordNb = apdu[index++];
            char len      = apdu[index++];
            records->insert(std::pair<int, std::vector<uint8_t>>(
                recordNb, Arrays::copyOfRange(apdu, index, index + len)));
            index   = index + len;
            apduLen = apduLen - 2 - len;
        }
    } else {
        throw IllegalStateException("The file is a counter file.");
    }
    return records;
}

}
}
}
}
}
