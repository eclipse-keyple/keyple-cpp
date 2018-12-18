#include "SelectionRequest.h"
#include "../seproxy/message/SeRequestSet.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace transaction {
                using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;

                SelectionRequest::SelectionRequest(std::shared_ptr<SeRequestSet> selectionSeRequestSet) : selectionSeRequestSet(selectionSeRequestSet) {
                }

                std::shared_ptr<SeRequestSet> SelectionRequest::getSelectionSeRequestSet() {
                    return selectionSeRequestSet;
                }
            }
        }
    }
}
