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
#include "SeResponse.h"
#include "SelectionStatus.h"
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "stringhelper.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::core::util;

SeResponse::SeResponse(
    bool logicalChannelIsOpen, bool channelPreviouslyOpen,
    std::shared_ptr<SelectionStatus> selectionStatus,
    std::vector<std::shared_ptr<ApduResponse>>& apduResponses)
: logicalChannelIsOpen(logicalChannelIsOpen), selectionStatus(selectionStatus)
{
    this->channelPreviouslyOpen = channelPreviouslyOpen;
    this->apduResponses         = apduResponses;
}

bool SeResponse::wasChannelPreviouslyOpen() const
{
    return channelPreviouslyOpen;
}

bool SeResponse::isLogicalChannelOpen() const
{
    return logicalChannelIsOpen;
}

const std::shared_ptr<SelectionStatus> SeResponse::getSelectionStatus() const
{
    return selectionStatus;
}

std::vector<std::shared_ptr<ApduResponse>> SeResponse::getApduResponses() const
{
    return apduResponses;
}

std::ostream& operator<<(std::ostream& os, const SeResponse& sr)
{
     os << "SERESPONSE: {"
        << "RESPONSES = " << sr.apduResponses << ", "
        << "SELECTIONSTATUS = " << sr.selectionStatus << ", "
        << "CHANNELWASOPEN = " << sr.channelPreviouslyOpen
	    << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os,
	                     const std::shared_ptr<SeResponse>& sr)
{
    if (sr)
		os << *(sr.get());
    else
		os << "SERESPONSE: null";

    return os;
}

std::ostream& operator<<(std::ostream& os,
                         const std::list<std::shared_ptr<SeResponse>>& sr)
{
	os << "SERESPONSES: { ";
	for (const auto& r : sr) {
		if (r)
			os << *(r.get());
        else
			os << "SERESPONSE: null";
		if (r != sr.back())
			os << ", ";
    }
	os << "}";

	return os;
}

bool SeResponse::equals(std::shared_ptr<void> o)
{
    if (o == shared_from_this()) {
        return true;
    }
    if (!(std::static_pointer_cast<SeResponse>(o) != nullptr)) {
        return false;
    }

    std::shared_ptr<SeResponse> seResponse =
        std::static_pointer_cast<SeResponse>(o);

    return seResponse->getSelectionStatus() == (selectionStatus) &&
           (seResponse->getApduResponses().empty()
                ? apduResponses.empty()
                : seResponse->getApduResponses() == (apduResponses)) &&
           seResponse->isLogicalChannelOpen() == logicalChannelIsOpen &&
           seResponse->wasChannelPreviouslyOpen() == channelPreviouslyOpen;
}

int SeResponse::hashCode()
{
    int hash = 17;
    hash     = 31 * hash + (selectionStatus->getAtr() == nullptr
                            ? 0
                            : selectionStatus->getAtr()->hashCode());
    hash     = 7 * hash +
           (apduResponses.empty() ? 0 : 1); //this->apduResponses.hashCode());
    hash = 29 * hash + (this->channelPreviouslyOpen ? 1 : 0);
    hash = 37 * hash + (this->logicalChannelIsOpen ? 1 : 0);
    return hash;
}

void SeResponse::finalize()
{
}

}
}
}
}
