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

#pragma once

#include <map>
#include <memory>

/* Common */
#include "Integer.h"

/* Core */
#include "AbstractMatchingSe.h"
#include "KeypleCoreExport.h"

namespace keyple {
namespace core {
namespace selection {

/**
 * The SelectionsResult class holds the result of a selection process.
 * <p>
 * Embeds a map of keyple::core::selection::AbstractMatchingSe. At most one of these matching SE
 * is active.<br>
 * Provides a set of methods to retrieve the active selection
 * (getActiveMatchingSe) or a particular matching SE specified by its index.
 */
class KEYPLECORE_API SelectionsResult final
: public std::enable_shared_from_this<SelectionsResult> {
public:
    /**
     * Constructor
     */
    SelectionsResult();

    /**
     * Append a keyple::core::selection::AbstractMatchingSe to the internal list
     *
     * @param selectionIndex the index of the selection that resulted in the
     *                       matching SE
     * @param matchingSe the matching SE to add
     * @param isSelected true if the currently added matching SE is selected
     *                   (its logical channel is open)
     */
    void addMatchingSe(
        int selectionIndex,
        const std::shared_ptr<AbstractMatchingSe> matchingSe, bool isSelected);

    /**
     * Get the active matching SE. I.e. the SE that has been selected. <br>
     * The hasActiveSelection method should be called before.
     *
     * @return the currently active matching SE
     * @throws IllegalStateException if no active matching SE is found
     */
    const std::shared_ptr<AbstractMatchingSe> getActiveMatchingSe();

    /**
     * @return the keyple::core::selection::AbstractMatchingSe map
     */
    const std::map<int, std::shared_ptr<AbstractMatchingSe>>&
        getMatchingSelections() const;

    /**
     * Gets the keyple::core::selection::AbstractMatchingSe for the specified index.
     * <p>
     * Returns null if no keyple::core::selection::AbstractMatchingSe was found.
     *
     * @param selectionIndex the selection index
     * @return the keyple::core::selection::AbstractMatchingSe or null
     */
    const std::shared_ptr<AbstractMatchingSe> getMatchingSe(int selectionIndex);

    /**
     * @return true if an active selection is present
     */
    bool hasActiveSelection() const;

    /**
     * Get the matching status of a selection for which the index is provided.
     * <br>
     * Checks for the presence of an entry in the MatchingSe Map for the given
     * index
     *
     * @param selectionIndex the selection index
     * @return true if the selection has matched
     */
    bool hasSelectionMatched(int selectionIndex) const;

    /**
     * @return the index of the active selection
     */
    int getActiveSelectionIndex() const;

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(
        std::ostream& os, const SelectionsResult& sr);

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(
        std::ostream& os, const std::shared_ptr<SelectionsResult>& sr);

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(
        std::ostream& os,
        const std::map<int, std::shared_ptr<AbstractMatchingSe>>& sr);

private:
    /**
     *
     */
    int mActiveSelectionIndex = -1;

    /**
     *
     */
    std::map<int, std::shared_ptr<AbstractMatchingSe>> mMatchingSeMap;
};

}
}
}
