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

#include "MatchingSelection.h"
#include "AbstractMatchingSe.h"
#include "AbstractSeSelectionRequest.h"
#include "SeResponse.h"
#include "SeSelector.h"
#include "AbstractApduResponseParser.h"

namespace keyple {
namespace core {
namespace selection {

using namespace keyple::core::command;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;

MatchingSelection::MatchingSelection(
    int selectionIndex,
    std::shared_ptr<AbstractSeSelectionRequest> seSelectionRequest,
    const std::shared_ptr<AbstractMatchingSe> matchingSe,
    std::shared_ptr<SeResponse> selectionSeResponse)
: matchingSe(matchingSe), seSelectionRequest(seSelectionRequest),
  selectionSeResponse(selectionSeResponse), selectionIndex(selectionIndex)
{
}

const std::shared_ptr<AbstractMatchingSe> MatchingSelection::getMatchingSe()
    const
{
    return matchingSe;
}

std::shared_ptr<AbstractApduResponseParser>
    MatchingSelection::getResponseParser(int commandIndex) const
{
    if (!seSelectionRequest)
        return nullptr;

    return seSelectionRequest->getCommandParser(selectionSeResponse,
                                                commandIndex);
}

const std::string MatchingSelection::getExtraInfo() const
{
    if (!seSelectionRequest || !seSelectionRequest->getSeSelector())
        return "";

    return seSelectionRequest->getSeSelector()->getExtraInfo();
}

int MatchingSelection::getSelectionIndex() const
{
    return selectionIndex;
}

std::ostream& operator<<(std::ostream& os, const MatchingSelection& ms)
{
    os << "MATCHINGSELECTION: {"
       << ms.matchingSe << ", "
       << ms.seSelectionRequest << ", "
       << ms.selectionSeResponse << ", "
       << "SELECTIONINDEX = " << ms.selectionIndex
       << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os,
                         const std::shared_ptr<MatchingSelection>& ms)
{
    if (ms)
        os << *ms.get();
    else
        os << "MATCHINGSELECTION = null";

    return os;
}

std::ostream& operator<<(
    std::ostream& os, const std::vector<std::shared_ptr<MatchingSelection>>& ms)
{
    os << "MATCHINGSELECTIONS: {";
    for (const auto& m : ms) {
        if (m != *ms.begin())
            os << ", ";
        os << m;
    }
    os << "}";

    return os;
}

}
}
}
