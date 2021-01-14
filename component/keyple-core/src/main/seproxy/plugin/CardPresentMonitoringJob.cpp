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
#include "CardPresentMonitoringJob.h"

/* Common */
#include "InterruptedException.h"
#include "KeypleReaderIOException.h"
#include "Thread.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

CardPresentMonitoringJob::CardPresentMonitoringJob(SeReader* reader,
                                                   long waitTimeout,
                                                   bool monitorInsertion)
: mReader(reader), mWaitTimeout(waitTimeout), mMonitorInsertion(monitorInsertion) {}

void CardPresentMonitoringJob::monitoringJob(AbstractObservableState* state,
                                             std::atomic<bool>& cancellationFlag)
{
    long retries = 0;

    mLogger->debug("[%] Polling from isSePresentPing\n", mReader->getName());

    /* Re-init loop value to true */
    mLoop = true;

    while (mLoop) {
        if (cancellationFlag) {
            mLogger->debug("[%] monitoring job cancelled\n", mReader->getName());
            return;
        }

        try {
            /* Polls for SE_INSERTED */
            if (mMonitorInsertion && mReader->isSePresent()) {
                mLogger->debug("[%] The SE is present\n", mReader->getName());
                mLoop = false;
                state->onEvent(InternalEvent::SE_INSERTED);
                return;
            }

            /* Polls for SE_REMOVED */
            if (!mMonitorInsertion && !mReader->isSePresent()) {
                mLogger->debug("[%] The SE is not present\n", mReader->getName());
                mLoop = false;
                state->onEvent(InternalEvent::SE_REMOVED);
                return;
            }

        } catch (KeypleReaderIOException& e) {
            (void)e;
            mLoop = false;
            /* What do do here */
        }

        retries++;

        mLogger->trace("[%] isSePresent polling retries : %\n", mReader->getName(), retries);

        try {
            /* Wait a bit */
            Thread::sleep(mWaitTimeout);
        } catch (InterruptedException& ignored) {
            (void)ignored;
            /* Restore interrupted state... */
            //Thread.currentThread().interrupt();
            mLoop = false;
        }
    }

    mLogger->trace("[%] Looping has been stopped\n", mReader->getName());
}

void CardPresentMonitoringJob::stop()
{
    mLogger->debug("[%] Stop Polling\n", mReader->getName());
    mLoop = false;
}

std::future<void> CardPresentMonitoringJob::startMonitoring(AbstractObservableState* state,
                                                            std::atomic<bool>& cancellationFlag)
{
    return std::async(std::launch::deferred,
                      &CardPresentMonitoringJob::monitoringJob,
                      this,
                      state,
                      std::ref(cancellationFlag));
}

}
}
}
}
