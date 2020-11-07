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

/* Core */
#include "CardAbsentPingMonitoringJob.h"

/* Common */
#include "InterruptedException.h"
#include "Thread.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

CardAbsentPingMonitoringJob::CardAbsentPingMonitoringJob(AbstractObservableLocalReader* reader)
: mReader(reader) {}

CardAbsentPingMonitoringJob::CardAbsentPingMonitoringJob(AbstractObservableLocalReader* reader,
                                                         long removalWait)
: mReader(reader), mRemovalWait(removalWait) {}

void CardAbsentPingMonitoringJob::monitoringJob(AbstractObservableState* state,
                                                std::atomic<bool>& cancellationFlag)
{
    /*
     * Loop until one the following condition is met : -
     * AbstractObservableLocalReader#isSePresentPing returns false, meaning that the SE ping has
     * failed - InterruptedException is caught
     */

    long retries = 0;

    mLogger->debug("[%] Polling from isSePresentPing\n", mReader->getName());

    /* Re-init loop value to true */
    mLoop = true;

    while (mLoop) {
        if (cancellationFlag) {
            mLogger->debug("[%] monitoring job cancelled\n",
                           mReader->getName());
            return;
        }

        if (!mReader->isSePresentPing()) {
            mLogger->debug("[%] The SE stopped responding\n",
                           mReader->getName());
            mLoop = false;
            state->onEvent(InternalEvent::SE_REMOVED);
            return;
        }

        retries++;

        mLogger->trace("[%] Polling retries : %\n", mReader->getName(),
                      retries);

        try {
            /* Wait for a bit */
            Thread::sleep(mRemovalWait);
        } catch (const InterruptedException& ignored) {
            (void)ignored;
            /* Restore interrupted state... */
            mLoop = false;
            std::terminate();
        }
    }

    mLogger->debug("[%] Polling loop has been stopped\n", mReader->getName());
}

void CardAbsentPingMonitoringJob::stop()
{
    mLogger->debug("[%] Stop Polling\n", mReader->getName());

    mLoop = false;
}

std::future<void> CardAbsentPingMonitoringJob::startMonitoring(
    AbstractObservableState* state, std::atomic<bool>& cancellationFlag)
{
    return std::async(std::launch::async,
                      &CardAbsentPingMonitoringJob::monitoringJob, this, state,
                      std::ref(cancellationFlag));
}

}
}
}
}
