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

#include "AbstractReaderObserverEngine.h"

#include <thread>

/* Core */
#include "AbstractDefaultSelectionsResponse.h"
#include "ObservableReader.h"
#include "ReaderEvent.h"
#include "SeProxyService.h"

namespace keyple {
namespace example {
namespace generic {
namespace common {

using namespace keyple::core::seproxy::event;

void AbstractReaderObserverEngine::runProcessSeInserted(const std::shared_ptr<ReaderEvent> event)
{
    /* Run the PO processing asynchronously in a detach thread */
    std::thread(
        [this, event]() {
            mCurrentlyProcessingSe = true;
            /* Optional, to process alternative AID selection */
            processSeInserted();

            /**
             * Informs the underlying layer of the end of the SE processing, in order to manage
             * the removal sequence.
             * <p>
             * If closing has already been requested, this method will do nothing.
             */
            try {
                std::shared_ptr<SeReader> reader =
                    SeProxyService::getInstance().getPlugin(event->getPluginName())
                                                ->getReader(event->getReaderName());
                auto observable = std::dynamic_pointer_cast<ObservableReader>(reader);
            } catch (const KeypleReaderNotFoundException& e) {
                mLogger->error("Reader not found exception: {}", e.getMessage());
            } catch (const KeyplePluginNotFoundException& e) {
                mLogger->error("Plugin not found exception: {}", e.getMessage());
            }

            mCurrentlyProcessingSe = false;
        });
}

void AbstractReaderObserverEngine::runProcessSeMatched(const std::shared_ptr<ReaderEvent> event)
{
    /* Run the PO processing asynchronously in a detach thread */
    std::thread(
        [this, event]() {
            mCurrentlyProcessingSe = true;
            try {
                /* To process the selection application */
                processSeMatch(event->getDefaultSelectionsResponse());
            } catch (const KeypleException& e) {
                mLogger->error("Keyple exception: %\n", e.getMessage());
            }

            /**
             * Informs the underlying layer of the end of the SE processing, in order to manage
             * the removal sequence.
             * <p>
             * If closing has already been requested, this method will do nothing.
             */
            try {
                std::shared_ptr<SeReader> reader =
                    SeProxyService::getInstance().getPlugin(event->getPluginName())
                                                ->getReader(event->getReaderName());
                auto observable = std::dynamic_pointer_cast<ObservableReader>(reader);
                observable->notifySeProcessed();
            } catch (KeypleReaderNotFoundException e) {
                mLogger->error("Reader not found exception: {}", e.getMessage());
            } catch (KeyplePluginNotFoundException e) {
                mLogger->error("Plugin not found exception: {}", e.getMessage());
            }

            mCurrentlyProcessingSe = false;
        });
}

void AbstractReaderObserverEngine::update(std::shared_ptr<ReaderEvent> event)
{
    mLogger->info("New reader event: %\n", event->getReaderName());

    if (event->getEventType() == ReaderEvent::EventType::SE_INSERTED) {
        runProcessSeInserted(event);

    } else if (event->getEventType() == ReaderEvent::EventType::SE_MATCHED) {
        runProcessSeMatched(event);

    } else if (event->getEventType() == ReaderEvent::EventType::SE_REMOVED) {
        if (mCurrentlyProcessingSe) {
            processUnexpectedSeRemoval(); // to clean current SE processing
            mLogger->error("Unexpected SE Removal\n");
        } else {
            processSeRemoval();
            mLogger->info("Waiting for a SE...\n");
        }
        mCurrentlyProcessingSe = false;

     } else if (event->getEventType() == ReaderEvent::EventType::TIMEOUT_ERROR){
        mLogger->error("Timeout error: the processing time or the time limit for removing the SE " \
                       "has been exceeded\n");
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
