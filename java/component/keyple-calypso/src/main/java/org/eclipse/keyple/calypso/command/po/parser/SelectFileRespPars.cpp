#include "SelectFileRespPars.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {

                            using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;

                            std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> SelectFileRespPars::STATUS_TABLE;

                            SelectFileRespPars::StaticConstructor::StaticConstructor() {
                                std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> m(AbstractApduResponseParser::STATUS_TABLE);
                                m.emplace(0x6A88, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Data object not found (optional mode not available)."));
                                m.emplace(0x6B00, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "P1 or P2 value not supported (<>004fh, 0062h, 006Fh, 00C0h, 00D0h, 0185h and 5F52h, according to availabl optional modes)."));
                                STATUS_TABLE = m;
                            }

                            SelectFileRespPars::StaticConstructor SelectFileRespPars::staticConstructor;

                            SelectFileRespPars::SelectFileRespPars() {

                            }

                            std::vector<char> SelectFileRespPars::getSelectionData() {
                                return response->getDataOut();
                            }
                        }
                    }
                }
            }
        }
    }
}
