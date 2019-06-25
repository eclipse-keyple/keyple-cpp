#include "AbstractMatchingSe.h"
#include "SelectionsResult.h"
#include "MatchingSelection.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace selection {

using AbstractMatchingSe = org::eclipse::keyple::core::selection::AbstractMatchingSe;
using MatchingSelection  = org::eclipse::keyple::core::selection::MatchingSelection;

SelectionsResult::SelectionsResult() {
}

void SelectionsResult::addMatchingSelection(std::shared_ptr<MatchingSelection> matchingSelection) {
    matchingSelectionList.push_back(matchingSelection);
    /* test if the current selection is active */
    if (matchingSelection->getMatchingSe()->isSelected()) {
        hasActiveSelection_Renamed = true;
    }
}

std::shared_ptr<MatchingSelection> SelectionsResult::getActiveSelection() {
    std::shared_ptr<MatchingSelection> activeSelection = nullptr;
    for (auto matchingSelection : matchingSelectionList) {
        if (matchingSelection != nullptr && matchingSelection->getMatchingSe()->isSelected()) {
            activeSelection = matchingSelection;
            break;
        }
    }
    return activeSelection;
}

std::vector<std::shared_ptr<MatchingSelection>> SelectionsResult::getMatchingSelections() {
    return matchingSelectionList;
}

std::shared_ptr<MatchingSelection> SelectionsResult::getMatchingSelection(int selectionIndex) {
    for (auto matchingSelection : matchingSelectionList) {
        if (matchingSelection->getSelectionIndex() == selectionIndex) {
            return matchingSelection;
        }
    }
    return nullptr;
}

bool SelectionsResult::hasActiveSelection() {
    return hasActiveSelection_Renamed;
}

}
}
}
}
}
