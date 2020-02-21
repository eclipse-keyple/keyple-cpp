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
#include <vector>
#include <stdexcept>
#include <memory>

/* Core */
#include "AbstractMatchingSe.h"
#include "AbstractSeSelectionRequest.h"
#include "PoClass.h"
#include "ReadDataStructure.h"

/* Common */
#include "exceptionhelper.h"
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"

/* Core */
#include "AbstractApduResponseParser_Import.h"
#include "SeProtocol_Import.h"

/* Calypso */
#include "PoSelector.h"
#include "ReadRecordsRespPars.h"
#include "SelectFileCmdBuild.h"
#include "SelectFileRespPars.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;
using namespace keyple::core;
using namespace keyple::core::command;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::calypso::transaction;

/**
 * Specialized selection request to manage the specific characteristics of
 * Calypso POs
 */
class EXPORT PoSelectionRequest final : public AbstractSeSelectionRequest {
public:
    /**
     * Constructor.
     *
     * @param poSelector the selector to target a particular SE
     */
    PoSelectionRequest(std::shared_ptr<PoSelector> poSelector);

    /**
     * Prepare one or more read record ApduRequest based on the target revision
     * to be executed following the selection.
     * <p>
     * The expected length is provided and its value is checked between 1 and
     * 250.
     * <p>
     * In the case of a mixed target (rev2 or rev3) two commands are prepared.
     * The first one in rev3 format, the second one in rev2 format (mainly class
     * byte)
     *
     * @param sfi the sfi top select
     * @param readDataStructureEnum read mode enum to indicate a SINGLE,
     *        MULTIPLE or COUNTER read
     * @param firstRecordNumber the record number to read (or first record to
     *        read in case of several records)
     * @param expectedLength the expected length of the record(s)
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index indicating the order of the command in the
     *         command list
     */
    int prepareReadRecordsCmd(uint8_t sfi,
                              ReadDataStructure readDataStructureEnum,
                              uint8_t firstRecordNumber, int expectedLength,
                              const std::string &extraInfo);

    /**
     * Prepare one or more read record ApduRequest based on the target revision
     * to be executed following the selection. No expected length is specified,
     * the record output length is handled automatically.
     * <p>
     * In the case of a mixed target (rev2 or rev3) two commands are prepared.
     * The first one in rev3 format, the second one in rev2 format (mainly class
     * byte)
     *
     * @param sfi the sfi top select
     * @param readDataStructureEnum read mode enum to indicate a SINGLE,
     *        MULTIPLE or COUNTER read
     * @param firstRecordNumber the record number to read (or first record to
     *        read in case of several records)
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index indicating the order of the command in the
     *         command list
     */
    int prepareReadRecordsCmd(uint8_t sfi,
                              ReadDataStructure readDataStructureEnum,
                              uint8_t firstRecordNumber,
                              const std::string& extraInfo);

    /**
     * Prepare a select file ApduRequest to be executed following the selection.
     * <p>
     *
     * @param path path from the CURRENT_DF (CURRENT_DF identifier excluded)
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index indicating the order of the command in the
     *         command list
     */
    int prepareSelectFileCmd(const std::vector<uint8_t>& path,
                             const std::string& extraInfo);

    /**
     * Prepare a select file ApduRequest to be executed following the selection.
     * <p>
     *
     * @param selectControl provides the navigation case: FIRST, NEXT or CURRENT
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index indicating the order of the command in the
     *         command list
     */
    int prepareSelectFileCmd(SelectFileCmdBuild::SelectControl selectControl,
                             const std::string& extraInfo);

    /**
     * Prepare a custom read ApduRequest to be executed following the selection.
     *
     * @param name the name of the command (will appear in the ApduRequest log)
     * @param apdu the byte array corresponding to the command to be sent (the
     *        correct instruction byte must be provided)
     * @return the command index indicating the order of the command in the
     *         command list
     */
    int preparePoCustomReadCmd(const std::string& name,
                               const std::vector<uint8_t>& apdu);

    /**
     * Prepare a custom modification ApduRequest to be executed following the
     * selection.
     *
     * @param name the name of the command (will appear in the ApduRequest log)
     * @param apduRequest the ApduRequest (the correct instruction byte must be
     *        provided)
     * @return the command index indicating the order of the command in the
     *         command list
     */
    int preparePoCustomModificationCmd(const std::string& name,
                                       std::shared_ptr<ApduRequest> apduRequest);

    /**
     * Return the parser corresponding to the command whose index is provided.
     *
     * @param seResponse the received SeResponse containing the commands raw
     *        responses
     * @param commandIndex the command index
     * @return a parser of the type matching the command
     */
    std::shared_ptr<AbstractApduResponseParser> getCommandParser(
        std::shared_ptr<SeResponse> seResponse, int commandIndex) override;

protected:
    /**
     * Create a CalypsoPo object containing the selection data received from the
     * plugin
     *
     * @param seResponse the SE response received
     * @return a {@link CalypsoPo}
     */
    //std::shared_ptr<CalypsoPo> parse(std::shared_ptr<SeResponse> seResponse)
    // override;
    std::shared_ptr<AbstractMatchingSe> parse(
        std::shared_ptr<SeResponse> seResponse) override;

    /**
     *
     */
    std::shared_ptr<PoSelectionRequest> shared_from_this()
    {
        return std::static_pointer_cast<PoSelectionRequest>(
                   AbstractSeSelectionRequest::shared_from_this());
    }

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(PoSelectionRequest));

    /**
     *
     */
    int commandIndex = 0;

    /**
     *
     */
    std::vector<std::string> parsingClassList = std::vector<std::string>();

    /**
     *
     */
    std::unordered_map<int, uint8_t> readRecordFirstRecordNumberMap =
         std::unordered_map<int, uint8_t>();

    /**
     *
     */
    std::unordered_map<int, ReadDataStructure> readRecordDataStructureMap =
        std::unordered_map<int, ReadDataStructure>();

    /**
     *
     */
    PoClass poClass;

    /**
     * Prepare one or more read record ApduRequest based on the target revision
     * to be executed following the selection.
     * <p>
     * In the case of a mixed target (rev2 or rev3) two commands are prepared.
     * The first one in rev3 format, the second one in rev2 format (mainly class
     * byte)
     *
     * @param sfi the sfi top select
     * @param readDataStructureEnum read mode enum to indicate a SINGLE,
     *        MULTIPLE or COUNTER read
     * @param firstRecordNumber the record number to read (or first record to
     *        read in case of several records)
     * @param expectedLength the expected length of the record(s)
     * @param extraInfo extra information included in the logs (can be null or
     *        empty)
     * @return the command index indicating the order of the command in the
     *         command list
     */
    int prepareReadRecordsCmdInternal(uint8_t sfi,
                                     ReadDataStructure readDataStructureEnum,
                                     uint8_t firstRecordNumber,
                                     int expectedLength,
                                     const std::string& extraInfo);
};

}
}
}
