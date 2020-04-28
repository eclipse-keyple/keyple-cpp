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

#include "WaitForStartDetect.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {
namespace state {

WaitForStartDetect::WaitForStartDetect(AbstractObservableLocalReader* reader)
: AbstractObservableState(MonitoringState::WAIT_FOR_START_DETECTION, reader)
{
}

WaitForStartDetect::WaitForStartDetect(
    AbstractObservableLocalReader* reader,
    std::shared_ptr<MonitoringJob> monitoringJob,
    std::shared_ptr<MonitoringPool> executorService)
: AbstractObservableState(MonitoringState::WAIT_FOR_START_DETECTION, reader,
                          monitoringJob, executorService)
{
}

void WaitForStartDetect::onEvent(const InternalEvent event)
{
    logger->trace("[%] onEvent => Event % received in currentState %\n",
                  reader->getName(), event, state);

    /*
     * Process InternalEvent
     */
    switch (event) {
    case InternalEvent::START_DETECT:
        switchState(MonitoringState::WAIT_FOR_SE_INSERTION);
        break;

    default:
        logger->warn("[%] Ignore =>  Event % received in currentState %\n",
                     reader->getName(), event, state);
        break;
    }
}

}
}
}
}
}
}
