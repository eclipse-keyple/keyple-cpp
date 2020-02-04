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

#pragma once

/* Common */
#include "Export.h"
#include "LoggerFactory.h"

/* Core */
#include "AbstractObservableLocalReader.h"
#include "AbstractObservableState.h"
#include "MonitoringJob.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {
namespace monitoring {

using namespace keyple::common;

/**
 * Ping the SE to detect removal thanks to the method
 * {@link AbstractObservableLocalReader#isSePresentPing()}. This method is
 * invoked in another thread.
 * <p>
 * This job should be used by readers who do not have the ability to natively
 * detect the disappearance of the SE at the end of the transaction.
 * <p>
 * It is based on sending a neutral APDU command as long as the SE is
 * responding, an internal SE_REMOVED event is fired when the SE is no longer
 * responding.
 * <p>
 * A delay of 200 ms is inserted between each APDU sending.
 */
class EXPORT CardAbsentPingMonitoringJob : public MonitoringJob {
public:
    /**
     *
     */
    CardAbsentPingMonitoringJob(const AbstractObservableLocalReader& reader);

    /**
     *
     */
    std::future<void> startMonitoring(AbstractObservableState* state,
        std::atomic<bool>& cancellationFlag) override;

    /**
     *
     */
    void monitoringJob(AbstractObservableState* state,
                       std::atomic<bool>& cancellationFlag);


private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
              LoggerFactory::getLogger(typeid(CardAbsentPingMonitoringJob));

    /**
     *
     */
    AbstractObservableLocalReader& reader;
};

}
}
}
}
}
}
