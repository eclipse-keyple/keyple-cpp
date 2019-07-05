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
#include <vector>
#include <stdexcept>
#include <memory>

/* Core */
#include "AbstractMatchingSe.h"
#include "AbstractSeSelectionRequest.h"
#include "PoClass.h"
#include "ChannelState.h"
#include "ReadDataStructure.h"

/* Common */
#include "exceptionhelper.h"
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"

/* Core */
#include "AbstractApduResponseParser_Import.h"
#include "SeProtocol.h"

/* Calypso */
#include "PoSelector.h"
#include "ReadRecordsRespPars.h"
#include "SelectFileCmdBuild.h"
#include "SelectFileRespPars.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace transaction {

using namespace org::eclipse::keyple::common;
using namespace org::eclipse::keyple::core;
using namespace org::eclipse::keyple::core::command;
using namespace org::eclipse::keyple::core::selection;
using namespace org::eclipse::keyple::core::seproxy;
using namespace org::eclipse::keyple::core::seproxy::protocol;
using namespace org::eclipse::keyple::calypso::command;
using namespace org::eclipse::keyple::calypso::command::po;
using namespace org::eclipse::keyple::calypso::command::po::builder;
using namespace org::eclipse::keyple::calypso::command::po::parser;
using namespace org::eclipse::keyple::calypso::transaction;

/**
 * Specialized selection request to manage the specific characteristics of Calypso POs
 */
class PoSelectionRequest final : public AbstractSeSelectionRequest {
  private:
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(PoSelectionRequest));

    int commandIndex = 0;
    std::vector<std::type_info> parsingClassList = std::vector<std::type_info>();
    std::unordered_map<int, char> readRecordFirstRecordNumberMap = std::unordered_map<int, char>();
    std::unordered_map<int, ReadDataStructure> readRecordDataStructureMap = std::unordered_map<int, ReadDataStructure>();

    PoClass poClass;

    /**
     * Constructor.
     *
     * @param poSelector the selector to target a particular SE
     * @param channelState tell if the channel is to be closed or not after the command
     */
  public:
    PoSelectionRequest(std::shared_ptr<PoSelector> poSelector, ChannelState channelState);

    /**
     * Prepare one or more read record ApduRequest based on the target revision to be executed
     * following the selection.
     * <p>
     * In the case of a mixed target (rev2 or rev3) two commands are prepared. The first one in rev3
     * format, the second one in rev2 format (mainly class byte)
     * 
     * @param sfi the sfi top select
     * @param readDataStructureEnum read mode enum to indicate a SINGLE, MULTIPLE or COUNTER read
     * @param firstRecordNumber the record number to read (or first record to read in case of
     *        several records)
     * @param expectedLength the expected length of the record(s)
     * @param extraInfo extra information included in the logs (can be null or empty)
     * @return the command index indicating the order of the command in the command list
     */
  private:
    int prepareReadRecordsCmdInternal(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, int expectedLength, const std::string &extraInfo);

    /**
     * Prepare one or more read record ApduRequest based on the target revision to be executed
     * following the selection.
     * <p>
     * The expected length is provided and its value is checked between 1 and 250.
     * <p>
     * In the case of a mixed target (rev2 or rev3) two commands are prepared. The first one in rev3
     * format, the second one in rev2 format (mainly class byte)
     *
     * @param sfi the sfi top select
     * @param readDataStructureEnum read mode enum to indicate a SINGLE, MULTIPLE or COUNTER read
     * @param firstRecordNumber the record number to read (or first record to read in case of
     *        several records)
     * @param expectedLength the expected length of the record(s)
     * @param extraInfo extra information included in the logs (can be null or empty)
     * @return the command index indicating the order of the command in the command list
     */
  public:
    int prepareReadRecordsCmd(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, int expectedLength, const std::string &extraInfo);

    /**
     * Prepare one or more read record ApduRequest based on the target revision to be executed
     * following the selection. No expected length is specified, the record output length is handled
     * automatically.
     * <p>
     * In the case of a mixed target (rev2 or rev3) two commands are prepared. The first one in rev3
     * format, the second one in rev2 format (mainly class byte)
     *
     * @param sfi the sfi top select
     * @param readDataStructureEnum read mode enum to indicate a SINGLE, MULTIPLE or COUNTER read
     * @param firstRecordNumber the record number to read (or first record to read in case of
     *        several records)
     * @param extraInfo extra information included in the logs (can be null or empty)
     * @return the command index indicating the order of the command in the command list
     */
    int prepareReadRecordsCmd(char sfi, ReadDataStructure readDataStructureEnum, char firstRecordNumber, const std::string &extraInfo);

    /**
     * Prepare a select file ApduRequest to be executed following the selection.
     * <p>
     * 
     * @param path path from the CURRENT_DF (CURRENT_DF identifier excluded)
     * @param extraInfo extra information included in the logs (can be null or empty)
     * @return the command index indicating the order of the command in the command list
     */
    int prepareSelectFileCmd(std::vector<char> &path, const std::string &extraInfo);

    /**
     * Prepare a select file ApduRequest to be executed following the selection.
     * <p>
     * 
     * @param selectControl provides the navigation case: FIRST, NEXT or CURRENT
     * @param extraInfo extra information included in the logs (can be null or empty)
     * @return the command index indicating the order of the command in the command list
     */
    int prepareSelectFileCmd(SelectFileCmdBuild::SelectControl selectControl, const std::string &extraInfo);

    /**
     * Prepare a custom read ApduRequest to be executed following the selection.
     * 
     * @param name the name of the command (will appear in the ApduRequest log)
     * @param apdu the byte array corresponding to the command to be sent (the correct instruction
     *        byte must be provided)
     * @return the command index indicating the order of the command in the command list
     */
    int preparePoCustomReadCmd(const std::string &name, std::vector<char> &apdu);

    /**
     * Prepare a custom modification ApduRequest to be executed following the selection.
     *
     * @param name the name of the command (will appear in the ApduRequest log)
     * @param apduRequest the ApduRequest (the correct instruction byte must be provided)
     * @return the command index indicating the order of the command in the command list
     */
    int preparePoCustomModificationCmd(const std::string &name, std::shared_ptr<ApduRequest> apduRequest);

    /**
     * Return the parser corresponding to the command whose index is provided.
     *
     * @param seResponse the received SeResponse containing the commands raw responses
     * @param commandIndex the command index
     * @return a parser of the type matching the command
     */
    std::shared_ptr<AbstractApduResponseParser> getCommandParser(std::shared_ptr<SeResponse> seResponse, int commandIndex) override;

    /**
     * Create a CalypsoPo object containing the selection data received from the plugin
     * 
     * @param seResponse the SE response received
     * @return a {@link CalypsoPo}
     */
protected:
    //std::shared_ptr<CalypsoPo> parse(std::shared_ptr<SeResponse> seResponse) override;
    std::shared_ptr<AbstractMatchingSe> parse(std::shared_ptr<SeResponse> seResponse) override;
    
  protected:
    std::shared_ptr<PoSelectionRequest> shared_from_this()
{
        return std::static_pointer_cast<PoSelectionRequest>(AbstractSeSelectionRequest::shared_from_this());
    }
};

}
}
}
}
}
