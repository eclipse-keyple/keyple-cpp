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

#include "SmartRemovalMonitoringJob.h"

/* Core */
#include "AbstractObservableLocalReader.h"
#include "KeypleReaderIOException.h"
#include "InterruptedException.h"

/* Common */
#include "Thread.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::common;

SmartRemovalMonitoringJob::SmartRemovalMonitoringJob(SmartRemovalReader* reader)
: mReader(reader) {}

void SmartRemovalMonitoringJob::monitoringJob(AbstractObservableState* state,
                                              std::atomic<bool>& cancellationFlag)
{
    if (!mReader || mReader->mShuttingDown)
        return;

    mRunning = true;

    mLogger->trace("[%] Invoke waitForCardAbsentNative asynchronously\n", mReader->getName());

    try {
        if (cancellationFlag) {
            mRunning = false;
            throw InterruptedException("monitoring job interrupted");
        }

        if (mReader->waitForCardAbsentNative()) {
            // timeout is already managed within the task
            mLogger->debug("throwing card removed event\n");
            state->onEvent(InternalEvent::SE_REMOVED);
        }
    } catch (const KeypleReaderIOException& e) {
        mLogger->trace("[%] waitForCardAbsent => Error while polling SE with waitForCardAbsent, %\n",
                       mReader->getName(),
                       e.getMessage());

        state->onEvent(InternalEvent::STOP_DETECT);
    }

    mRunning = false;
}

std::future<void> SmartRemovalMonitoringJob::startMonitoring(AbstractObservableState* state,
                                                             std::atomic<bool>& cancellationFlag)
{
    return std::async(std::launch::deferred,
                      &SmartRemovalMonitoringJob::monitoringJob,
                      this,
                      state,
                      std::ref(cancellationFlag));
}

void SmartRemovalMonitoringJob::stop()
{
    mLogger->trace("[%] stopWaitForCardRemoval on reader\n", mReader->getName());
    mReader->stopWaitForCardRemoval();
}

}
}
}
}
