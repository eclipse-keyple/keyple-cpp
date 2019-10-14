/* Calypso */
#include "CardGenerateKeyRespPars.h"

/* Core */
#include "ApduResponse.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {
namespace parser {
namespace security {

using AbstractSamResponseParser = org::eclipse::keyple::calypso::command::sam::AbstractSamResponseParser;
using ApduResponse              = org::eclipse::keyple::core::seproxy::message::ApduResponse;
using StatusProperties          = org::eclipse::keyple::core::command::AbstractApduResponseParser::StatusProperties;

std::unordered_map<int, std::shared_ptr<StatusProperties>> CardGenerateKeyRespPars::STATUS_TABLE;

CardGenerateKeyRespPars::StaticConstructor::StaticConstructor()
{
    std::unordered_map<int, std::shared_ptr<StatusProperties>> m(AbstractSamResponseParser::STATUS_TABLE);
    m.emplace(0x6700, std::make_shared<StatusProperties>(false, "Lc value not supported"));
    m.emplace(0x6985, std::make_shared<StatusProperties>(false, "Preconditions not satisfied"));
    m.emplace(0x6A00, std::make_shared<StatusProperties>(false, "Incorrect P1 or P2"));
    m.emplace(0x6A80, std::make_shared<StatusProperties>(false, "Incorrect incoming data: unknown or incorrect format"));
    m.emplace(0x6A83, std::make_shared<StatusProperties>(false, "Record not found: ciphering key or key to cipher not found"));
    m.emplace(0x9000, std::make_shared<StatusProperties>(true, "Successful execution."));
    STATUS_TABLE = m;
}

CardGenerateKeyRespPars::StaticConstructor CardGenerateKeyRespPars::staticConstructor;

std::unordered_map<int, std::shared_ptr<StatusProperties>> CardGenerateKeyRespPars::getStatusTable()
{
    return STATUS_TABLE;
}

CardGenerateKeyRespPars::CardGenerateKeyRespPars(std::shared_ptr<ApduResponse> response) : org::eclipse::keyple::calypso::command::sam::AbstractSamResponseParser(response) {
}

std::vector<char> CardGenerateKeyRespPars::getCipheredData() {
    return isSuccessful() ? response->getDataOut() : std::vector<char>();
}

}
}
}
}
}
}
}
}
