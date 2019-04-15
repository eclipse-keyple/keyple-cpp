#include "SamGetChallengeRespPars.h"
#include "ApduResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace parser {
                            namespace session {
                                using AbstractApduResponseParser = org::eclipse::keyple::command::AbstractApduResponseParser;
                                using ApduResponse = org::eclipse::keyple::seproxy::message::ApduResponse;

                                SamGetChallengeRespPars::SamGetChallengeRespPars(std::shared_ptr<ApduResponse> response) : AbstractApduResponseParser(response) {
                                }

                                std::vector<char> SamGetChallengeRespPars::getChallenge() {
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
