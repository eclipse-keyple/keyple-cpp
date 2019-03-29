#include "DefaultSelectionRequest.h"
#include "../message/SeRequestSet.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace event_Renamed {
                    using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;

                    DefaultSelectionRequest::DefaultSelectionRequest(std::shared_ptr<SeRequestSet> selectionSeRequestSet) : selectionSeRequestSet(selectionSeRequestSet) {
                    }

                    std::shared_ptr<SeRequestSet> DefaultSelectionRequest::getSelectionSeRequestSet() {
                        return selectionSeRequestSet;
                    }
                }
            }
        }
    }
}
