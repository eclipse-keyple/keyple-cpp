#include "ReadRecordsCmdBuild.h"
#include "ReadRecordsRespPars.h"
#include "ApduResponse.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using PoClass                  = org::eclipse::keyple::calypso::command::PoClass;
using AbstractPoCommandBuilder = org::eclipse::keyple::calypso::command::po::AbstractPoCommandBuilder;
using CalypsoPoCommands        = org::eclipse::keyple::calypso::command::po::CalypsoPoCommands;
using PoSendableInSession      = org::eclipse::keyple::calypso::command::po::PoSendableInSession;
using ReadDataStructure        = org::eclipse::keyple::calypso::command::po::parser::ReadDataStructure;
using ReadRecordsRespPars      = org::eclipse::keyple::calypso::command::po::parser::ReadRecordsRespPars;
using ApduResponse             = org::eclipse::keyple::core::seproxy::message::ApduResponse;

ReadRecordsCmdBuild::ReadRecordsCmdBuild(PoClass poClass, char sfi, ReadDataStructure readDataStructure, char firstRecordNumber, bool readJustOneRecord, char expectedLength, const std::string &extraInfo)
: AbstractPoCommandBuilder<ReadRecordsRespPars>(command, nullptr) {

    if (firstRecordNumber < 1) {
        throw std::invalid_argument("Bad record number (< 1)");
    }

    char p2 = (sfi == static_cast<char>(0x00)) ? static_cast<char>(0x05) : static_cast<char>(static_cast<char>(sfi * 8) + 5);
    if (readJustOneRecord) {
        p2 = static_cast<char>(p2 - static_cast<char>(0x01));
    }
    std::vector<char> emptyVector;
    this->request = setApduRequest(poClass.getValue(), std::make_shared<CalypsoPoCommands>(command), firstRecordNumber, p2, emptyVector, expectedLength);
    if (extraInfo != "") {
        this->addSubName(extraInfo);
    }
}

ReadRecordsCmdBuild::ReadRecordsCmdBuild(PoClass poClass, char sfi, ReadDataStructure readDataStructure, char firstRecordNumber, bool readJustOneRecord, const std::string &extraInfo)
: ReadRecordsCmdBuild(poClass, sfi, readDataStructure, firstRecordNumber, readJustOneRecord, 0x00, extraInfo) {
}

std::shared_ptr<ReadRecordsRespPars> ReadRecordsCmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse) {
    return std::make_shared<ReadRecordsRespPars>(apduResponse, readDataStructure, firstRecordNumber);
}

}
}
}
}
}
}
}
