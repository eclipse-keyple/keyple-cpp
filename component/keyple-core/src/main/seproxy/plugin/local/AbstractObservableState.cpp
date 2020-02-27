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

#include "AbstractObservableState.h"

#include <chrono>

/* Core */
#include "AbstractObservableLocalReader.h"

/* Common */
#include "exceptionhelper.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {

AbstractObservableState::AbstractObservableState(
    MonitoringState state, AbstractObservableLocalReader* reader,
    std::shared_ptr<MonitoringJob> monitoringJob,
    std::shared_ptr<MonitoringPool> executorService)
: state(state), reader(reader), monitoringJob(monitoringJob),
  executorService(executorService), cancellationFlag()
{
}

AbstractObservableState::AbstractObservableState(
    MonitoringState state, AbstractObservableLocalReader* reader)
: state(state), reader(reader), cancellationFlag()
{
}

MonitoringState AbstractObservableState::getMonitoringState()
{
    return state;
}

void AbstractObservableState::switchState(const MonitoringState stateId)
{
    reader->switchState(stateId);
}

void AbstractObservableState::onActivate()
{
    logger->trace("[%s] onActivate => %d\n", this->reader->getName().c_str(),
                  this->getMonitoringState());

    /* Launch the monitoringJob is necessary */
    if (monitoringJob != nullptr) {
        if (executorService == nullptr) {
            throw AssertionError("ExecutorService must be set");
        }

        monitoringEvent =
            executorService->submit(monitoringJob, this, cancellationFlag);
    }
}

void AbstractObservableState::onDeactivate()
{
    logger->trace("[%s] onDeactivate => %d\n", this->reader->getName().c_str(),
                  this->getMonitoringState());

    /* Cancel the monitoringJob is necessary */
    if (monitoringEvent != nullptr &&
        monitoringEvent->wait_for(std::chrono::seconds(0)) !=
            std::future_status::ready) {

        logger->debug("onDeactivate - cancelling monitoring job\n");
        monitoringJob->stop();

        /* TODO this could be inside the stop method? */
        cancellationFlag = true;
        monitoringEvent->wait();
        cancellationFlag = false;
        logger->trace("[%s] onDeactivate => cancel runnable waitForCarPresent"
                      " by thead interruption\n",
                      reader->getName());
    }
}

}
}
}
}
}
