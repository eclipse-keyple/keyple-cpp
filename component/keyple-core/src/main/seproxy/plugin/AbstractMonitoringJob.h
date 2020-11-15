/**************************************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                                                *
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

#include <future>

/* Core */
#include "KeypleCoreExport.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

class AbstractObservableState;

/**
 * Monitoring jobs abstract class.
 * <p>
 * Observable readers can instantiate {@link AbstractMonitoringJob} to perform background processing
 * during the different states of the generic state machine.
 * <p>
 * Internal events ({@link AbstractObservableLocalReader.InternalEvent}) can be fired to change the
 * state of the machine via the {@link AbstractObservableState} class passed as a constructor's
 * argument.
 * <p>
 * Standard {@link AbstractMonitoringJob} are already defined in the local.monitoring package but it
 * is possible to define new ones within a plugin reader if necessary, implementing this abstract
 * class.
 */
class KEYPLECORE_API AbstractMonitoringJob {
public:
    /**
     *
     */
    AbstractMonitoringJob()
    {
        mRunning = false;
    }

    /**
     *
     */
    virtual ~AbstractMonitoringJob()
    {
        stop();
    }

    /**
     *
     */
    virtual std::future<void> startMonitoring(AbstractObservableState* state,
                                              std::atomic<bool>& cancellationFlag) = 0;

    /**
     * Should stop/interrupt the monitoring job
     *
     * C++ vs. Java: should be pure virtual, but couln't call it from destructor
     */
    virtual void stop() {}

    /**
     *
     */
    bool isRunning() const
    {
        return mRunning;
    }

protected:
    /**
     * C++ vs. Java: running flag
     */
    std::atomic<bool> mRunning;
};

}
}
}
}
