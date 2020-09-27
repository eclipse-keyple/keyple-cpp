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
#include <stdexcept>
#include <memory>

#include "AbstractPoCommandBuilder.h"
#include "CalypsoPoCommand.h"
#include "KeypleCalypsoExport.h"
#include "PoClass.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace po {
    namespace parser { class ReadRecordsRespPars; } } } } }

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::seproxy::message;

/**
 * The {@link ReadRecordsCmdBuild} class provides the dedicated constructor to
 * build the Read Records APDU command.
 */
class KEYPLECALYPSO_API ReadRecordsCmdBuild final
: public AbstractPoCommandBuilder<ReadRecordsRespPars> {
public:
    /**
     *
     */
    enum class ReadMode {
        ONE_RECORD,
        MULTIPLE_RECORD
    };

    /**
     * Instantiates a new read records cmd build
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param sfi the sfi top select
     * @param firstRecordNumber the record number to read (or first record to
     *                          read in case of several records)
     * @param readMode read mode, requests the reading of one or all the records
     * @param expectedLength the expected length of the record(s)
     * @throw IllegalArgumentException - if record number &lt; 1
     * @throw IllegalArgumentException - if the request is inconsistent
     */
    ReadRecordsCmdBuild(const PoClass& poClass,
                        const uint8_t sfi,
                        const uint8_t firstRecordNumber,
                        const ReadMode readMode,
                        const int expectedLength);

    /**
     *
     */
    std::shared_ptr<ReadRecordsRespPars> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) override;

    /**
     */
    bool isSessionBufferUsed() const override;

    /**
     * @return the SFI of the accessed file
     */
    uint8_t getSfi() const;

    /**
     * @return the number of the first record to read
     */
    uint8_t getFirstRecordNumber() const;

    /**
     * @return the readJustOneRecord flag
     */
    ReadMode getReadMode() const;

    /**
     *
     */
    friend std::ostream& operator<<(std::ostream& os, const ReadMode& rm);

private:
    /**
     * The command
     */
    const CalypsoPoCommand& command = CalypsoPoCommand::READ_RECORDS;

    /**
     * Construction arguments used for parsing
     */
    const uint8_t mSfi;
    const uint8_t mFirstRecordNumber;
    const ReadMode mReadMode;
};

}
}
}
}
}
