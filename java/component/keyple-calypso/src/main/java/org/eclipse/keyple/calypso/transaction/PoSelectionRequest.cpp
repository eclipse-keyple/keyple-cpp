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
#include "SeCommonProtocols.h"
#include "SeSelector.h"
#include "SeProtocol.h"
#include "SelectFileRespPars.h"
#include "SelectFileCmdBuild.h"
#include "SeResponse.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace transaction {

using namespace org::eclipse::keyple::calypso::command;
using namespace org::eclipse::keyple::calypso::command::po;
using namespace org::eclipse::keyple::calypso::command::po::builder;
using namespace org::eclipse::keyple::calypso::command::po::parser;
using namespace org::eclipse::keyple::core::command;
using namespace org::eclipse::keyple::core::seproxy;
using namespace org::eclipse::keyple::core::seproxy::message;
using namespace org::eclipse::keyple::core::seproxy::protocol;
using namespace org::eclipse::keyple::core::util;

PoSelectionRequest::PoSelectionRequest(std::shared_ptr<PoSelector> poSelector,
                                       ChannelState channelState)
: AbstractSeSelectionRequest(poSelector, channelState),
  poClass(PoClass::LEGACY) /* Default constructor to please compiler */
{
    commandIndex = 0;

    /* No AID selector for a legacy Calypso PO */
    if (seSelector->getAidSelector() == nullptr) {
        poClass = PoClass::LEGACY;
    }
    else {
        poClass = PoClass::ISO;
    }

    if (logger->isTraceEnabled()) {
        logger->trace("Calypso %s selector", poClass.toString());
    }
}

int PoSelectionRequest::prepareReadRecordsCmdInternal(char sfi,
                                                      ReadDataStructure readDataStructureEnum,
                                                      char firstRecordNumber, int expectedLength,
                                                      const std::string &extraInfo)
{
    /*
     * the readJustOneRecord flag is set to false only in case of multiple read records, in all
     * other cases it is set to true
     */
    bool readJustOneRecord = !(readDataStructureEnum == ReadDataStructure::MULTIPLE_RECORD_DATA);

    addApduRequest((std::make_shared<ReadRecordsCmdBuild>(poClass, sfi, readDataStructureEnum,
                                                         firstRecordNumber, readJustOneRecord,
                                                         static_cast<char>(expectedLength),
                                                         extraInfo))->getApduRequest());

    if (logger->isTraceEnabled()) {
        logger->trace("ReadRecords: SFI = %02x, RECNUMBER = %d, JUSTONE = %d, EXPECTEDLENGTH = %d",
                      sfi, firstRecordNumber, readJustOneRecord, expectedLength);
    }

    /* keep read record parameters in the dedicated Maps */
    readRecordFirstRecordNumberMap.emplace(commandIndex, firstRecordNumber);
    readRecordDataStructureMap.emplace(commandIndex, readDataStructureEnum);

    /* set the parser for the response of this command */
    parsingClassList.push_back(typeid(ReadRecordsRespPars).name());

    /* return and post increment the command index */
    return commandIndex++;
}

int PoSelectionRequest::prepareReadRecordsCmd(char sfi, ReadDataStructure readDataStructureEnum,
                                              char firstRecordNumber, int expectedLength,
                                              const std::string &extraInfo)
{
    if (expectedLength < 1 || expectedLength > 250) {
        throw std::invalid_argument("Bad length.");
    }

    return prepareReadRecordsCmdInternal(sfi, readDataStructureEnum, firstRecordNumber,
                                         expectedLength, extraInfo);
}

int PoSelectionRequest::prepareReadRecordsCmd(char sfi, ReadDataStructure readDataStructureEnum,
                                              char firstRecordNumber, const std::string &extraInfo)
{
    if ((std::dynamic_pointer_cast<SeCommonProtocols>(seSelector))->ordinal() ==
        SeCommonProtocols::PROTOCOL_ISO7816_3.ordinal()) {
        throw std::invalid_argument("In contacts mode, the expected length must be specified.");
    }

    return prepareReadRecordsCmdInternal(sfi, readDataStructureEnum, firstRecordNumber, 0,
                                         extraInfo);
}

