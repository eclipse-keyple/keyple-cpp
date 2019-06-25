#include "AppendRecordRespPars.h"
#include "AbstractApduResponseParser_Import.h"
#include "ApduResponse.h"

#include "Integer.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {

using AbstractPoResponseParser = org::eclipse::keyple::calypso::command::po::AbstractPoResponseParser;
using AbstractApduResponseParser = org::eclipse::keyple::core::command::AbstractApduResponseParser;
using ApduResponse = org::eclipse::keyple::core::seproxy::message::ApduResponse;

std::unordered_map<Integer, std::shared_ptr<StatusProperties>> AppendRecordRespPars::STATUS_TABLE;

AppendRecordRespPars::StaticConstructor::StaticConstructor() {
    std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> m(AbstractApduResponseParser::STATUS_TABLE);
    m.emplace(0x6B00, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "P1 or P2 vaue not supported."));
    m.emplace(0x6700, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Lc value not supported."));
    m.emplace(0x6400, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Too many modifications in session."));
    m.emplace(0x6981, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "The current EF is not a Cyclic EF."));
    m.emplace(0x6982, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Security conditions not fulfilled (no session, wrong key)."));
    m.emplace(0x6985, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Access forbidden (Never access mode, DF is invalidated, etc..)."));
    m.emplace(0x6986, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Command not allowed (no current EF)."));
    m.emplace(0x6A82, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "File not found."));
    STATUS_TABLE = m;
}

AppendRecordRespPars::StaticConstructor AppendRecordRespPars::staticConstructor;

AppendRecordRespPars::AppendRecordRespPars(std::shared_ptr<ApduResponse> response)
: AbstractPoResponseParser(response) {
}

std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> AppendRecordRespPars::getStatusTable() {
    return STATUS_TABLE;
}

}
}
}
}
}
}
}
