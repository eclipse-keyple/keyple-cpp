/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "AbstractDefaultSelectionsRequest.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

AbstractDefaultSelectionsRequest::AbstractDefaultSelectionsRequest(
  const std::vector<std::shared_ptr<SeRequest>>& selectionSeRequests,
  const MultiSeRequestProcessing& multiSeRequestProcessing,
  const ChannelControl& channelControl)
: mSelectionSeRequests(selectionSeRequests),
  mMultiSeRequestProcessing(multiSeRequestProcessing),
  mChannelControl(channelControl) {}

const std::vector<std::shared_ptr<SeRequest>>&
    AbstractDefaultSelectionsRequest::getSelectionSeRequests() const
{
    return mSelectionSeRequests;
}

const MultiSeRequestProcessing&
    AbstractDefaultSelectionsRequest::getMultiSeRequestProcessing() const
{
    return mMultiSeRequestProcessing;
}

const ChannelControl& AbstractDefaultSelectionsRequest::getChannelControl()
    const
{
    return mChannelControl;
}

}
}
}
}
