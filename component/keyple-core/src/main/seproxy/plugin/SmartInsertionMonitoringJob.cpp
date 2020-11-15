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

#include "SmartInsertionMonitoringJob.h"

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
using namespace keyple::core::seproxy::exception;

SmartInsertionMonitoringJob::SmartInsertionMonitoringJob(SmartInsertionReader* reader)
: mReader(reader) {}

void SmartInsertionMonitoringJob::monitoringJob(AbstractObservableState* state,
                                                std::atomic<bool>& cancellationFlag)
{
    if (!mReader || mReader->mShuttingDown)
        return;

    mRunning = true;

    mLogger->trace("[%] Invoke waitForCardPresent asynchronously\n", mReader->getName());

    try {
        if (cancellationFlag) {
            mRunning = false;
            throw InterruptedException("monitoring job interrupted");
        }

        if (mReader->waitForCardPresent()) {
            mLogger->debug("throwing card inserted event\n");
            state->onEvent(InternalEvent::SE_INSERTED);
        }

    } catch (KeypleReaderIOException& e) {
        mLogger->trace("[%] waitForCardPresent => Error while polling SE with waitForCardPresent. %\n",
                       mReader->getName(),
                       e.getMessage());
        state->onEvent(InternalEvent::STOP_DETECT);
    }

    mRunning = false;
}

std::future<void> SmartInsertionMonitoringJob::startMonitoring(AbstractObservableState* state,
                                                               std::atomic<bool>& cancellationFlag)
{
    return std::async(std::launch::async,
                      &SmartInsertionMonitoringJob::monitoringJob,
                      this,
                      state,
                      std::ref(cancellationFlag));
}

void SmartInsertionMonitoringJob::stop()
{
    mLogger->trace("[%] stopWaitForCard on reader\n", mReader->getName());
    mReader->stopWaitForCard();
}

}
}
}
}
