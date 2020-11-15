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

#pragma once

/* Core */
#include "AbstractMonitoringJob.h"
#include "AbstractObservableLocalReader.h"
#include "AbstractObservableState.h"
#include "KeypleCoreExport.h"
#include "MonitoringPool.h"

/* Common */
#include "LoggerFactory.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

/**
 * Wait for Start SE Detection
 * <p>
 * The state during which the reader does not wait for a SE to be inserted but for a signal from the
 * application to do so (switch to the WAIT_FOR_SE_INSERTION state).
 * <ul>
 * <li>Upon START_DETECT event, the machine changes state for WAIT_FOR_SE_INSERTION.
 * </ul>
 */
class KEYPLECORE_API WaitForStartDetect final : public AbstractObservableState {
public:
    /**
     *
     */
    WaitForStartDetect(AbstractObservableLocalReader* reader);

    /**
     *
     */
    WaitForStartDetect(AbstractObservableLocalReader* reader,
                       std::shared_ptr<AbstractMonitoringJob> monitoringJob,
                       std::shared_ptr<MonitoringPool> executorService);

    /**
     *
     */
    void onEvent(const InternalEvent event) override;

private:
    /**
     * Logger
     */
    const std::shared_ptr<Logger> mLogger = LoggerFactory::getLogger(typeid(WaitForStartDetect));
};

}
}
}
}
