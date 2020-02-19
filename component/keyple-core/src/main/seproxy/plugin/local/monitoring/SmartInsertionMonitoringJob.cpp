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

#include "SmartInsertionMonitoringJob.h"

/* Core */
#include "AbstractObservableLocalReader.h"
#include "KeypleIOReaderException.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {
namespace monitoring {

SmartInsertionMonitoringJob::SmartInsertionMonitoringJob(
  SmartInsertionReader* reader)
: reader(reader)
{

}

void SmartInsertionMonitoringJob::monitoringJob(
    AbstractObservableState* state, std::atomic<bool>& cancellationFlag)
{
    (void)cancellationFlag;

    logger->trace("[%s] Invoke waitForCardPresent asynchronously\n",
                  reader->getName());

    try {
        if (reader->waitForCardPresent()) {
            state->onEvent(InternalEvent::SE_INSERTED);
        }
    } catch (KeypleIOReaderException &e) {
        logger->trace("[%s] waitForCardPresent => Error while polling SE with" \
                      " waitForCardPresent\n", reader->getName());
        state->onEvent(InternalEvent::STOP_DETECT);
    }
}

std::future<void> SmartInsertionMonitoringJob::startMonitoring(
    AbstractObservableState* state, std::atomic<bool>& cancellationFlag)
{
    return std::async(std::launch::async,
                      &SmartInsertionMonitoringJob::monitoringJob,
                      this, state, std::ref(cancellationFlag));
}

void SmartInsertionMonitoringJob::stop()
{
    logger->trace("[%s] stopWaitForCard on reader\n", reader->getName());
    reader->stopWaitForCard();
}

}
}
}
}
}
}
