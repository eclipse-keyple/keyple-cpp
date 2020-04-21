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

#include <thread>

/* Examples */
#include "AbstractReaderObserverEngine.h"

/* Core */
#include "AbstractDefaultSelectionsResponse.h"
#include "AbstractReader.h"
#include "ReaderEvent_Import.h"
#include "SeProxyService.h"

namespace keyple {
namespace example {
namespace generic {
namespace common {

using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::plugin;

void* AbstractReaderObserverEngine::runSeInsertedThread(
    std::shared_ptr<ReaderEvent> event)
{
    currentlyProcessingSe = true;
    processSeInserted();

    /**
     * Informs the underlying layer of the end of the SE processing, in order to
     * manage the removal sequence.
     * <p>
     * If closing has already been requested, this method will do nothing.
     */
    try {
        std::dynamic_pointer_cast<AbstractReader>(
            SeProxyService::getInstance().getPlugin(event->getPluginName())
                ->getReader(event->getReaderName()))->notifySeProcessed();
    } catch (KeypleReaderNotFoundException& e) {
        logger->debug("KeypleReaderNotFoundException: %\n", e);
    } catch (KeyplePluginNotFoundException& e) {
        logger->debug("KeyplePluginNotFoundException: %\n", e);
    }

    currentlyProcessingSe = false;

    return NULL;
}

void* AbstractReaderObserverEngine::runSeMatchedThread(
    std::shared_ptr<ReaderEvent> event)
{
    currentlyProcessingSe = true;
    processSeMatch(event->getDefaultSelectionsResponse());

    /**
     * Informs the underlying layer of the end of the SE processing, in order to
     * manage the removal sequence.
     * <p>
     * If closing has already been requested, this method will do nothing.
     */
    try {
        std::dynamic_pointer_cast<AbstractReader>(
            SeProxyService::getInstance().getPlugin(event->getPluginName())
                ->getReader(event->getReaderName()))->notifySeProcessed();
    } catch (KeypleReaderNotFoundException& e) {
        logger->debug("KeypleReaderNotFoundException: %\n", e);
    } catch (KeyplePluginNotFoundException& e) {
        logger->debug("KeyplePluginNotFoundException: %\n", e);
    }

    currentlyProcessingSe = false;

    return NULL;
}

void AbstractReaderObserverEngine::update(std::shared_ptr<ReaderEvent> event)
{
    logger->info("New reader event: %\n", event->getReaderName());

    if (event->getEventType() == ReaderEvent::EventType::SE_INSERTED) {
        /* Run the PO processing asynchronously in a detach thread */
        new std::thread(&AbstractReaderObserverEngine::runSeInsertedThread,
                        this, event);

    } else if (event->getEventType() == ReaderEvent::EventType::SE_MATCHED) {
        /* Run the PO processing asynchronously in a detach thread */
        new std::thread(&AbstractReaderObserverEngine::runSeMatchedThread,
                        this, event);

    } else if (event->getEventType() ==ReaderEvent::EventType::SE_REMOVED) {
        if (currentlyProcessingSe) {
            processUnexpectedSeRemoval(); // to clean current SE processing
            logger->error("Unexpected SE Removal\n");
        } else {
            processSeRemoval();
            logger->info("Waiting for a SE...\n");
        }
        currentlyProcessingSe = false;

     } else if (event->getEventType() == ReaderEvent::EventType::TIMEOUT_ERROR){
        logger->error("Timeout error: the processing time or the time limit " \
                      "for removing the SE has been exceeded\n");
        /*
         * Do the appropriate processing here but do not prevent the return of
         * this update method (e.g. by raising an exception).
         */
    }
}

}
}
}
}
