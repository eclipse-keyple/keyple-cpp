#include "MatchingSe.h"
#include "SeResponse.h"
#include "SeSelectionRequest.h"
#include "SeRequest.h"
#include "SelectionStatus.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace transaction {

using SeResponse = org::eclipse::keyple::core::seproxy::message::SeResponse;
using SeRequest  = org::eclipse::keyple::core::seproxy::message::SeRequest;

MatchingSe::MatchingSe(std::shared_ptr<SeSelectionRequest> seSelectionRequest) : channelIsKeptOpen(seSelectionRequest->getSelectionRequest()->isKeepChannelOpen()), extraInfo(seSelectionRequest->getSeSelector()->getExtraInfo()) {
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
    return channelIsKeptOpen && selectionSeResponse != nullptr && selectionSeResponse->getSelectionStatus() != nullptr && selectionSeResponse->getSelectionStatus()->hasMatched();
}

std::string MatchingSe::getExtraInfo() {
    return extraInfo;
}

void MatchingSe::reset() {
    selectionSeResponse.reset();
}

}
}
}
}
}
