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
#include "CardAbsentPingMonitoringJob.h"

/* Common */
#include "InterruptedException.h"
#include "Thread.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {
namespace monitoring {

CardAbsentPingMonitoringJob::CardAbsentPingMonitoringJob(
  AbstractObservableLocalReader* reader)
: reader(reader)
{

}

void CardAbsentPingMonitoringJob::monitoringJob(
    AbstractObservableState* state, std::atomic<bool>& cancellationFlag)
{
    long threshold = 200;
    long retries = 0;
    bool loop = true;

    logger->debug("[%s] Polling from isSePresentPing\n", reader->getName());
    while (loop) {
        if (cancellationFlag) {
            logger->debug("[%s] monitoring job cancelled\n", reader->getName());
            return;
        }

        if (!reader->isSePresentPing()) {
            logger->debug("[%s] The SE stopped responding\n", reader->getName());
            loop = false;
            state->onEvent(InternalEvent::SE_REMOVED);
            return;
        }

        retries++;

        logger->trace("[%s] Polling retries : %d\n", reader->getName(), retries);

        try {
            /* Wait for a bit */
            Thread::sleep(threshold);
        } catch (InterruptedException &ignored) {
            /* Restore interrupted state... */
            std::terminate();
            loop = false;
        }
    }
}

std::future<void> CardAbsentPingMonitoringJob::startMonitoring(
    AbstractObservableState* state, std::atomic<bool>& cancellationFlag)
{
    return std::async(std::launch::async,
                      &CardAbsentPingMonitoringJob::monitoringJob,
                      this, state, std::ref(cancellationFlag));
}

}
}
}
}
}
}
