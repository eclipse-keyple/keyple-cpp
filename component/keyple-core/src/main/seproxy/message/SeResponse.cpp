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

bool SeResponse::operator==(const SeResponse& o) const
{
    return this->channelPreviouslyOpen == o.channelPreviouslyOpen &&
           this->logicalChannelIsOpen == o.logicalChannelIsOpen &&
           this->apduResponses == o.apduResponses &&
           /* Both pointer are null or equal */
           ((!this->selectionStatus && !o.selectionStatus) ||
           *(this->selectionStatus.get()) == *(o.selectionStatus.get()));
}

bool SeResponse::operator!=(const SeResponse& o) const
{
    return !(*this == o);
}

}
}
}
}
