#include "PoCommandBuilder.h"
#include "ApduRequest.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        
                        using AbstractIso7816CommandBuilder = org::eclipse::keyple::command::AbstractIso7816CommandBuilder;
                        using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;

                        PoCommandBuilder::PoCommandBuilder(std::shared_ptr<CalypsoPoCommands> reference, std::shared_ptr<ApduRequest> request) : AbstractIso7816CommandBuilder(reference, request) {
                        }
                    }
                }
            }
        }
    }
}
