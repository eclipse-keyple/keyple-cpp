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
#include "LoggerFactory.h"

/* Core */
#include "KeypleCoreExport.h"
#include "MonitoringJob.h"
#include "MonitoringPool.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {

using namespace keyple::common;

class AbstractObservableLocalReader;
enum class InternalEvent;

/**
 * The states that the reader monitoring currentState machine can have
 */
enum class MonitoringState {
    WAIT_FOR_START_DETECTION,
    WAIT_FOR_SE_INSERTION,
    WAIT_FOR_SE_PROCESSING,
    WAIT_FOR_SE_REMOVAL
};

/**
 * Defines a state behaviour for a {@link AbstractObservableLocalReader} Handles
 * {@link AbstractObservableLocalReader.InternalEvent} that might results on a
 * switch of state.
 */
class KEYPLECORE_API AbstractObservableState {
public:
    /**
     * Get the current state identifier of the state machine
     *
     * @return the current state identifier
     */
    MonitoringState getMonitoringState();

    /**
     * Handle Internal Event Usually state is switched using method
     * reader::switchState
     *
     * @param event internal event received by reader
     */
    virtual void onEvent(const InternalEvent event) = 0;

    /**
     * Invoked when activated, a custom behaviour can be added here
     */
    void onActivate();

    /**
     * Invoked when deactivated
     */
    void onDeactivate();

protected:
    /**
     * Identifier of the currentState
     */
    MonitoringState state;

    /**
     * Reference to Reader
     */
    AbstractObservableLocalReader* reader;

    /**
     * Background job definition if any
     */
    std::shared_ptr<MonitoringJob> monitoringJob;

    /**
     * Result of the background job if any
     */
    std::future<void>* monitoringEvent;

    /**
     * Executor service used to execute MonitoringJob
     */
    std::shared_ptr<MonitoringPool> executorService;

    /**
     *
     */
    std::atomic<bool> cancellationFlag;

    /**
     * Create a new state with a state identifier and a monitor job
     *
     * @param state the state identifier
     * @param reader the current reader
     * @param monitoringJob the job to be executed in background (may be null if
     *        no background job is required)
     * @param executorService the executor service
     */
    AbstractObservableState(MonitoringState state,
                            AbstractObservableLocalReader* reader,
                            std::shared_ptr<MonitoringJob> monitoringJob,
                            std::shared_ptr<MonitoringPool> executorService);

    /**
     * Create a new state with a state identifier
     *
     * @param reader : observable reader this currentState is attached to
     * @param state : name of the currentState
     */
    AbstractObservableState(MonitoringState state,
                            AbstractObservableLocalReader* reader);

    /**
     * Switch state in the parent reader
     *
     * @param stateId the new state
     */
    void switchState(MonitoringState stateId);

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(AbstractObservableState));
};

KEYPLECORE_API std::ostream& operator<<(std::ostream& os,
                                        const MonitoringState& ms);

}
}
}
}
}
