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

#include "AbstractApduResponseParser_Import.h"
#include "AbstractMatchingSe.h"
#include "ApduRequest.h"
#include "ByteArrayUtil.h"
#include "CalypsoPo.h"
#include "PoSelectionRequest.h"
#include "PoCustomReadCommandBuilder.h"
#include "PoCustomModificationCommandBuilder.h"
#include "ReadRecordsRespPars.h"
#include "ReadRecordsCmdBuild.h"
#include "SeCommonProtocols_Import.h"
#include "SeSelector_Import.h"
#include "SeProtocol_Import.h"
#include "SelectFileRespPars.h"
#include "SelectFileCmdBuild.h"
#include "SeResponse.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::core::command;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

PoSelectionRequest::PoSelectionRequest(std::shared_ptr<PoSelector> poSelector)
: AbstractSeSelectionRequest(poSelector), poClass(PoClass::LEGACY)
{
    commandIndex = 0;

    /* No AID selector for a legacy Calypso PO */
    if (seSelector->getAidSelector() == nullptr) {
        poClass = PoClass::LEGACY;
    } else {
        poClass = PoClass::ISO;
    }

    logger->trace("Calypso %s selector\n", poClass.toString().c_str());
}

int PoSelectionRequest::prepareReadRecordsCmdInternal(
    uint8_t sfi, ReadDataStructure readDataStructureEnum,
    uint8_t firstRecordNumber, int expectedLength, const std::string& extraInfo)
{
    /*
     * the readJustOneRecord flag is set to false only in case of multiple read
     * records, in all other cases it is set to true
     */
    bool readJustOneRecord =
        !(readDataStructureEnum == ReadDataStructure::MULTIPLE_RECORD_DATA);

    logger->debug("prepareReadRecordsCmdInternal - ReadRecordsCmdBuild\n");

    std::shared_ptr<ReadRecordsCmdBuild> rrcmd =
        std::make_shared<ReadRecordsCmdBuild>(
            poClass, sfi, readDataStructureEnum, firstRecordNumber,
            readJustOneRecord, expectedLength, extraInfo);

    logger->debug("prepareReadRecordsCmdInternal - addApduRequest\n");

    addApduRequest(rrcmd->getApduRequest());

    logger->trace("ReadRecords: SFI = %02x, RECNUMBER = %d, JUSTONE = %d, "
                  "EXPECTEDLENGTH = %d\n",
                  sfi, firstRecordNumber, readJustOneRecord, expectedLength);

    /* keep read record parameters in the dedicated Maps */
    readRecordFirstRecordNumberMap.emplace(commandIndex, firstRecordNumber);
    readRecordDataStructureMap.emplace(commandIndex, readDataStructureEnum);

    /* set the parser for the response of this command */
    parsingClassList.push_back(typeid(ReadRecordsRespPars).name());

    /* return and post increment the command index */
    return commandIndex++;
}

int PoSelectionRequest::prepareReadRecordsCmd(
    uint8_t sfi, ReadDataStructure readDataStructureEnum,
    uint8_t firstRecordNumber, int expectedLength, const std::string& extraInfo)
{
    if (expectedLength < 1 || expectedLength > 250) {
        throw std::invalid_argument("Bad length.");
    }

    return prepareReadRecordsCmdInternal(sfi, readDataStructureEnum,
                                         firstRecordNumber, expectedLength,
                                         extraInfo);
}

int PoSelectionRequest::prepareReadRecordsCmd(
    uint8_t sfi, ReadDataStructure readDataStructureEnum,
    uint8_t firstRecordNumber, const std::string& extraInfo)
{
    logger->debug("prepareReadRecordsCmd\n");

    if (seSelector->getSeProtocol().ordinalValue ==
        SeCommonProtocols::PROTOCOL_ISO7816_3.ordinal()) {
        throw std::invalid_argument("In contacts mode, the expected length "
                                    "must be specified.");
    }

    logger->debug("prepareReadRecordsCmd - prepareReadRecordsCmdInternal\n");

    return prepareReadRecordsCmdInternal(sfi, readDataStructureEnum,
                                         firstRecordNumber, 0, extraInfo);
}

