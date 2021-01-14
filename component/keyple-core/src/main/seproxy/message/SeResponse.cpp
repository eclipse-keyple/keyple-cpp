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
  const bool logicalChannelIsOpen,
  const bool channelPreviouslyOpen,
  std::shared_ptr<SelectionStatus> selectionStatus,
  const std::vector<std::shared_ptr<ApduResponse>>& apduResponses)
: mChannelPreviouslyOpen(channelPreviouslyOpen),
  mLogicalChannelIsOpen(logicalChannelIsOpen),
  mSelectionStatus(selectionStatus),
  mApduResponses(apduResponses) {}

bool SeResponse::wasChannelPreviouslyOpen() const
{
    return mChannelPreviouslyOpen;
}

bool SeResponse::isLogicalChannelOpen() const
{
    return mLogicalChannelIsOpen;
}

const std::shared_ptr<SelectionStatus> SeResponse::getSelectionStatus() const
{
    return mSelectionStatus;
}

std::vector<std::shared_ptr<ApduResponse>>& SeResponse::getApduResponses()
{
    return mApduResponses;
}

std::ostream& operator<<(std::ostream& os, const SeResponse& sr)
{
     os << "SERESPONSE: {"
        << "RESPONSES = " << sr.mApduResponses << ", "
        << "SELECTIONSTATUS = " << sr.mSelectionStatus << ", "
        << "CHANNELWASOPEN = " << sr.mChannelPreviouslyOpen
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
                         const std::vector<std::shared_ptr<SeResponse>>& sr)
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
    return mChannelPreviouslyOpen == o.mChannelPreviouslyOpen &&
           mLogicalChannelIsOpen == o.mLogicalChannelIsOpen &&
           mApduResponses == o.mApduResponses &&
           /* Both pointers are null or equal */
           ((!mSelectionStatus && !o.mSelectionStatus) ||
            *(mSelectionStatus.get()) == *(o.mSelectionStatus.get()));
}

bool SeResponse::operator!=(const SeResponse& o) const
{
    return !(*this == o);
}

}
}
}
}
