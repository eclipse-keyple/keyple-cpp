#include "AbstractPoCommandBuilder.h"
#include "ApduResponse.h"
#include "ReadRecordsCmdBuild.h"
#include "ReadRecordsRespPars.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace builder {

using namespace org::eclipse::keyple::calypso::command;
using namespace org::eclipse::keyple::calypso::command::po;
using namespace org::eclipse::keyple::calypso::command::po::parser;
using namespace org::eclipse::keyple::core::seproxy::message;

ReadRecordsCmdBuild::ReadRecordsCmdBuild(PoClass poClass, char sfi, ReadDataStructure readDataStructure, char firstRecordNumber,
                                         bool readJustOneRecord, char expectedLength, const std::string& extraInfo)
: AbstractPoCommandBuilder<ReadRecordsRespPars>(command, nullptr), firstRecordNumber(firstRecordNumber),

  readDataStructure(readDataStructure)
{
    if (firstRecordNumber < 1) {
        throw std::invalid_argument("Bad record number (< 1)");
    }

    char p2 = (sfi == static_cast<char>(0x00)) ? static_cast<char>(0x05) : static_cast<char>(static_cast<char>(sfi * 8) + 5);
    if (readJustOneRecord) {
        p2 = static_cast<char>(p2 - static_cast<char>(0x01));
    }

    std::vector<char> emptyVector;
    this->request = setApduRequest(poClass.getValue(), command, firstRecordNumber, p2, emptyVector,
                                   expectedLength);
    if (extraInfo != "") {
        this->addSubName(extraInfo);
    }
}

ReadRecordsCmdBuild::ReadRecordsCmdBuild(PoClass poClass, char sfi, ReadDataStructure readDataStructure, char firstRecordNumber,
                                         bool readJustOneRecord, const std::string &extraInfo)
: ReadRecordsCmdBuild(poClass, sfi, readDataStructure, firstRecordNumber, readJustOneRecord, 0x00, extraInfo)
{
    std::cout << "here we go.........." << std::endl;
}

std::shared_ptr<ReadRecordsRespPars> ReadRecordsCmdBuild::createResponseParser(std::shared_ptr<ApduResponse> apduResponse)
{
    return std::make_shared<ReadRecordsRespPars>(apduResponse, readDataStructure, firstRecordNumber);
}

}
}
}
}
}
}
}
