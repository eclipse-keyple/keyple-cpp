/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

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

void SelectionsResult::addMatchingSelection(
    std::shared_ptr<MatchingSelection> matchingSelection)
{
    if (!matchingSelection)
        return;

    matchingSelectionList.push_back(matchingSelection);

    /* Test if the current selection is active */
    if (matchingSelection->getMatchingSe() != nullptr &&
        matchingSelection->getMatchingSe()->isSelected()) {
        hasActiveSelection_Renamed = true;
    }
}

const std::shared_ptr<MatchingSelection> SelectionsResult::getActiveSelection()
    const
{
    std::shared_ptr<MatchingSelection> activeSelection = nullptr;

    for (auto matchingSelection : matchingSelectionList) {
        if (matchingSelection != nullptr &&
            matchingSelection->getMatchingSe()->isSelected()) {
            activeSelection = matchingSelection;
            break;
        }
    }
    return activeSelection;
}

const  std::vector<std::shared_ptr<MatchingSelection>>&
    SelectionsResult::getMatchingSelections() const
{
    return matchingSelectionList;
}

const std::shared_ptr<MatchingSelection>
    SelectionsResult::getMatchingSelection(int selectionIndex) const
{
    for (auto matchingSelection : matchingSelectionList) {
        if (matchingSelection->getSelectionIndex() == selectionIndex) {
            return matchingSelection;
        }
    }
    return nullptr;
}

bool SelectionsResult::hasActiveSelection() const
{
    return hasActiveSelection_Renamed;
}

std::ostream& operator<<(std::ostream& os, const SelectionsResult& sr)
{
    os << "SELECTIONSRESULT: {"
       << "HASACTIVESELECTION = " << sr.hasActiveSelection() << ", "
       << sr.getMatchingSelections()
       << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os,
                         const std::shared_ptr<SelectionsResult>& sr)
{
    if (sr)
        os << *sr.get();
    else
        os << "SELECTIONSRESULT = null";

    return os;
}

}
}
}
