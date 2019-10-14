#include "AbstractSamResponseParser.h"
#include "ApduResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace sam {
                        using AbstractApduResponseParser = org::eclipse::keyple::core::command::AbstractApduResponseParser;
                        using ApduResponse = org::eclipse::keyple::core::seproxy::message::ApduResponse;

                        AbstractSamResponseParser::AbstractSamResponseParser(std::shared_ptr<ApduResponse> response) : org::eclipse::keyple::core::command::AbstractApduResponseParser(response) {
                        }
                    }
                }
            }
        }
    }
}
