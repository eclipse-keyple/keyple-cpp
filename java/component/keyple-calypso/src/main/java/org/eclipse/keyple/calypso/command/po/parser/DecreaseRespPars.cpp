#include "DecreaseRespPars.h"

#include "stringhelper.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {
                            using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;

                            std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> DecreaseRespPars::STATUS_TABLE;

                            DecreaseRespPars::StaticConstructor::StaticConstructor() {
                                std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> m(AbstractApduResponseParser::STATUS_TABLE);
                                m.emplace(0x6400, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Too many modifications in session."));
                                m.emplace(0x6700, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Lc value not supported."));
                                m.emplace(0x6981, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "The current EF is not a Counters or Simulated Counter EF."));
                                m.emplace(0x6982, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Security conditions not fulfilled (no session, wrong key, encryption required)."));
                                m.emplace(0x6985, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Access forbidden (Never access mode, DF is invalidated, etc.)"));
                                m.emplace(0x6986, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Command not allowed (no current EF)."));
                                m.emplace(0x6A80, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Overflow error."));
                                m.emplace(0x6A82, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "File not found."));
                                m.emplace(0x6B00, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "P1 or P2 value not supported."));
                                m.emplace(0x6103, std::make_shared<AbstractApduResponseParser::StatusProperties>(true, "Successful execution."));
                                m.emplace(0x9000, std::make_shared<AbstractApduResponseParser::StatusProperties>(true, "Successful execution."));
                                STATUS_TABLE = m;
                            }

DecreaseRespPars::StaticConstructor DecreaseRespPars::staticConstructor;

                            std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> DecreaseRespPars::getStatusTable() {
                                return STATUS_TABLE;
                            }

                            DecreaseRespPars::DecreaseRespPars() {

                            }

                            int DecreaseRespPars::getNewValue()
                            {
                                std::vector<char> newValueBuffer = getApduResponse()->getDataOut();
                                if (newValueBuffer.size() == 3) {
                                    return (newValueBuffer[0] << 16) + (newValueBuffer[1] << 8) + newValueBuffer[2];
                                }
                                else {
                                    throw std::make_shared<IllegalStateException>("No counter value available in response to the Decrease command.");
                                }
                            }

                            std::string DecreaseRespPars::toString() {
                                return StringHelper::formatSimple("New counter value: %d", getNewValue());
                            }
                        }
                    }
                }
            }
        }
    }
}
