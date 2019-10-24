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
#include "SeSelector_Import.h"
#include "AbstractApduResponseParser_Import.h"

namespace keyple {
namespace core {
namespace selection {

using namespace keyple::core::command;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;

MatchingSelection::MatchingSelection(
  int selectionIndex,
  std::shared_ptr<AbstractSeSelectionRequest> seSelectionRequest,
  std::shared_ptr<AbstractMatchingSe> matchingSe,
  std::shared_ptr<SeResponse> selectionSeResponse)
: matchingSe(matchingSe), seSelectionRequest(seSelectionRequest),
  selectionSeResponse(selectionSeResponse), selectionIndex(selectionIndex)
{
}

std::shared_ptr<AbstractMatchingSe> MatchingSelection::getMatchingSe()
{
    return matchingSe;
}

std::shared_ptr<AbstractApduResponseParser>
MatchingSelection::getResponseParser(int commandIndex)
{
    return seSelectionRequest->getCommandParser(selectionSeResponse,
                                                commandIndex);
}

std::string MatchingSelection::getExtraInfo()
{
    return seSelectionRequest->getSeSelector()->getExtraInfo();
}

int MatchingSelection::getSelectionIndex()
{
    return selectionIndex;
}

}
}
}
