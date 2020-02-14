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

#include "AbstractObservableLocalReader.h"

/* Core */
#include "ObservableReaderStateService.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {

AbstractObservableLocalReader::AbstractObservableLocalReader(
  const std::string& pluginName, const std::string& readerName)
: AbstractLocalReader(pluginName, readerName)
{
}

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
        if (isLogicalChannelOpen() || isPhysicalChannelOpen()) {
            processSeRemoved();
        }
        return false;
    }
}

void AbstractObservableLocalReader::startSeDetection(
    const ObservableReader::PollingMode pollingMode)
{
    logger->trace("[%s] startSeDetection => start Se Detection with " \
                  "pollingMode %d\n", this->getName(), pollingMode);
    this->currentPollingMode = pollingMode;
    this->stateService->onEvent(InternalEvent::START_DETECT);
}

void AbstractObservableLocalReader::stopSeDetection()
{
    logger->trace("[%s] stopSeDetection => stop Se Detection\n",
                  this->getName());

    this->stateService->onEvent(InternalEvent::STOP_DETECT);
}

void AbstractObservableLocalReader::setDefaultSelectionRequest(
    std::shared_ptr<AbstractDefaultSelectionsRequest> defaultSelectionsRequest,
    const ObservableReader::NotificationMode notificationMode)
{
    this->defaultSelectionsRequest =
        std::dynamic_pointer_cast<DefaultSelectionsRequest>(
            defaultSelectionsRequest);
    this->notificationMode = notificationMode;
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

void AbstractObservableLocalReader::startRemovalSequence()
{
    logger->trace("[%s] startRemovalSequence => start removal sequence of " \
                  "the reader\n", this->getName());
    this->stateService->onEvent(InternalEvent::SE_PROCESSED);
}

bool AbstractObservableLocalReader::processSeInserted()
{
    logger->trace("[%s] processSeInserted => process the inserted se\n",
                  this->getName());

    bool presenceNotified = false;

    if (defaultSelectionsRequest == nullptr) {
        logger->trace("[%s] processSeInserted => no default selection request" \
                      " defined, notify SE_INSERTED\n", this->getName());

        /* No default request is defined, just notify the SE insertion */
        notifyObservers(
            std::make_shared<ReaderEvent>(this->pluginName, this->name,
                                          ReaderEvent::EventType::SE_INSERTED,
                                          nullptr));
        presenceNotified = true;
    } else {
        /*
         * A default request is defined, send it and notify according to the
         * notification mode and the selection status
         */
        bool aSeMatched = false;

        try {
            std::list<std::shared_ptr<SeResponse>> seResponseList =
                 transmitSet(
                     defaultSelectionsRequest->getSelectionSeRequestSet(),
                     defaultSelectionsRequest->getMultiSeRequestProcessing(),
                     defaultSelectionsRequest->getChannelControl());

            for (auto seResponse : seResponseList) {
                if (seResponse != nullptr &&
                    seResponse->getSelectionStatus()->hasMatched()) {
                    logger->trace("[%s] processSeInserted => a default " \
                                  "selection has matched\n", this->getName());
                    aSeMatched = true;
                    break;
                }
            }

            if (notificationMode ==
                    ObservableReader::NotificationMode::MATCHED_ONLY) {
                /*
                 * Notify only if a SE matched the selection, just ignore if not
                 */
                if (aSeMatched) {
                    notifyObservers(
                        std::make_shared<ReaderEvent>(
                            this->pluginName, this->name,
                            ReaderEvent::EventType::SE_MATCHED,
                            std::make_shared<DefaultSelectionsResponse>(
                                seResponseList)));
                    presenceNotified = true;
                } else {
                    logger->trace("[%s] processSeInserted => selection hasn't" \
                                  " matched, do not throw any event because " \
                                  "of MATCHED_ONLY flag\n", this->getName());
                }
            } else {
                /* ObservableReader::NotificationMode::ALWAYS */
                if (aSeMatched) {
                    /*
                     * The SE matched, notify a SE_MATCHED event with the
                     * received response.
                     */
                    notifyObservers(
                        std::make_shared<ReaderEvent>(
                            this->pluginName, this->name,
                            ReaderEvent::EventType::SE_MATCHED,
                            std::make_shared<DefaultSelectionsResponse>(
                                seResponseList)));
                } else {
                    /*
                     * The SE didn't match, notify an SE_INSERTED event with the
                     * received response
                     */
                    logger->trace("[%s] processSeInserted => none of %d " \
                                  "default selection matched\n",
                                  this->getName(), seResponseList.size());
                    notifyObservers(
                        std::make_shared<ReaderEvent>(
                            this->pluginName, this->name,
                            ReaderEvent::EventType::SE_INSERTED,
                            std::make_shared<DefaultSelectionsResponse>(
                                seResponseList)));
                }
                presenceNotified = true;
            }
        } catch (const KeypleReaderException& e) {
            /*
             * The last transmission failed, close the logical and physical
             * channels.
             */
            closeLogicalAndPhysicalChannels();
            logger->debug("An IO Exception occurred while processing the " \
                          "default selection. %s\n", e.getMessage());
            /*
             * In this case the SE has been removed or not read correctly, do
             * not throw event
             */
        }
    }

    if (!presenceNotified) {
        /*
         * We close here the physical channel in case it has been opened for a
         * SE outside the expected SEs
         */
        try {
            closePhysicalChannel();
        } catch (const KeypleChannelControlException& e) {
            logger->error("Error while closing physical channel. %s\n",
                          e.getMessage());
        }
    }

    return presenceNotified;
}

bool AbstractObservableLocalReader::isSePresentPing()
{
    /* APDU sent to check the communication with the PO */
    std::vector<uint8_t> apdu = {0x00, 0xC0, 0x00, 0x00, 0x00};

    /* Transmits the APDU and checks for the IO exception */
    try {
        logger->trace("[%s] Ping SE\n", this->getName());
        transmitApdu(apdu);
    } catch (const KeypleIOReaderException& e) {
        logger->trace("[%s] Exception occurred in isSePresentPing. Message: " \
                      "%s\n", this->getName(), e.getMessage());
        return false;
    }

    return true;
}

void AbstractObservableLocalReader::processSeRemoved()
{
    closeLogicalAndPhysicalChannels();
    notifyObservers(
        std::make_shared<ReaderEvent>(
            this->pluginName, this->name,
            ReaderEvent::EventType::SE_REMOVED, nullptr));
}

ObservableReader::PollingMode AbstractObservableLocalReader::getPollingMode()
{
    return currentPollingMode;
}

void AbstractObservableLocalReader::switchState(
    const MonitoringState stateId)
{
    this->stateService->switchState(stateId);
}

MonitoringState AbstractObservableLocalReader::getCurrentMonitoringState()
{
    return this->stateService->getCurrentMonitoringState();
}

void AbstractObservableLocalReader::onEvent(const InternalEvent event)
{
    this->stateService->onEvent(event);
}

}
}
}
}
}