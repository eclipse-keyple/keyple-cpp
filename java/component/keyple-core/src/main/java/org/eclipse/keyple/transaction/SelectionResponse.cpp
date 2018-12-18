#include "SelectionResponse.h"
#include "../seproxy/message/SeResponseSet.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace transaction {
                using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;

                SelectionResponse::SelectionResponse(std::shared_ptr<SeResponseSet> selectionSeResponseSet) : selectionSeResponseSet(selectionSeResponseSet) {
                }

                std::shared_ptr<SeResponseSet> SelectionResponse::getSelectionSeResponseSet() {
                    return selectionSeResponseSet;
                }
            }
        }
    }
}
