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

#pragma once

#include <string>
#include <map>
#include <map>
#include <vector>
#include <memory>

/* Core */
#include "AbstractPoResponseParser.h"

/* Calypso */
#include "AbstractPoResponseParser.h"
#include "ReadRecordsCmdBuild.h"

/* Common */
#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using namespace keyple::core::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;

/**
 * Read Records (00B2) response parser. See specs: Calypso / page 89 / 9.4.7
 * Read Records The {@link ReadRecordsRespPars} class holds the data resulting
 * from a Read Records command. It provides methods to retrieve these data
 * according to the file structure profile specified in the command preparation
 * step: SINGLE or MULTIPLE RECORD or COUNTER.
 */
class KEYPLECALYPSO_API ReadRecordsRespPars final
: public AbstractPoResponseParser {
public:
    /**
     * Instantiates a new ReadRecordsRespPars.
     *
     * @param apduResponse the response from the PO
     * @param builder the reference to the builder that created this parser
     */
    ReadRecordsRespPars(std::shared_ptr<ApduResponse> apduResponse,
                        ReadRecordsCmdBuild* builder);

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
     */
    std::map<int, std::vector<uint8_t>> getRecords() const;

protected:
    /**
     *
     */
    const std::map<int, std::shared_ptr<StatusProperties>>& getStatusTable()
        const override;

private:
    /**
     *
     */
    static const std::map<int, std::shared_ptr<StatusProperties>> STATUS_TABLE;
};

}
}
}
}
}
