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

#include "ObservableReaderStateService.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

ObservableReaderStateService::ObservableReaderStateService(
    AbstractObservableLocalReader* reader,
    std::map<MonitoringState,
    std::shared_ptr<AbstractObservableState>> states,
    const MonitoringState initState)
: mReader(reader), mStates(states)
{
    switchState(initState);
}

ObservableReaderStateService::~ObservableReaderStateService()
{
    mLogger->trace("terminating states jobs\n");
    for (auto& state : mStates) {
        if (state.second && state.second->monitoringJob)
            state.second->monitoringJob->stop();
    }
}

void ObservableReaderStateService::onEvent(const InternalEvent event)
{
    mCurrentState->onEvent(event);
}

void ObservableReaderStateService::switchState(const MonitoringState stateId)
{
    /* C++ vs. Java: thread management */
    if (mReader->mShuttingDown)
        return;

    if (mCurrentState != nullptr) {
        mLogger->trace("[%] Switch currentState from % to %\n",
                      mReader->getName(),
                      mCurrentState->getMonitoringState(),
                      stateId);

        mCurrentState->onDeactivate();
    } else {
        mLogger->trace("[%] Switch to a new currentState %\n", mReader->getName(), stateId);
    }

    /* Switch currentState */
    mCurrentState = mStates.find(stateId)->second;

    mLogger->trace("[%] New currentState %\n", 
                   mReader->getName(),
                   mCurrentState->getMonitoringState());

    /* onActivate the new current state */
    mCurrentState->onActivate();
}

const std::shared_ptr<AbstractObservableState> ObservableReaderStateService::getCurrentState() const
{
    return mCurrentState;
}

const MonitoringState& ObservableReaderStateService::getCurrentMonitoringState() const
{
    return mCurrentState->getMonitoringState();
}

}
}
}
}
