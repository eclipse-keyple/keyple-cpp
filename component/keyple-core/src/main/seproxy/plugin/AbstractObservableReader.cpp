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

#include "AbstractObservableReader.h"
#include "ReaderEvent_Import.h"
#include "SeRequestSet.h"
#include "SeResponseSet.h"
#include "SeRequest.h"
#include "SeResponse.h"
#include "KeypleReaderException.h"
#include "KeypleChannelStateException.h"
#include "KeypleIOReaderException.h"
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

const std::shared_ptr<Logger> logger =
    LoggerFactory::getLogger(typeid(AbstractObservableReader));

AbstractObservableReader::AbstractObservableReader(
  const std::string &pluginName, const std::string &readerName)
: AbstractLoggedObservable<ReaderEvent>(readerName), pluginName(pluginName),
  notificationMode(NotificationMode::ALWAYS)
{
    /*
     * provides an initial value for measuring the
     * inter-exchange time. The first measurement gives the
     * time elapsed since the plugin was loaded
     */
    this->before = System::nanoTime();
}

void AbstractObservableReader::startObservation()
{
    logger->debug("starting observation\n");
}

void AbstractObservableReader::stopObservation() {
}

std::string AbstractObservableReader::getPluginName() {
    return pluginName;
    if (AbstractLoggedObservable<ReaderEvent>::countObservers() == 0) {
        logger->debug("stop the reader monitoring\n");
        stopObservation();
    }
}

int AbstractObservableReader::compareTo(std::shared_ptr<SeReader> seReader)
{
    return AbstractLoggedObservable<ReaderEvent>::getName().compare(
               seReader->getName());
}

std::shared_ptr<SeResponseSet> AbstractObservableReader::transmitSet(
    std::shared_ptr<SeRequestSet> requestSet)
{
    if (requestSet == nullptr) {
        throw std::invalid_argument("seRequestSet must not be null\n");
    }

    std::shared_ptr<SeResponseSet> responseSet;

    if (logger->isDebugEnabled()) {
        long long timeStamp = System::nanoTime();
        double elapsedMs =
            static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;
        logger->debug("[%s] transmit => SEREQUESTSET = %s, elapsed %d ms\n",
                      AbstractLoggedObservable<ReaderEvent>::getName().c_str(),
                      requestSet->toString().c_str(), elapsedMs);
    }

    try {
        responseSet = processSeRequestSet(requestSet);
    } catch (const KeypleChannelStateException &ex) {
        long long timeStamp = System::nanoTime();
        double elapsedMs =
            static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;
        logger->debug("transmit => SEREQUESTSET channel failure. elapsed %d\n",
                      elapsedMs);
        /* Throw an exception with the responses collected so far. */
        logger->debug("exception message: %s\n", ex.getMessage().c_str());
        throw ex;
    } catch (const KeypleIOReaderException &ex) {
        long long timeStamp = System::nanoTime();
        double elapsedMs =
            static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;
        logger->debug("transmit => SEREQUESTSET IO failure. elapsed %d\n",
                      elapsedMs);

        /* Throw an exception with the responses collected so far. */
        logger->debug("exception message: %s\n", ex.getMessage().c_str());
        throw ex;
    }

    if (logger->isDebugEnabled()) {
        long long timeStamp = System::nanoTime();
        double elapsedMs =
            static_cast<double>((timeStamp - before) / 100000) / 10;
        this->before = timeStamp;
        logger->debug("[%s] transmit => SERESPONSESET = %s, elapsed %d ms\n",
                        AbstractLoggedObservable<ReaderEvent>::getName().c_str(),
                        responseSet->toString().c_str(), elapsedMs);
    }

    return responseSet;
}

std::shared_ptr<SeResponse>
AbstractObservableReader::transmit(std::shared_ptr<SeRequest> seRequest)
{
    if (seRequest == nullptr) {
        throw std::invalid_argument("seRequest must not be null\n");
    }

    std::shared_ptr<SeResponse> seResponse = nullptr;

    if (logger->isDebugEnabled()) {
        long long timeStamp = System::nanoTime();
        double elapsedMs =
        static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;
 
        logger->debug("[%s] transmit => SEREQUEST = %s, elapsed %d ms\n",
                      AbstractLoggedObservable<ReaderEvent>::getName().c_str(),
                      seRequest->toString().c_str(), elapsedMs);
    }

    try {
        seResponse = processSeRequest(seRequest);
    } catch (const KeypleChannelStateException &ex) {
        long long timeStamp = System::nanoTime();
        double elapsedMs =
         static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;
 
        logger->debug("[%s] transmit => SEREQUEST channel failure. elaps. %d\n",
                      AbstractLoggedObservable<ReaderEvent>::getName().c_str(),
                      elapsedMs);
        /*
         * Throw an exception with the responses collected so far
         * (ex.getSeResponse()).
         */
        throw ex;
    }
    catch (const KeypleIOReaderException &ex) {
        long long timeStamp = System::nanoTime();
        double elapsedMs =
         static_cast<double>((timeStamp - this->before) / 100000) / 10;
        this->before = timeStamp;

        logger->debug("[%s] transmit => SEREQUEST IO failure. elapsed %d\n",
                      AbstractLoggedObservable<ReaderEvent>::getName().c_str(),
                      elapsedMs);
        /*
         * Throw an exception with the responses collected so far
         * (ex.getSeResponse()).
         */
        throw ex;
    }

    if (logger->isDebugEnabled()) {
        long long timeStamp = System::nanoTime();
        double elapsedMs =
         static_cast<double>((timeStamp - before) / 100000) / 10;
        this->before = timeStamp;
        
        logger->debug("[%s] transmit => SERESPONSE = %s, elapsed %d ms\n",
                      AbstractLoggedObservable<ReaderEvent>::getName().c_str(),
                      seResponse->toString().c_str(), elapsedMs);
    }

    return seResponse;
}

void AbstractObservableReader::addObserver(
    std::shared_ptr<ObservableReader::ReaderObserver> observer)
{
    /* If an observer is added to an empty list, start the observation */
    if (AbstractLoggedObservable<ReaderEvent>::countObservers() == 0) {
        logger->debug("Start the reader monitoring\n");
        startObservation();
    }

    AbstractLoggedObservable<ReaderEvent>::addObserver(observer);
}

void AbstractObservableReader::removeObserver(
    std::shared_ptr<ObservableReader::ReaderObserver> observer)
{
    AbstractLoggedObservable<ReaderEvent>::removeObserver(observer);
    if (AbstractLoggedObservable<ReaderEvent>::countObservers() == 0) {
        logger->debug("Stop the reader monitoring\n");
        stopObservation();
    }
}

}
}
}
}
