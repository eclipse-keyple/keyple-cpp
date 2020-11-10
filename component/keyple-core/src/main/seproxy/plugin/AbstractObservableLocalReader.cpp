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

#include "AbstractObservableLocalReader.h"

/* Core */
#include "ObservableReaderStateService.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

AbstractObservableLocalReader::AbstractObservableLocalReader(const std::string& pluginName,
                                                             const std::string& readerName)
: AbstractLocalReader(pluginName, readerName) {}

bool AbstractObservableLocalReader::isSePresent()
{
    if (checkSePresence()) {
        return true;
    } else {
        /*
         * if the SE is no longer present but one of the channels is still open,
         * then the SE_REMOVED notification is performed and the channels are
         * closed.
         */
        if (isLogicalChannelOpen() || isPhysicalChannelOpen())
            processSeRemoved();

        return false;
    }
}

void AbstractObservableLocalReader::startSeDetection(
    const ObservableReader::PollingMode pollingMode)
{
    mLogger->trace("[%] start Se Detection with pollingMode %\n", getName(),
                  pollingMode);

    mCurrentPollingMode = pollingMode;

    if (mStateService)
        mStateService->onEvent(InternalEvent::START_DETECT);
}

void AbstractObservableLocalReader::stopSeDetection()
{
    mLogger->trace("[%] stop SE Detection\n", getName());

    if (mStateService)
        mStateService->onEvent(InternalEvent::STOP_DETECT);
}

void AbstractObservableLocalReader::setDefaultSelectionRequest(
    std::shared_ptr<AbstractDefaultSelectionsRequest> defaultSelectionsRequest,
    const ObservableReader::NotificationMode notificationMode)
{
    mDefaultSelectionsRequest =
        std::dynamic_pointer_cast<DefaultSelectionsRequest>(
            defaultSelectionsRequest);
    mNotificationMode = notificationMode;
}

void AbstractObservableLocalReader::setDefaultSelectionRequest(
    std::shared_ptr<AbstractDefaultSelectionsRequest> defaultSelectionsRequest,
    const ObservableReader::NotificationMode notificationMode,
    const ObservableReader::PollingMode pollingMode)
{
    /* Define the default selection request */
    setDefaultSelectionRequest(defaultSelectionsRequest, notificationMode);
    /* Initiates the SE detection */
    startSeDetection(pollingMode);
}

void AbstractObservableLocalReader::terminateSeCommunication()
{
    mLogger->trace("[%] start removal sequence of the reader\n", getName());

    if (mStateService)
        mStateService->onEvent(InternalEvent::SE_PROCESSED);
}

std::shared_ptr<ReaderEvent> AbstractObservableLocalReader::processSeInserted()
{
    mLogger->trace("[%] process the inserted se\n", getName());

    if (mDefaultSelectionsRequest == nullptr) {
        mLogger->trace("[%] no default selection request defined, notify " \
                      "SE_INSERTED\n", getName());

        /* No default request is defined, just notify the SE insertion */
        return std::make_shared<ReaderEvent>(
            getPluginName(), getName(), ReaderEvent::EventType::SE_INSERTED,
            nullptr);
    } else {
        /*
         * A default request is defined, send it and notify according to the
         * notification mode and the selection status
         */
        bool aSeMatched = false;

        try {
            std::vector<std::shared_ptr<SeResponse>> seResponses =
                transmitSeRequests(
                    mDefaultSelectionsRequest->getSelectionSeRequests(),
                    mDefaultSelectionsRequest->getMultiSeRequestProcessing(),
                    mDefaultSelectionsRequest->getChannelControl());

            for (auto seResponse : seResponses) {
                if (seResponse != nullptr && seResponse->getSelectionStatus()->hasMatched()) {
                    mLogger->trace("[%] a default selection has matched\n", getName());
                    aSeMatched = true;
                    break;
                }
            }

            if (mNotificationMode ==
                ObservableReader::NotificationMode::MATCHED_ONLY) {
                /* Notify only if a SE matched the selection, just ignore if not */
                if (aSeMatched) {
                    return std::make_shared<ReaderEvent>(
                                    getPluginName(),
                                    getName(),
                                    ReaderEvent::EventType::SE_MATCHED,
                                    std::make_shared<DefaultSelectionsResponse>(seResponses));
                } else {
                    mLogger->trace("[%] selection hasn't matched, do not throw" \
                                   " any event because of MATCHED_ONLY flag\n",
                                   getName());
                    return nullptr;
                }
            } else {
                /* ObservableReader::NotificationMode::ALWAYS */
                if (aSeMatched) {
                    /* The SE matched, notify a SE_MATCHED event with the received response */
                    return std::make_shared<ReaderEvent>(
                               getPluginName(),
                               getName(),
                               ReaderEvent::EventType::SE_MATCHED,
                               std::make_shared<DefaultSelectionsResponse>(seResponses));
                } else {
                    /*
                     * The SE didn't match, notify an SE_INSERTED event with the
                     * received response
                     */
                    mLogger->trace("[%] none of % default selection matched\n",
                                   getName(),
                        seResponses.size());

                    return std::make_shared<ReaderEvent>(
                               getPluginName(),
                               getName(),
                               ReaderEvent::EventType::SE_INSERTED,
                               std::make_shared<DefaultSelectionsResponse>(seResponses));
                }
            }
        } catch (const KeypleReaderException& e) {
            /* The last transmission failed, close the logical and physical channels */
            closeLogicalAndPhysicalChannels();
            mLogger->debug("An IO Exception occurred while processing the "
                          "default selection. %\n", e);
            /*
             * In this case the SE has been removed or not read correctly, do
             * not throw event
             */
        }
    }

    /*
     * We close here the physical channel in case it has been opened for a
     * SE outside the expected SEs
     */
    try {
        closePhysicalChannel();
    } catch (const KeypleReaderIOException& e) {
        mLogger->error("Error while closing physical channel. %\n", e);
    }

    /* No event returned */
    return nullptr;
}

