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

#include "AbstractReader.h"
#include "ReaderEvent.h"
#include "SeRequest.h"
#include "SeResponse.h"
#include "KeypleReaderException.h"
#include "KeypleChannelControlException.h"
#include "KeypleIOReaderException.h"
#include "MultiSeRequestProcessing.h"
#include "SeReader.h"
#include "LoggerFactory.h"
#include "System.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;

AbstractReader::AbstractReader(const std::string& pluginName,
                               const std::string& name)
: AbstractSeProxyComponent(name),
  mNotificationMode(ObservableReader::NotificationMode::ALWAYS),
  pluginName(pluginName)
{
    /*
     * provides an initial value for measuring the
     * inter-exchange time. The first measurement gives the
     * time elapsed since the plugin was loaded
     */
    this->before = System::nanoTime();
}

const std::string& AbstractReader::getPluginName() const
{
    return pluginName;
}

int AbstractReader::compareTo(std::shared_ptr<SeReader> seReader)
{
    return this->getName().compare(seReader->getName());
}

std::list<std::shared_ptr<SeResponse>> AbstractReader::transmitSet(
    const std::vector<std::shared_ptr<SeRequest>>& requestSet,
    const MultiSeRequestProcessing& multiSeRequestProcessing,
    const ChannelControl& channelControl)
{
    /*
     * Alex:
     * Parameter 'requestSet' is not supposed to be null, using a reference here
     * should force correct behaviour. Let's see if that goes well with the rest
     * of the code.
     */
    /*
    if (requestSet == nullptr) {
        throw std::invalid_argument("seRequestSet must not be null\n");
    }
    */

    std::list<std::shared_ptr<SeResponse>> responseSet;

    long long timeStamp = System::nanoTime();
    double elapsedMs =
        static_cast<double>((timeStamp - this->before) / 100000) / 10;
    this->before = timeStamp;
    logger->debug("[%] transmit => SEREQUESTSET = %, elapsed % ms\n",
                  getName(), requestSet, elapsedMs);

    try {
        responseSet = processSeRequestSet(requestSet, multiSeRequestProcessing,
                                          channelControl);
    } catch (const KeypleChannelControlException& ex) {
        timeStamp = System::nanoTime();
        elapsedMs =
            static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;
        logger->debug("transmit => SEREQUESTSET channel failure. elapsed %\n",
                      elapsedMs);
        /* Throw an exception with the responses collected so far. */
        logger->debug("exception message: %\n", ex.getMessage());
        throw ex;
    } catch (const KeypleIOReaderException& ex) {
        timeStamp = System::nanoTime();
        elapsedMs =
            static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;
        logger->debug("transmit => SEREQUESTSET IO failure. elapsed %\n",
                      elapsedMs);

        /* Throw an exception with the responses collected so far. */
        logger->debug("exception message: %\n", ex.getMessage());
        throw ex;
    }

    timeStamp = System::nanoTime();
    elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
    this->before = timeStamp;
    logger->debug("[%] transmit => SERESPONSESET = %, elapsed % ms\n",
                  getName(), responseSet, elapsedMs);

    return responseSet;
}

std::list<std::shared_ptr<SeResponse>> AbstractReader::transmitSet(
    const std::vector<std::shared_ptr<SeRequest>>& requestSet)
{
    return transmitSet(requestSet, MultiSeRequestProcessing::FIRST_MATCH,
                       ChannelControl::KEEP_OPEN);
}

std::shared_ptr<SeResponse>
AbstractReader::transmit(std::shared_ptr<SeRequest> seRequest,
                         ChannelControl channelControl)
{
    if (seRequest == nullptr) {
        throw std::invalid_argument("seRequest must not be null\n");
    }

    /* Sets the forceClosing flag */
    forceClosing = channelControl == ChannelControl::KEEP_OPEN;

    std::shared_ptr<SeResponse> seResponse = nullptr;

    long long timeStamp = System::nanoTime();
    double elapsedMs =
        static_cast<double>((timeStamp - this->before) / 100000) / 10;
    this->before = timeStamp;

    logger->trace("[%] transmit => SEREQUEST = %, elapsed % ms\n",
                  this->getName(), seRequest, elapsedMs);

    try {
        seResponse = processSeRequest(seRequest, channelControl);
    } catch (const KeypleChannelControlException& ex) {
        timeStamp = System::nanoTime();
        elapsedMs =
            static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;

        logger->debug("[%] transmit => SEREQUEST channel failure. elapsed %" \
			          "ms\n", this->getName(), elapsedMs);

        /*
         * Throw an exception with the responses collected so far
         * (ex.getSeResponse()).
         */
        throw ex;
    } catch (const KeypleIOReaderException& ex) {
        timeStamp = System::nanoTime();
        elapsedMs =
            static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;

        logger->debug("[%] transmit => SEREQUEST IO failure. elapsed % ms\n",
                      this->getName(), elapsedMs);

        /*
         * Throw an exception with the responses collected so far
         * (ex.getSeResponse()).
         */
        throw ex;
    }

    timeStamp = System::nanoTime();
    elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
    this->before = timeStamp;

    logger->trace("[%] transmit => SERESPONSE = %, elapsed % ms\n",
                  this->getName(), seResponse, elapsedMs);

    return seResponse;
}

std::shared_ptr<SeResponse>
AbstractReader::transmit(std::shared_ptr<SeRequest> seRequest)
{
    return transmit(seRequest, ChannelControl::KEEP_OPEN);
}

}
}
}
}
