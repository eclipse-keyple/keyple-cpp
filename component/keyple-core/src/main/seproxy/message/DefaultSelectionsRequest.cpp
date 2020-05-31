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

#include "DefaultSelectionsRequest.h"
#include "ObservableReader.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::core::seproxy::event;

DefaultSelectionsRequest::DefaultSelectionsRequest(
    std::vector<std::shared_ptr<SeRequest>>& selectionSeRequestSet,
    MultiSeRequestProcessing multiSeRequestProcessing,
    ChannelControl channelControl)
: selectionSeRequestSet(selectionSeRequestSet),
  multiSeRequestProcessing(multiSeRequestProcessing),
  channelControl(channelControl)
{
}

DefaultSelectionsRequest::DefaultSelectionsRequest(
    std::vector<std::shared_ptr<SeRequest>>& selectionSeRequestSet)
: DefaultSelectionsRequest(selectionSeRequestSet,
                           MultiSeRequestProcessing::FIRST_MATCH,
                           ChannelControl::KEEP_OPEN)
{
}

const MultiSeRequestProcessing&
    DefaultSelectionsRequest::getMultiSeRequestProcessing() const
{
    return multiSeRequestProcessing;
}

const ChannelControl& DefaultSelectionsRequest::getChannelControl() const
{
    return channelControl;
}

const std::vector<std::shared_ptr<SeRequest>>&
    DefaultSelectionsRequest::getSelectionSeRequestSet() const
{
    return selectionSeRequestSet;
}

}
}
}
}
