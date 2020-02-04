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
std::unordered_map<int, std::shared_ptr<StatusProperties>>
    ReadRecordsRespPars::STATUS_TABLE;

ReadRecordsRespPars::StaticConstructor::StaticConstructor()
{
    std::unordered_map<int, std::shared_ptr<StatusProperties>>
        m(AbstractApduResponseParser::STATUS_TABLE);

    m.emplace(0x6981,
              std::make_shared<StatusProperties>(
                  false, "Command forbidden on binary files"));
    m.emplace(0x6982,
              std::make_shared<StatusProperties>(
                  false,
                  "Security conditions not fulfilled (PIN code not presented," \
                  " encryption required)."));
    m.emplace(0x6985,
              std::make_shared<StatusProperties>(
                  false, "Access forbidden (Never access mode, stored value " \
                  "log file and a stored value operation was done during the " \
                  "current session)."));
    m.emplace(0x6986,
              std::make_shared<StatusProperties>(
                  false, "Command not allowed (no current EF)"));
    m.emplace(0x6A82,
              std::make_shared<StatusProperties>(false, "File not found"));
    m.emplace(0x6A83,
              std::make_shared<StatusProperties>(
                  false,
                  "Record not found (record index is 0, or above NumRec"));
    m.emplace(0x6B00,
              std::make_shared<StatusProperties>(
                  false, "P2 value not supported"));
    m.emplace(0x6CFF,
              std::make_shared<StatusProperties>(false, "Le value incorrect"));
    m.emplace(0x9000,
              std::make_shared<StatusProperties>(
                  true, "Successful execution."));

    STATUS_TABLE = m;
}

ReadRecordsRespPars::StaticConstructor ReadRecordsRespPars::staticConstructor;

std::unordered_map<int, std::shared_ptr<
    AbstractApduResponseParser::StatusProperties>>
         ReadRecordsRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

ReadRecordsRespPars::ReadRecordsRespPars(
  std::shared_ptr<ApduResponse> apduResponse,
  ReadDataStructure readDataStructure, uint8_t recordNumber)
: AbstractPoResponseParser(apduResponse), readDataStructure(readDataStructure),
  recordNumber(recordNumber)
{
}

bool ReadRecordsRespPars::isCounterFile()
{
    return readDataStructure == ReadDataStructure::SINGLE_COUNTER ||
           readDataStructure == ReadDataStructure::MULTIPLE_COUNTER;
}

std::shared_ptr<std::map<int, std::vector<uint8_t>>>
    ReadRecordsRespPars::getRecords()
{
    if (!isInitialized()) {
        throw IllegalStateException("Parser not initialized.");
    }
    std::shared_ptr<std::map<int, std::vector<uint8_t>>> records =
        std::make_shared<std::map<int, std::vector<uint8_t>>>();

    if (!response->isSuccessful()) {
        /* return an empty map */
        // TODO should we raise an exception?
        return records;
    }
    if (readDataStructure == ReadDataStructure::SINGLE_RECORD_DATA) {
        records->emplace(static_cast<int>(recordNumber),
                         response->getDataOut());
    }
    else if (readDataStructure == ReadDataStructure::MULTIPLE_RECORD_DATA) {
        std::vector<uint8_t> apdu = response->getDataOut();
        int apduLen = apdu.size();
        int index = 0;
        while (apduLen > 0) {
            char recordNb = apdu[index++];
            char len = apdu[index++];
            records->insert(std::pair<int, std::vector<uint8_t>>(
                                recordNb,
                                Arrays::copyOfRange(apdu, index, index + len)));
            index = index + len;
            apduLen = apduLen - 2 - len;
        }
    }
    else {
        throw IllegalStateException("The file is a counter file.");
    }
    return records;
}

