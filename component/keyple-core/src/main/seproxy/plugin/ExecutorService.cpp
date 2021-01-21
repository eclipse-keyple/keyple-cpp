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

#include "ExecutorService.h"

/* Core */
#include "AbstractMonitoringJob.h"
#include "AbstractObservableState.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

ExecutorService::ExecutorService() : mRunning(true), mTerminated(false)
{
    mThread = new std::thread(&ExecutorService::run, this);
}

ExecutorService::~ExecutorService()
{
    mRunning = false;

    while (!mTerminated)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void ExecutorService::run()
{
    /* Emulates a SingleThreadExecutor (e.g. only one thread at a time) */

    while (mRunning) {
        if (mPool.size()) {
            /* Start first service and wait until completion */
            std::future<void>& current = mPool[0];
            current.wait();

            /* Remove from vector */
            mPool.erase(mPool.begin());
        }
    }

    mTerminated = true;
}

std::future<void>* ExecutorService::submit(std::shared_ptr<AbstractMonitoringJob> monitoringJob,
                                           AbstractObservableState* state,
                                           std::atomic<bool>& cancellationFlag)
{
    mPool.push_back(monitoringJob->startMonitoring(state, cancellationFlag));

    return &mPool.back();
}

}
}
}
}