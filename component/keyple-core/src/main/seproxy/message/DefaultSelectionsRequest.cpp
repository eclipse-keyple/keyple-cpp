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
#include "ObservableReader_Import.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::core::seproxy::event;

DefaultSelectionsRequest::DefaultSelectionsRequest(
    std::set<std::shared_ptr<SeRequest>>& selectionSeRequestSet,
    MultiSeRequestProcessing multiSeRequestProcessing,
    ChannelControl channelControl)
: selectionSeRequestSet(selectionSeRequestSet),
  multiSeRequestProcessing(multiSeRequestProcessing),
  channelControl(channelControl)
{
}

DefaultSelectionsRequest::DefaultSelectionsRequest(
    std::set<std::shared_ptr<SeRequest>>& selectionSeRequestSet)
: DefaultSelectionsRequest(selectionSeRequestSet,
                           MultiSeRequestProcessing::FIRST_MATCH,
                           ChannelControl::KEEP_OPEN)
{
}

MultiSeRequestProcessing&
DefaultSelectionsRequest::getMultiSeRequestProcessing()
{
    return multiSeRequestProcessing;
}

ChannelControl& DefaultSelectionsRequest::getChannelControl()
{
    return channelControl;
}

std::set<std::shared_ptr<SeRequest>>&
DefaultSelectionsRequest::getSelectionSeRequestSet()
{
    return selectionSeRequestSet;
}

}
}
}
}