bool AbstractObservableLocalReader::isSePresentPing()
{
    /* APDU sent to check the communication with the PO */
    std::vector<uint8_t> apdu = {0x00, 0xC0, 0x00, 0x00, 0x00};

    /* Transmits the APDU and checks for the IO exception */
    try {
        mLogger->trace("[%] Ping SE\n", getName());
        transmitApdu(apdu);
    } catch (const KeypleReaderIOException& e) {
        mLogger->trace("[%] Exception occurred in isSePresentPing. Message: %\n",
                      getName(), e.getMessage());
        return false;
    }

    return true;
}

void AbstractObservableLocalReader::processSeRemoved()
{
    closeLogicalAndPhysicalChannels();
    notifyObservers(std::make_shared<ReaderEvent>(
        getPluginName(), getName(), ReaderEvent::EventType::SE_REMOVED,
        nullptr));
}

const ObservableReader::PollingMode&
    AbstractObservableLocalReader::getPollingMode() const
{
    return mCurrentPollingMode;
}

void AbstractObservableLocalReader::switchState(const MonitoringState stateId)
{
    if (mStateService)
        mStateService->switchState(stateId);
}

const MonitoringState&
    AbstractObservableLocalReader::getCurrentMonitoringState() const
{
    return mStateService->getCurrentMonitoringState();
}

void AbstractObservableLocalReader::onEvent(const InternalEvent event)
{
    if (mStateService)
        mStateService->onEvent(event);
}

void AbstractObservableLocalReader::addObserver(
    const std::shared_ptr<ObservableReader::ReaderObserver> observer)
{
    if (observer == nullptr)
        return;

    mLogger->trace("Adding an observer of '%'\n", getName());

    mMutex.lock();
    mObservers.push_back(observer);
    mMutex.unlock();
}

void AbstractObservableLocalReader::removeObserver(
    const std::shared_ptr<ObservableReader::ReaderObserver>  observer)
{
    if (observer == nullptr)
        return;

    mLogger->trace("[%] Deleting a reader observer\n", getName());

    mMutex.lock();
    mObservers.remove(observer);
    mMutex.unlock();
}

void AbstractObservableLocalReader::notifyObservers(
    const std::shared_ptr<ReaderEvent> event)
{
    mLogger->trace("[%] Notifying a reader event to % observers. EVENTNAME =" \
                  " %\n", getName(), countObservers(),
                  event->getEventType().getName());

    mMutex.lock();
    std::list<std::shared_ptr<ObservableReader::ReaderObserver>>
        observersCopy(mObservers);
    mMutex.unlock();

    for (const auto& observer : observersCopy)
        observer->update(event);
}

int AbstractObservableLocalReader::countObservers() const
{
    return mObservers.size();
}

void AbstractObservableLocalReader::clearObservers()
{
   mObservers.clear();
}

void AbstractObservableLocalReader::finalizeSeProcessing()
{
    if (isPhysicalChannelOpen())
        transmitSeRequest(nullptr, ChannelControl::CLOSE_AFTER);
}

std::ostream& operator<<(std::ostream& os, const InternalEvent& ie)
{
    switch (ie) {
    case InternalEvent::SE_INSERTED:
        os << "SE_INSERTED";
        break;
    case InternalEvent::SE_REMOVED:
        os << "SE_REMOVED";
        break;
    case InternalEvent::SE_PROCESSED:
        os << "SE_PROCESSED";
        break;
    case InternalEvent::START_DETECT:
        os << "START_DETECT";
        break;
    case InternalEvent::STOP_DETECT:
        os << "STOP_DETECT";
        break;
    case InternalEvent::TIME_OUT:
        os << "TIME_OUT";
        break;
    }

    return os;
}

}
}
}
}
