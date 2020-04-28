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

#include "AbstractSeSelectionRequest.h"
#include "ApduRequest.h"
#include "SeRequest.h"
#include "SeResponse.h"
#include "AbstractMatchingSe.h"

namespace keyple {
namespace core {
namespace selection {

using namespace keyple::core::command;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;

AbstractSeSelectionRequest::AbstractSeSelectionRequest(
    std::shared_ptr<SeSelector> seSelector)
: seSelector(seSelector)
{
}

AbstractSeSelectionRequest::~AbstractSeSelectionRequest()
{
}

std::shared_ptr<SeRequest> AbstractSeSelectionRequest::getSelectionRequest()
{
    return std::make_shared<SeRequest>(seSelector, seSelectionApduRequestList);
}

std::shared_ptr<SeSelector> AbstractSeSelectionRequest::getSeSelector()
{
    return seSelector;
}

void AbstractSeSelectionRequest::addApduRequest(
    std::shared_ptr<ApduRequest> apduRequest)
{
    seSelectionApduRequestList.push_back(apduRequest);
}

std::shared_ptr<AbstractApduResponseParser>
AbstractSeSelectionRequest::getCommandParser(
    std::shared_ptr<SeResponse> seResponse, int commandIndex)
{
    (void)seResponse;
    (void)commandIndex;

    /* not yet implemented in keyple-core */
    // TODO add a generic command parser
    throw std::make_shared<IllegalStateException>(
        "No parsers available for this request.");
}


std::ostream& operator<<(std::ostream& os,
	                     const AbstractSeSelectionRequest& asr)
{
    os << "ABSTRACTSELECTIONREQUEST: {"
	   << "APDUREQUESTLIST = " << asr.seSelectionApduRequestList
	   << "}";

	return os;
}

}
}
}
