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

#include "AbstractReader.h"

/* Common */
#include "IllegalArgumentException.h"
#include "System.h"

/* Core */
#include "KeypleReaderException.h"
#include "KeypleReaderIOException.h"
#include "MultiSeRequestProcessing.h"
#include "ReaderEvent.h"
#include "SeReader.h"
#include "SeRequest.h"
#include "SeResponse.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::common::exception;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;

using NotificationMode = ObservableReader::NotificationMode;

AbstractReader::AbstractReader(const std::string& pluginName, const std::string& name)
: AbstractSeProxyComponent(name),
  mNotificationMode(NotificationMode::ALWAYS),
  mPluginName(pluginName)
{
    /*
     * provides an initial value for measuring the inter-exchange time. The first measurement gives
     * the time elapsed since the plugin was loaded
     */
    mBefore = System::nanoTime();
}

const std::string& AbstractReader::getPluginName() const
{
    return mPluginName;
}

std::vector<std::shared_ptr<SeResponse>> AbstractReader::transmitSeRequests(
    const std::vector<std::shared_ptr<SeRequest>>& seRequests,
    const MultiSeRequestProcessing& multiSeRequestProcessing,
    const ChannelControl& channelControl)
{
    if (seRequests.size() == 0 && channelControl == ChannelControl::KEEP_OPEN)
        throw IllegalArgumentException("The request list must not be null when the channel is to" \
                                       " remain open.");

    std::vector<std::shared_ptr<SeResponse>> seResponses;

    long long timeStamp = System::nanoTime();
    long long elapsed10ms = (timeStamp - mBefore) / 100000;
    mBefore = timeStamp;

    mLogger->trace("[%] transmit => SEREQUESTLIST = %, elapsed % ms\n",
                   getName(),
                   seRequests,
                   elapsed10ms / 10);

    try {
        seResponses = processSeRequests(seRequests, multiSeRequestProcessing, channelControl);
    } catch (const KeypleReaderIOException& ex) {
        timeStamp = System::nanoTime();
        elapsed10ms = (timeStamp - mBefore) / 100000;
        mBefore = timeStamp;

        mLogger->debug("[%] transmit => SEREQUESTLIST IO failure. elapsed %\n",
                       getName(),
                       elapsed10ms / 10);

        /* Throw an exception with the responses collected so far. */
        mLogger->debug("exception message: %\n", ex.getMessage());
        throw ex;
    }

    timeStamp = System::nanoTime();
    elapsed10ms = (timeStamp - mBefore) / 100000;
    mBefore = timeStamp;

    mLogger->trace("[%] transmit => SERESPONSELIST = %, elapsed % ms\n",
                   getName(),
                   seResponses,
                   elapsed10ms);

    return seResponses;
}

std::shared_ptr<SeResponse> AbstractReader::transmitSeRequest(std::shared_ptr<SeRequest> seRequest,
                                                              const ChannelControl& channelControl)
{
    if (seRequest == nullptr && channelControl == ChannelControl::KEEP_OPEN)
        throw IllegalArgumentException("The request must not be null when the channel is to " \
                                       "remain open.");

    std::shared_ptr<SeResponse> seResponse = nullptr;

    long long timeStamp = System::nanoTime();
    long long elapsed10ms = (timeStamp - mBefore) / 100000;
    mBefore = timeStamp;

    mLogger->trace("[%] transmit => SEREQUEST = %, elapsed % ms\n", getName(),
                   seRequest,
                   elapsed10ms / 10);

    try {
        seResponse = processSeRequest(seRequest, channelControl);
    } catch (const KeypleReaderIOException& ex) {
        (void)ex;
        timeStamp = System::nanoTime();
        elapsed10ms = (timeStamp - mBefore) / 100000;
        mBefore = timeStamp;

        mLogger->debug("[%] transmit => SEREQUEST IO failure. elapsed % ms\n",
                       getName(),
                       elapsed10ms / 10);

        /* Throw an exception with the responses collected so far (ex.getSeResponse()). */
        throw;
    }

    timeStamp = System::nanoTime();
    elapsed10ms = (timeStamp - mBefore) / 100000;
    mBefore = timeStamp;

    mLogger->trace("[%] transmit => SERESPONSE = %, elapsed % ms\n", getName(),
                   seResponse,
                   elapsed10ms / 10);

    return seResponse;
}

}
}
}
}
