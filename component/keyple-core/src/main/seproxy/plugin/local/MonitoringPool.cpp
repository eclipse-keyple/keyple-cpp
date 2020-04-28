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

#include "MonitoringPool.h"

/* Core */
#include "AbstractObservableState.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {

MonitoringPool::MonitoringPool()
{
}

std::future<void>*
MonitoringPool::submit(std::shared_ptr<MonitoringJob> monitoringJob,
                       AbstractObservableState* state,
                       std::atomic<bool>& cancellationFlag)
{
    pool.push_back(monitoringJob->startMonitoring(state, cancellationFlag));

    return &pool.back();
}

}
}
}
}
}