std::shared_ptr<std::map<int, int>> ReadRecordsRespPars::getCounters()
{
    if (!isInitialized()) {
        throw IllegalStateException("Parser not initialized.");
    }

    std::shared_ptr<std::map<int, int>> counters =
         std::make_shared<std::map<int, int>>();

    if (!response->isSuccessful()) {
        /* return an empty map */
        // TODO should we raise an exception?
        return counters;
    }

    if (readDataStructure == ReadDataStructure::SINGLE_COUNTER ||
        readDataStructure == ReadDataStructure::MULTIPLE_COUNTER) {
        std::vector<uint8_t> apdu = response->getDataOut();
        int numberOfCounters = apdu.size() / 3;
        int index = 0;
        int key = 1; // the first counter is indexed 1
        for (int i = 0; i < numberOfCounters; i++) {
            /*
             * convert the 3-byte unsigned value of the counter into an
             * integer (up to 2^24 -1)
             */
            int counterValue = ((apdu[index + 0] & 0xFF) * 65536) +
                               ((apdu[index + 1] & 0xFF) * 256)   +
                                (apdu[index + 2] & 0xFF);
            counters->insert(std::pair<int, int>(key++, counterValue));
            index = index + 3;
        }

    }
    else {
        throw IllegalStateException("The file is a data file.");
    }
    return counters;
}

std::string ReadRecordsRespPars::toString() {
    std::string string;
    if (isInitialized()) {
        switch (readDataStructure) {
            case ReadDataStructure::SINGLE_RECORD_DATA: {
                std::shared_ptr<std::map<int, std::vector<uint8_t>>> recordMap =
                     getRecords();
                string = StringHelper::formatSimple(
                            "Single record data: {RECORD = %d, DATA = %s}",
                            recordMap->begin()->first,
                            ByteArrayUtil::toHex(recordMap->begin()->second));
            }
                break;
            case ReadDataStructure::MULTIPLE_RECORD_DATA: {
                std::shared_ptr<std::map<int, std::vector<uint8_t>>> recordMap =
                     getRecords();
                std::shared_ptr<StringBuilder> sb =
                    std::make_shared<StringBuilder>();
                sb->append("Multiple record data: ");
                std::set<int> records;
                std::transform(recordMap->begin(), recordMap->end(),
                               std::inserter(records, records.begin()),
                               std::bind(&std::map<int, std::vector<uint8_t>>
                                   ::value_type::first, std::placeholders::_1));
                for (std::set<int>::const_iterator it = records.begin();
                     it != records.end(); ++it) {
                    int record = (int) *it;
                    sb->append(StringHelper::formatSimple(
                                   "{RECORD = %d, DATA = %s}", record,
                                   ByteArrayUtil::toHex(recordMap->find(record)
                                       ->second)));
                    if (it != records.end()) {
                        sb->append(", ");
                    }
                }
                string = sb->toString();
            }
                break;
            case ReadDataStructure::SINGLE_COUNTER: {
                std::shared_ptr<std::map<int, int>> counterMap = getCounters();
                string = StringHelper::formatSimple(
                            "Single counter: {COUNTER = %d, VALUE = %d}",
                            counterMap->begin()->first,
                            counterMap->begin()->second);
            }
                break;
            case ReadDataStructure::MULTIPLE_COUNTER: {
                std::shared_ptr<std::map<int, int>> counterMap = getCounters();
                std::shared_ptr<StringBuilder> sb =
                    std::make_shared<StringBuilder>();
                sb->append("Multiple counter: ");
                std::set<int> counters;
                std::transform(counterMap->begin(), counterMap->end(),
                               inserter(counters, counters.begin()),
                               std::bind(&std::map<int, int>::value_type::first,
                                         std::placeholders::_1));
                for (std::set<int>::const_iterator it = counters.begin();
                     it != counters.end(); ++it) {
                    int counter = (int) *it;
                    sb->append(StringHelper::formatSimple(
                                  "{COUNTER = %d, VALUE = %d}", counter,
                                  counterMap->find(counter)->second));
                    if (it != counters.end()) {
                        sb->append(", ");
                    }
                }
                string = sb->toString();
            }
                break;
            default:
                throw IllegalStateException("Unexpected data structure");
        }
    }
    else {
        string = "Not initialized.";
    }
    return string;
}

}
}
}
}
}
