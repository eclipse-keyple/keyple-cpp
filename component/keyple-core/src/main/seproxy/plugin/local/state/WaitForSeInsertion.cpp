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

/* Core */
#include "AbstractObservableState.h"
#include "WaitForSeInsertion.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {
namespace state {

WaitForSeInsertion::WaitForSeInsertion(AbstractObservableLocalReader& reader)
: AbstractObservableState(MonitoringState::WAIT_FOR_SE_INSERTION, reader)
{
}

WaitForSeInsertion::WaitForSeInsertion(
  AbstractObservableLocalReader& reader, MonitoringJob* monitoringJob,
  MonitoringPool* executorService)
: AbstractObservableState(MonitoringState::WAIT_FOR_SE_INSERTION, reader,
    monitoringJob, executorService)
{
}

void WaitForSeInsertion::onEvent(const InternalEvent event)
{
    logger->trace("[%s] onEvent => Event %d received in currentState %d\n",
                  reader.getName(), event, state);

    /*
     * Process InternalEvent
     */
    switch (event) {
    case InternalEvent::SE_INSERTED:
        /* Process default selection if any */
        if (this->reader.processSeInserted()) {
            switchState(MonitoringState::WAIT_FOR_SE_PROCESSING);
        } else {
            /*
             * If none event was sent to the application, back to SE detection
             * stay in the same state.
             */
            logger->trace("[%s] onEvent => Inserted SE hasn't matched\n",
                          reader.getName());
        }
        break;
    case InternalEvent::STOP_DETECT:
        switchState(MonitoringState::WAIT_FOR_START_DETECTION);
        break;
    case InternalEvent::SE_REMOVED:
        /*
         * TODO Check if this case really happens (NFC?)
         * SE has been removed during default selection
         */
        if (reader.getPollingMode() ==
            ObservableReader::PollingMode::REPEATING) {
            switchState(MonitoringState::WAIT_FOR_SE_INSERTION);
        } else {
            switchState(MonitoringState::WAIT_FOR_START_DETECTION);
        }
        break;
    default:
        logger->warn("[%s] Ignore =>  Event %d received in currentState %d\n",
                     reader.getName(), event, state);
        break;
    }
}

}
}
}
}
}
}
