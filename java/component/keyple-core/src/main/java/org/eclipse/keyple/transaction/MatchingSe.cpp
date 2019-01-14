#include "MatchingSe.h"
#include "../seproxy/message/SeResponse.h"
#include "SeSelector.h"
#include "SeRequest.h"
#include "SelectionStatus.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace transaction {
                using SeResponse = org::eclipse::keyple::seproxy::message::SeResponse;

                MatchingSe::MatchingSe(std::shared_ptr<SeSelector> seSelector) : channelIsKeptOpen(seSelector->getSelectorRequest()->isKeepChannelOpen()), extraInfo(seSelector->getExtraInfo()) {
                }

                void MatchingSe::setSelectionResponse(std::shared_ptr<SeResponse> selectionResponse) {
                    this->selectionSeResponse = selectionResponse;
                }

                std::shared_ptr<SeResponse> MatchingSe::getSelectionSeResponse() {
                    return selectionSeResponse;
                }

                bool MatchingSe::isSelectable() {
                    return channelIsKeptOpen;
                }

                bool MatchingSe::isSelected() {
                    return channelIsKeptOpen && selectionSeResponse != nullptr && selectionSeResponse->getSelectionStatus()->hasMatched();
                }

                std::string MatchingSe::getExtraInfo() {
                    return extraInfo;
                }
            }
        }
    }
}
