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
#include "SmartRemovalReader.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {
namespace monitoring {

using namespace keyple::common;

/**
 * Detect the SE removal thanks to the method {@link
 * SmartRemovalReader#waitForCardAbsentNative()}. This method is invoked in
 * another thread
 * <p>
 * This job should be used by readers who have the ability to natively detect
 * the disappearance of the SE during a communication session with an ES
 * (between two APDU exchanges).
 * <p>
 * PC/SC readers have this capability.
 * <p>
 * If the SE is removed during processing, then an internal SE_REMOVED event is
 * triggered.
 * <p>
 * If a communication problem with the reader occurs (KeypleIOReaderException)
 * an internal
 * STOP_DETECT event is fired.
 */
class SmartRemovalMonitoringJob : public MonitoringJob {
public:
    /**
     *
     */
    SmartRemovalMonitoringJob(SmartRemovalReader& reader);

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
              LoggerFactory::getLogger(typeid(SmartRemovalMonitoringJob));

    /**
     *
     */
    SmartRemovalReader& reader;
};

}
}
}
}
}
}
