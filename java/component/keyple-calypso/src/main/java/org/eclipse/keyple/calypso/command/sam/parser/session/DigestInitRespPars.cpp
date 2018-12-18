#include "DigestInitRespPars.h"
#include "../../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduResponse.h"

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

                                DigestInitRespPars::DigestInitRespPars(std::shared_ptr<ApduResponse> response) : org::eclipse::keyple::command::AbstractApduResponseParser(response) {
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
