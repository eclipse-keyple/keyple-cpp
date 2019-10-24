/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#include <iostream>

#include "AbstractMatchingSe.h"
#include "SelectionsResult.h"
#include "MatchingSelection.h"

namespace keyple {
namespace core {
namespace selection {

using namespace keyple::core::selection;

SelectionsResult::SelectionsResult()
{
}

void SelectionsResult::addMatchingSelection(std::shared_ptr<MatchingSelection> matchingSelection)
{
    matchingSelectionList.push_back(matchingSelection);
    /* test if the current selection is active */
    if (matchingSelection->getMatchingSe()->isSelected()) {
        hasActiveSelection_Renamed = true;
    }
}

std::shared_ptr<MatchingSelection> SelectionsResult::getActiveSelection()
{
    std::shared_ptr<MatchingSelection> activeSelection = nullptr;

    for (auto matchingSelection : matchingSelectionList) {
        if (matchingSelection != nullptr && matchingSelection->getMatchingSe()->isSelected()) {
            activeSelection = matchingSelection;
            break;
        }
    }
    return activeSelection;
}

std::vector<std::shared_ptr<MatchingSelection>> SelectionsResult::getMatchingSelections()
{
    return matchingSelectionList;
}

std::shared_ptr<MatchingSelection> SelectionsResult::getMatchingSelection(int selectionIndex)
{
    for (auto matchingSelection : matchingSelectionList) {
        if (matchingSelection->getSelectionIndex() == selectionIndex) {
            return matchingSelection;
        }
    }
    return nullptr;
}

bool SelectionsResult::hasActiveSelection()
{
    return hasActiveSelection_Renamed;
}

}
}
}
