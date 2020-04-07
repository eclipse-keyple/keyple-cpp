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
#include "ReaderEvent_Import.h"
#include "SeRequest.h"
#include "SeResponse.h"
#include "KeypleReaderException.h"
#include "KeypleChannelControlException.h"
#include "KeypleIOReaderException.h"
#include "MultiSeRequestProcessing.h"
#include "SeReader.h"
#include "Logger.h"
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
: pluginName(pluginName), name(name),
  notificationMode(ObservableReader::NotificationMode::ALWAYS)
{
    /*
     * provides an initial value for measuring the
     * inter-exchange time. The first measurement gives the
     * time elapsed since the plugin was loaded
     */
    this->before = System::nanoTime();
}

const std::string& AbstractReader::getPluginName()
{
    return pluginName;
}

const std::string& AbstractReader::getName() const
{
    return name;
}

int AbstractReader::compareTo(std::shared_ptr<SeReader> seReader)
{
    return this->getName().compare(seReader->getName());
}

std::list<std::shared_ptr<SeResponse>>
AbstractReader::transmitSet(std::set<std::shared_ptr<SeRequest>>& requestSet,
                            MultiSeRequestProcessing multiSeRequestProcessing,
                            ChannelControl channelControl)
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
    logger->debug("[%s] transmit => SEREQUESTSET = %s, elapsed %d ms\n",
                  this->getName().c_str(), "requestSet <to fix>", elapsedMs);

    try {
        responseSet = processSeRequestSet(requestSet, multiSeRequestProcessing,
                                          channelControl);
    } catch (const KeypleChannelControlException& ex) {
        timeStamp = System::nanoTime();
        elapsedMs =
            static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;
        logger->debug("transmit => SEREQUESTSET channel failure. elapsed %d\n",
                      elapsedMs);
        /* Throw an exception with the responses collected so far. */
        logger->debug("exception message: %s\n", ex.getMessage().c_str());
        throw ex;
    } catch (const KeypleIOReaderException& ex) {
        timeStamp = System::nanoTime();
        elapsedMs =
            static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;
        logger->debug("transmit => SEREQUESTSET IO failure. elapsed %d\n",
                      elapsedMs);

        /* Throw an exception with the responses collected so far. */
        logger->debug("exception message: %s\n", ex.getMessage().c_str());
        throw ex;
    }

    timeStamp = System::nanoTime();
    elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
    this->before = timeStamp;
    logger->debug("[%s] transmit => SERESPONSESET = %s, elapsed %d ms\n",
                  this->getName().c_str(), "responseSet <to fix>", elapsedMs);

    return responseSet;
}

std::list<std::shared_ptr<SeResponse>>
AbstractReader::transmitSet(std::set<std::shared_ptr<SeRequest>>& requestSet)
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

    logger->debug("[%s] transmit => SEREQUEST = %s, elapsed %d ms\n",
                  this->getName().c_str(), seRequest->toString().c_str(),
                  elapsedMs);

    try {
        seResponse = processSeRequest(seRequest, channelControl);
    } catch (const KeypleChannelControlException& ex) {
        timeStamp = System::nanoTime();
        elapsedMs =
            static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;

        logger->debug("[%s] transmit => SEREQUEST channel failure. elaps. %d\n",
                      this->getName().c_str(), elapsedMs);

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

        logger->debug("[%s] transmit => SEREQUEST IO failure. elapsed %d\n",
                      this->getName().c_str(), elapsedMs);

        /*
         * Throw an exception with the responses collected so far
         * (ex.getSeResponse()).
         */
        throw ex;
    }

    timeStamp = System::nanoTime();
    elapsedMs = static_cast<double>((timeStamp - before) / 100000) / 10;
    this->before = timeStamp;

    logger->debug("[%s] transmit => SERESPONSE = %s, elapsed %d ms\n",
                  this->getName().c_str(), seResponse->toString().c_str(),
                  elapsedMs);

    return seResponse;
}

std::shared_ptr<SeResponse>
AbstractReader::transmit(std::shared_ptr<SeRequest> seRequest)
{
    return transmit(seRequest, ChannelControl::KEEP_OPEN);
}

void AbstractReader::addObserver(
    std::shared_ptr<ObservableReader::ReaderObserver> observer)
{
    logger->trace("[%s] addObserver => Adding '%s' as an observer of '%s'\n",
                  typeid(this).name(), typeid(observer).name(),
                  name.c_str());
    Observable<ReaderEvent>::addObserver(observer);
}

void AbstractReader::removeObserver(
    std::shared_ptr<ObservableReader::ReaderObserver> observer)
{
    logger->trace("[%s] removeObserver => Deleting a reader observer\n",
                  this->getName().c_str());
    Observable<ReaderEvent>::removeObserver(observer);
}

void AbstractReader::notifySeProcessed()
{
    if (forceClosing) {
        try {
            /* Close the physical channel thanks to CLOSE_AFTER flag */
            processSeRequest(nullptr, ChannelControl::CLOSE_AFTER);
            logger->trace("Explicit communication closing requested, starting"
                          " removal sequence\n");
        } catch (KeypleReaderException& e) {
            logger->error("KeypleReaderException while terminating: %s\n",
                          e.getMessage().c_str());
        }
    } else {
        logger->trace("Explicit physical channel closing already requested\n");
    }
}

void AbstractReader::notifyObservers(std::shared_ptr<ReaderEvent> event)
{
    logger->trace("[%s] AbstractReader => Notifying a reader event to %d "
                  "observers. EVENTNAME = %s\n",
                  this->getName().c_str(), this->countObservers(),
                  event->getEventType().getName().c_str());

    setChanged();

    Observable<ReaderEvent>::notifyObservers(event);
}

void AbstractReader::setParameters(
    const std::map<std::string, std::string>& parameters)
{
    for (auto& en : parameters) {
        setParameter(en.first, en.second);
    }
}

}
}
}
}
