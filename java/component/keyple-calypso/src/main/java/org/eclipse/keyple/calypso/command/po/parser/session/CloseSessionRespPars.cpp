#include "CloseSessionRespPars.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {
                            namespace session {
                                using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                                using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;
const std::unordered_map<Integer, std::shared_ptr<StatusProperties>> CloseSessionRespPars::STATUS_TABLE;

                                CloseSessionRespPars::StaticConstructor::StaticConstructor() {
                                                                            std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> m(AbstractApduResponseParser::STATUS_TABLE);
                                                                            m.emplace(0x6700, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Lc signatureLo not supported (e.g. Lc=4 with a Revision 3.2 mode for Open Secure Session)."));
                                                                            m.emplace(0x6B00, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "P1 or P2 signatureLo not supported."));
                                                                            m.emplace(0x6988, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "incorrect signatureLo."));
                                                                            m.emplace(0x6985, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "No session was opened."));
                                                                    
                                                                            STATUS_TABLE = m;
                                }

CloseSessionRespPars::StaticConstructor CloseSessionRespPars::staticConstructor;

                                std::unordered_map<Integer, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> CloseSessionRespPars::getStatusTable() {
                                    return STATUS_TABLE;
                                }

                                CloseSessionRespPars::CloseSessionRespPars(std::shared_ptr<ApduResponse> response) : org::eclipse::keyple::command::AbstractApduResponseParser(response) {
                                    parse(response->getDataOut());
                                }

                                void CloseSessionRespPars::parse(std::vector<char> &response) {
                                    if (response.size() == 8) {
                                        signatureLo = Arrays::copyOfRange(response, 4, 8);
                                        postponedData = Arrays::copyOfRange(response, 0, 4);
                                    }
                                    else if (response.size() == 4) {
                                        signatureLo = Arrays::copyOfRange(response, 0, 4);
                                    }
                                    else {
                                        if (response.size() != 0) {
                                            throw std::invalid_argument("Unexpected length in response to CloseSecureSession command: " + std::to_string(response.size()));
                                        }
                                    }
                                }

                                std::vector<char> CloseSessionRespPars::getSignatureLo() {
                                    return signatureLo;
                                }

                                std::vector<char> CloseSessionRespPars::getPostponedData() {
                                    return postponedData;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
