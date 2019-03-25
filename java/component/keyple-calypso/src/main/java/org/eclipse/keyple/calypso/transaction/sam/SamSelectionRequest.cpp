#include "SamSelectionRequest.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeSelector.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocol.h"
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

                            setMatchingClass(CalypsoSam::typeid);
                            setSelectionClass(SamSelectionRequest::typeid);
                        }
                    }
                }
            }
        }
    }
}