int PoSelectionRequest::prepareSelectFileCmd(const std::vector<uint8_t>& path,
                                             const std::string& extraInfo)
{
    (void)extraInfo;

    addApduRequest((std::make_shared<SelectFileCmdBuild>(poClass, path))
                       ->getApduRequest());

    logger->trace("Select File: PATH = %s\n",
                  ByteArrayUtil::toHex(path).c_str());

    /* set the parser for the response of this command */
    parsingClassList.push_back(typeid(SelectFileRespPars).name());

    /* return and post increment the command index */
    return commandIndex++;
}

int PoSelectionRequest::prepareSelectFileCmd(
    SelectFileCmdBuild::SelectControl selectControl,
    const std::string& extraInfo)
{
    (void)extraInfo;

    addApduRequest(
        (std::make_shared<SelectFileCmdBuild>(poClass, selectControl))
            ->getApduRequest());

    logger->trace("Navigate: CONTROL = %d\n", static_cast<int>(selectControl));

    /* set the parser for the response of this command */
    parsingClassList.push_back(typeid(SelectFileRespPars).name());

    /* return and post increment the command index */
    return commandIndex++;
}

int PoSelectionRequest::preparePoCustomReadCmd(const std::string& name,
                                               const std::vector<uint8_t>& apdu)
{
    std::shared_ptr<ApduRequest> apduRequest =
        std::make_shared<ApduRequest>(apdu, false);

    addApduRequest(
        (std::make_shared<PoCustomReadCommandBuilder>(name, apduRequest))
            ->getApduRequest());

    logger->trace("CustomReadCommand: APDUREQUEST = %s\n",
                  apduRequest->toString().c_str());

    /* return and post increment the command index */
    return commandIndex++;
}

int PoSelectionRequest::preparePoCustomModificationCmd(
    const std::string& name, std::shared_ptr<ApduRequest> apduRequest)
{
    addApduRequest((std::make_shared<PoCustomModificationCommandBuilder>(
                        name, apduRequest))
                       ->getApduRequest());

    logger->trace("CustomModificationCommand: APDUREQUEST = %s\n",
                  apduRequest->toString().c_str());

    /* return and post increment the command index */
    return commandIndex++;
}

std::shared_ptr<AbstractApduResponseParser>
PoSelectionRequest::getCommandParser(std::shared_ptr<SeResponse> seResponse,
                                     int commandIndex)
{
    if (commandIndex >= (int)parsingClassList.size()) {
        throw std::invalid_argument("Incorrect command index while getting "
                                    "command parser.");
    }

    if (seResponse->getApduResponses().size() != parsingClassList.size()) {
        throw std::invalid_argument("The number of responses and commands "
                                    "doesn't match.");
    }

    const std::string& parsingClass = parsingClassList[commandIndex];
    std::shared_ptr<AbstractApduResponseParser> parser;

    if (parsingClass == typeid(ReadRecordsRespPars).name()) {
        parser = std::make_shared<ReadRecordsRespPars>(
            seResponse->getApduResponses()[commandIndex],
            readRecordDataStructureMap[commandIndex],
            readRecordFirstRecordNumberMap[commandIndex]);
    } else if (parsingClass == typeid(SelectFileRespPars).name()) {
        parser = std::make_shared<SelectFileRespPars>(
            seResponse->getApduResponses()[commandIndex]);
    } else {
        throw std::invalid_argument("No parser available for this command.");
    }

    return parser;
}

//std::shared_ptr<CalypsoPo> PoSelectionRequest::parse(std::shared_ptr<SeResponse> seResponse) {
std::shared_ptr<AbstractMatchingSe>
PoSelectionRequest::parse(std::shared_ptr<SeResponse> seResponse)
{
    /*
     * Return an AbstractMatchingSe but *instanciate* a CalypsoPo otherwise some
     * members won't be initialized
     */
    return std::make_shared<CalypsoPo>(
               seResponse, seSelector->getSeProtocol().getTransmissionMode(),
               seSelector->getExtraInfo());
}

}
}
}
