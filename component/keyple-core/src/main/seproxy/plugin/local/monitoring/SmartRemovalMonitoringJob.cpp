/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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
#include "AbstractObservableLocalReader.h"
#include "KeypleReaderIOException.h"
#include "SmartRemovalMonitoringJob.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {
namespace monitoring {

SmartRemovalMonitoringJob::SmartRemovalMonitoringJob(SmartRemovalReader* reader)
: mReader(reader)
{
}

void SmartRemovalMonitoringJob::monitoringJob(
    AbstractObservableState* state, std::atomic<bool>& cancellationFlag)
{
    (void)cancellationFlag;

    try {
        if (mReader->waitForCardAbsentNative()) {
            // timeout is already managed within the task
            state->onEvent(InternalEvent::SE_REMOVED);
        } else {
            mLogger->trace("[%] waitForCardAbsentNative => return false, task"
                          " interrupted\n", mReader->getName());
        }
    } catch (const KeypleReaderIOException& e) {
        mLogger->trace("[%] waitForCardAbsent => Error while polling SE with "
                      "waitForCardAbsent, %\n", mReader->getName(),
			          e.getMessage());

        state->onEvent(InternalEvent::STOP_DETECT);
    }
}

std::future<void>
SmartRemovalMonitoringJob::startMonitoring(AbstractObservableState* state,
                                           std::atomic<bool>& cancellationFlag)
{
    return std::async(std::launch::async,
                      &SmartRemovalMonitoringJob::monitoringJob, this, state,
                      std::ref(cancellationFlag));
}

void SmartRemovalMonitoringJob::stop()
{
    mLogger->trace("[%] stopWaitForCardRemoval on reader\n",
                   mReader->getName());

    if (mReader)
        mReader->stopWaitForCardRemoval();
}

}
}
}
}
}
}
