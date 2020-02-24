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

#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>

/* Core */
#include "AbstractApduResponseParser_Import.h"

/* Calypso */
#include "ReadDataStructure.h"
#include "AbstractPoResponseParser.h"

/* Common */
#include "exceptionhelper.h"
#include "Export.h"
#include "stringbuilder.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::core::command;
using namespace keyple::calypso::command::po;
/**
 * Read Records (00B2) response parser. See specs: Calypso / page 89 / 9.4.7
 * Read Records The {@link ReadRecordsRespPars} class holds the data resulting
 * from a Read Records command. It provides methods to retrieve these data
 * according to the file structure profile specified in the command preparation
 * step: SINGLE or MULTIPLE RECORD or COUNTER.
 */
class EXPORT ReadRecordsRespPars final : public AbstractPoResponseParser {
public:
    /**
     * Instantiates a new ReadRecordsRespPars.
     *
     * @param apduResponse the response from the PO
     * @param recordNumber the record number
     * @param readDataStructure the type of content in the response to parse
     */
    ReadRecordsRespPars(std::shared_ptr<ApduResponse> apduResponse,
                        ReadDataStructure readDataStructure,
                        uint8_t recordNumber);

    /**
     * Indicates whether the parser is associated with a counter file.
     *
     * @return true or false
     */
    bool isCounterFile();

    /**
     * Parses the Apdu response as a data record (single or multiple), retrieves
     * the records and place it in an map.
     * <p>
     * The map index follows the PO specification, i.e. starts at 1 for the
     * first record.
     * <p>
     * An empty map is returned if no data is available.
     *
     * @return a map of records
     * @exception IllegalStateException if the parser has not been initialized
     */
    std::shared_ptr<std::map<int, std::vector<uint8_t>>> getRecords();

    /**
     * Parses the Apdu response as a counter record (single or multiple),
     * retrieves the counters values and place it in an map indexed with the
     * counter number.
     * <p>
     * The map index follows the PO specification, i.e. starts at 1 for the
     * first counter.
     * <p>
     * An empty map is returned if no data is available.
     *
     * @return a map of counters
     * @exception IllegalStateException if the parser has not been initialized
     */
    std::shared_ptr<std::map<int, int>> getCounters();

    /**
     *
     */
    std::string toString();

protected:
    /**
     *
     */
    std::unordered_map<
        int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
    getStatusTable() const override;

    /**
     *
     */
    std::shared_ptr<ReadRecordsRespPars> shared_from_this()
    {
        return std::static_pointer_cast<ReadRecordsRespPars>(
            AbstractPoResponseParser::shared_from_this());
    }

private:
    /**
     *
     */
    static std::unordered_map<
        int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>>
        STATUS_TABLE;

    /**
     *
     */
    class StaticConstructor
    : public std::enable_shared_from_this<StaticConstructor> {
    public:
        StaticConstructor();
    };

    /**
     *
     */
    static ReadRecordsRespPars::StaticConstructor staticConstructor;

    /**
     * Type of data to parse: record data or counter, single or multiple
     */
    ReadDataStructure readDataStructure = static_cast<ReadDataStructure>(0);

    /**
     * Number of the first record read
     */
    char recordNumber = 0;
};

}
}
}
}
}
