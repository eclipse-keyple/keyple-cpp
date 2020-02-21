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
 * This monitoring job polls the {@link SeReader#isSePresent()} method to detect
 * SE_INSERTED/SE_REMOVED
 */
class EXPORT CardPresentMonitoringJob : public MonitoringJob {
public:
    /**
     * Build a monitoring job to detect the card insertion
     *
     * @param reader : reader that will be polled with the method isSePresent()
     * @param waitTimeout : wait time during two hit of the polling
     * @param monitorInsertion : if true, polls for SE_INSERTED, else SE_REMOVED
     */
    CardPresentMonitoringJob(SeReader* reader, long waitTimeout,
                             bool monitorInsertion);

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
        LoggerFactory::getLogger(typeid(CardPresentMonitoringJob));

    /**
     *
     */
    SeReader* reader;

    /**
     *
     */
    std::atomic<bool> loop;

    /**
     *
     */
    long waitTimeout;

    /**
     *
     */
    bool monitorInsertion;
};

}
}
}
}
}
}