int PoSelectionRequest::prepareSelectFileCmd(std::vector<char> &path, const std::string &extraInfo)
{
    (void)extraInfo;

    addApduRequest((std::make_shared<SelectFileCmdBuild>(poClass, path))->getApduRequest());
    if (logger->isTraceEnabled()) {
        logger->trace("Select File: PATH = %s", ByteArrayUtil::toHex(path));
    }

    /* set the parser for the response of this command */
    parsingClassList.push_back(typeid(SelectFileRespPars).name());

    /* return and post increment the command index */
    return commandIndex++;
}

int PoSelectionRequest::prepareSelectFileCmd(SelectFileCmdBuild::SelectControl selectControl,
                                             const std::string &extraInfo)
{
    (void)extraInfo;

    addApduRequest(
                 (std::make_shared<SelectFileCmdBuild>(poClass, selectControl))->getApduRequest());
    if (logger->isTraceEnabled()) {
        logger->trace("Navigate: CONTROL = %d", static_cast<int>(selectControl));
    }

    /* set the parser for the response of this command */
    parsingClassList.push_back(typeid(SelectFileRespPars).name());

    /* return and post increment the command index */
    return commandIndex++;
}

int PoSelectionRequest::preparePoCustomReadCmd(const std::string &name, std::vector<char> &apdu)
{
    std::shared_ptr<ApduRequest> apduRequest = std::make_shared<ApduRequest>(apdu, false);
    addApduRequest(
              (std::make_shared<PoCustomReadCommandBuilder>(name, apduRequest))->getApduRequest());
    if (logger->isTraceEnabled()) {
        logger->trace("CustomReadCommand: APDUREQUEST = %s", apduRequest);
    }
                        /* return and post increment the command index */
                        return commandIndex++;
}

int PoSelectionRequest::preparePoCustomModificationCmd(const std::string &name,
                                                       std::shared_ptr<ApduRequest> apduRequest)
{
    addApduRequest(
      (std::make_shared<PoCustomModificationCommandBuilder>(name, apduRequest))->getApduRequest());
    if (logger->isTraceEnabled()) {
        logger->trace("CustomModificationCommand: APDUREQUEST = %s", apduRequest);
    }
                        /* return and post increment the command index */
                        return commandIndex++;
}

std::shared_ptr<AbstractApduResponseParser> PoSelectionRequest::getCommandParser(std::shared_ptr<SeResponse> seResponse, int commandIndex) {
    if (commandIndex >= (int)parsingClassList.size()) {
        throw std::invalid_argument("Incorrect command index while getting command parser.");
    }
    if (seResponse->getApduResponses().size() != parsingClassList.size()) {
        throw std::invalid_argument("The number of responses and commands doesn't match.");
    }
    const std::string& parsingClass = parsingClassList[commandIndex];
    std::shared_ptr<AbstractApduResponseParser> parser;
    if (parsingClass == typeid(ReadRecordsRespPars).name()) {
       parser = std::make_shared<ReadRecordsRespPars>(seResponse->getApduResponses()[commandIndex],
                                                      readRecordDataStructureMap[commandIndex],
                                                      readRecordFirstRecordNumberMap[commandIndex]);
    } else if (parsingClass == typeid(SelectFileRespPars).name()) {
       parser = std::make_shared<SelectFileRespPars>(seResponse->getApduResponses()[commandIndex]);
    } else {
        throw std::invalid_argument("No parser available for this command.");
    }

    return parser;
}

//std::shared_ptr<CalypsoPo> PoSelectionRequest::parse(std::shared_ptr<SeResponse> seResponse) {
std::shared_ptr<AbstractMatchingSe> PoSelectionRequest::parse(std::shared_ptr<SeResponse> seResponse)
{
    return std::make_shared<AbstractMatchingSe>(seResponse,
                                                seSelector->getSeProtocol().getTransmissionMode(),
                                                seSelector->getExtraInfo());
}

}
}
}
}
}

