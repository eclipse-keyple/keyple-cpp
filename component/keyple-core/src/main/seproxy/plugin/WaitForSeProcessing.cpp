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

#include "WaitForSeProcessing.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

WaitForSeProcessing::WaitForSeProcessing(AbstractObservableLocalReader* reader)
: AbstractObservableState(MonitoringState::WAIT_FOR_SE_PROCESSING, reader) {}

WaitForSeProcessing::WaitForSeProcessing(
    AbstractObservableLocalReader* reader,
    std::shared_ptr<AbstractMonitoringJob> monitoringJob,
    std::shared_ptr<MonitoringPool> executorService)
: AbstractObservableState(MonitoringState::WAIT_FOR_SE_PROCESSING,
                          reader,
                          monitoringJob,
                          executorService) {}

void WaitForSeProcessing::onEvent(const InternalEvent event)
{
    mLogger->trace("[%] onEvent => Event % received in currentState %\n",
                   mReader->getName(),
                   event,
                   mState);

    /* Process InternalEvent */
    switch (event) {
    case InternalEvent::SE_PROCESSED:
        if (this->mReader->getPollingMode() == ObservableReader::PollingMode::REPEATING) {
            switchState(MonitoringState::WAIT_FOR_SE_REMOVAL);
        } else {
            /* We close the channels now and notify the application of the SE_REMOVED event */
            this->mReader->processSeRemoved();
            switchState(MonitoringState::WAIT_FOR_START_DETECTION);
        }
        break;
    case InternalEvent::SE_REMOVED:
        /*
         * The SE has been removed, we close all channels and return to
         * the currentState of waiting for insertion
         * We notify the application of the SE_REMOVED event.
         */
        mReader->processSeRemoved();
        if (mReader->getPollingMode() == ObservableReader::PollingMode::REPEATING) {
            switchState(MonitoringState::WAIT_FOR_SE_INSERTION);
        } else {
            switchState(MonitoringState::WAIT_FOR_START_DETECTION);
        }
        break;
    case InternalEvent::STOP_DETECT:
        mReader->processSeRemoved();
        switchState(MonitoringState::WAIT_FOR_START_DETECTION);
        break;

    default:
        mLogger->warn("[%] Ignore =>  Event % received in currentState %\n",
                     mReader->getName(),
                     event,
                     mState);
        break;
    }
}

}
}
}
}
