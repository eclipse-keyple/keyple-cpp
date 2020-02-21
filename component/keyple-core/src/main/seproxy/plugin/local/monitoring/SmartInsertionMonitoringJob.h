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
#include "AbstractObservableState.h"
#include "MonitoringJob.h"
#include "SmartInsertionReader.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {
namespace monitoring {

/**
 * Detect the SE insertion thanks to the method {@link
 * SmartInsertionReader#waitForCardPresent()}. This method is invoked in another
 * thread.
 * <p>
 * The job waits indefinitely for the waitForCardPresent method to return.
 * <p>
 * When an SE is present, an internal SE_INSERTED event is fired.
 * <p>
 * If a communication problem with the reader occurs (KeypleIOReaderException)
 * an internal STOP_DETECT event is fired.
 */
class EXPORT SmartInsertionMonitoringJob : public MonitoringJob {
public:
    /**
     *
     */
    SmartInsertionMonitoringJob(SmartInsertionReader* reader);

    /**
     *
     */
    std::future<void>
    startMonitoring(AbstractObservableState* state,
                    std::atomic<bool>& cancellationFlag) override;

    /**
     *
     */
    void monitoringJob(AbstractObservableState* state,
                       std::atomic<bool>& cancellationFlag);

    /**
     *
     */
    void stop() override;

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(SmartInsertionMonitoringJob));

    /**
     *
     */
    SmartInsertionReader* reader;
};

}
}
}
}
}
}
