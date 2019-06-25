#include "MatchingSelection.h"
#include "AbstractMatchingSe.h"
#include "AbstractSeSelectionRequest.h"
#include "SeResponse.h"
#include "SeSelector.h"
#include "AbstractApduResponseParser.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace selection {

using AbstractApduResponseParser = org::eclipse::keyple::core::command::AbstractApduResponseParser;
using SeSelector                 = org::eclipse::keyple::core::seproxy::SeSelector;
using SeResponse                 = org::eclipse::keyple::core::seproxy::message::SeResponse;

MatchingSelection::MatchingSelection(int selectionIndex, std::shared_ptr<AbstractSeSelectionRequest> seSelectionRequest, std::shared_ptr<AbstractMatchingSe> matchingSe, std::shared_ptr<SeResponse> selectionSeResponse) : matchingSe(matchingSe), seSelectionRequest(seSelectionRequest), selectionSeResponse(selectionSeResponse), selectionIndex(selectionIndex) {
}

std::shared_ptr<AbstractMatchingSe> MatchingSelection::getMatchingSe() {
    return matchingSe;
}

std::shared_ptr<AbstractApduResponseParser> MatchingSelection::getResponseParser(int commandIndex) {
    return seSelectionRequest->getCommandParser(selectionSeResponse, commandIndex);
}

std::string MatchingSelection::getExtraInfo() {
    return seSelectionRequest->getSeSelector()->getExtraInfo();
}

int MatchingSelection::getSelectionIndex() {
    return selectionIndex;
}

}
}
}
}
}
