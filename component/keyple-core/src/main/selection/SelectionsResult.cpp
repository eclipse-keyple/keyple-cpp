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

/* Common */
#include "IllegalStateException.h"

/* Core */
#include "AbstractMatchingSe.h"
#include "SelectionsResult.h"

namespace keyple {
namespace core {
namespace selection {

SelectionsResult::SelectionsResult()
{
}

void SelectionsResult::addMatchingSe(
    int selectionIndex,
    const std::shared_ptr<AbstractMatchingSe> matchingSe, bool isSelected)
{
    if (matchingSe)
        mMatchingSeMap.insert({selectionIndex, matchingSe});

    /* If the current selection is active, we keep its index */
    if (isSelected)
        mActiveSelectionIndex = selectionIndex;
}

const std::shared_ptr<AbstractMatchingSe>
    SelectionsResult::getActiveMatchingSe()
{
    std::shared_ptr<AbstractMatchingSe> matchingSe = nullptr;

    /*
     * Do not use operator[] to check existence. This would alter the map with
     * a null element if not exiting.
     */
    if (mMatchingSeMap.find(mActiveSelectionIndex) != mMatchingSeMap.end())
        matchingSe = mMatchingSeMap[mActiveSelectionIndex];

    if (!matchingSe)
        throw IllegalStateException("No active Matching SE is available");

    return matchingSe;
}

const std::map<int, std::shared_ptr<AbstractMatchingSe>>&
    SelectionsResult::getMatchingSelections() const
{
    return mMatchingSeMap;
}

const std::shared_ptr<AbstractMatchingSe>
    SelectionsResult::getMatchingSe(int selectionIndex)
{
    /*
     * Do not use operator[] to check existence. This would alter the map with
     * a null element if not exiting.
     */
    if (mMatchingSeMap.find(selectionIndex) != mMatchingSeMap.end())
        return mMatchingSeMap[selectionIndex];
    else
        return nullptr;
}

bool SelectionsResult::hasActiveSelection() const
{
    return mActiveSelectionIndex >= 0 ? true : false;
}

bool SelectionsResult::hasSelectionMatched(int selectionIndex) const
{
    return mMatchingSeMap.count(selectionIndex) ? true : false;
}


int SelectionsResult::getActiveSelectionIndex() const
{
    if (hasActiveSelection())
        return mActiveSelectionIndex;

    throw IllegalStateException("No active Matching SE is available");
}


std::ostream& operator<<(
    std::ostream& os,
    const std::map<int, std::shared_ptr<AbstractMatchingSe>>& sr)
{
    os << "MATCHINGSELECTIONS: {";

    for (const auto& p : sr)
        os << p.first << "-" << p.second << " ";

    os << "}";

    return os;
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
