/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <string>
#include <stdexcept>
#include <memory>

#include "AbstractPoCommandBuilder.h"
#include "PoSendableInSession.h"
#include "CalypsoPoCommands.h"
#include "ReadDataStructure.h"
#include "ReadRecordsRespPars.h"
#include "PoClass.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace org::eclipse::keyple::calypso::command::po;

using PoClass                  = org::eclipse::keyple::calypso::command::PoClass;
using CalypsoPoCommands        = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
using PoSendableInSession      = org::eclipse::keyple::calypso::command::po::PoSendableInSession;
using ReadDataStructure        = org::eclipse::keyple::calypso::command::po::parser::ReadDataStructure;
using ReadRecordsRespPars      = org::eclipse::keyple::calypso::command::po::parser::ReadRecordsRespPars;
using ApduResponse             = org::eclipse::keyple::core::seproxy::message::ApduResponse;

/**
 * The Class ReadRecordsCmdBuild. This class provides the dedicated constructor to build the Read
 * Records APDU command.
 */
class ReadRecordsCmdBuild final : public AbstractPoCommandBuilder<ReadRecordsRespPars>, public PoSendableInSession {
private:
    /**
     * The command
     */
    CalypsoPoCommands& command = CalypsoPoCommands::READ_RECORDS;

    /**
     *
     */
    const char firstRecordNumber;

    /**
     *
     */
    const ReadDataStructure readDataStructure;

public:
    /**
     * Instantiates a new read records cmd build.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param sfi the sfi top select
     * @param readDataStructure file structure type (used to create the parser)
     * @param firstRecordNumber the record number to read (or first record to read in case of
     *        several records)
     * @param readJustOneRecord the read just one record
     * @param expectedLength the expected length of the record(s)
     * @param extraInfo extra information included in the logs (can be null or empty)
     * @throws IllegalArgumentException - if record number &lt; 1
     * @throws IllegalArgumentException - if the request is inconsistent
     */
    ReadRecordsCmdBuild(PoClass poClass, char sfi, ReadDataStructure readDataStructure, char firstRecordNumber, bool readJustOneRecord,
                        char expectedLength, const std::string &extraInfo);

    /**
     * Instantiates a new read records cmd build without specifying the expected length. This
     * constructor is allowed only in contactless mode.
     *
     * @param poClass indicates which CLA byte should be used for the Apdu
     * @param readDataStructure file structure type
     * @param sfi the sfi top select
     * @param firstRecordNumber the record number to read (or first record to read in case of
     *        several records)
     * @param readJustOneRecord the read just one record
     * @param extraInfo extra information included in the logs (can be null or empty)
     * @throws IllegalArgumentException - if record number &lt; 1
     * @throws IllegalArgumentException - if the request is inconsistent
     */
    ReadRecordsCmdBuild(PoClass poClass, char sfi, ReadDataStructure readDataStructure, char firstRecordNumber, bool readJustOneRecord,
                        const std::string &extraInfo);
    
    /**
     *
     */
    std::shared_ptr<ReadRecordsRespPars> createResponseParser(std::shared_ptr<ApduResponse> apduResponse) override;

protected:
    /**
     *
     */
    std::shared_ptr<ReadRecordsCmdBuild> shared_from_this()
    {
        return std::static_pointer_cast<ReadRecordsCmdBuild>(AbstractPoCommandBuilder<ReadRecordsRespPars>::shared_from_this());
    }
};

}
}
}
}
}
}
}
