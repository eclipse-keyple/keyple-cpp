#include "SamSelectionRequest.h"
#include "SeSelector.h"
#include "SeProtocol.h"
#include "CalypsoSam.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace calypso {
                namespace transaction {
                    namespace sam {
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using SeSelector = org::eclipse::keyple::seproxy::SeSelector;
                        using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                        using SeSelectionRequest = org::eclipse::keyple::transaction::SeSelectionRequest;

                        SamSelectionRequest::SamSelectionRequest(std::shared_ptr<SeSelector> seSelector, ChannelState channelState, std::shared_ptr<SeProtocol> protocolFlag) : org::eclipse::keyple::transaction::SeSelectionRequest(seSelector, channelState, protocolFlag) {

                            setMatchingClass(typeid(CalypsoSam));
                            setSelectionClass(typeid(SamSelectionRequest));
                        }
                    }
                }
            }
        }
    }
}
