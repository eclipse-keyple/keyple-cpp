#include "DigestCloseRespPars.h"
#include "ApduResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace parser {
                            namespace security {
                                using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                                using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;

                                DigestCloseRespPars::DigestCloseRespPars(std::shared_ptr<ApduResponse> response) : AbstractApduResponseParser(response) {
                                }

                                std::vector<char> DigestCloseRespPars::getSignature() {
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
