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

#include "AbstractObservableState.h"

#include <chrono>

/* Common */
#include "AssertionError.h"

/* Core */
#include "AbstractObservableLocalReader.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

AbstractObservableState::AbstractObservableState(
  MonitoringState state,
  AbstractObservableLocalReader* reader,
  std::shared_ptr<AbstractMonitoringJob> mMonitoringJob,
  std::shared_ptr<ExecutorService> executorService)
: mMonitoringJob(mMonitoringJob),
  mState(state),
  mReader(reader),
  mMonitoringEvent(nullptr),
  mExecutorService(executorService),
  mCancellationFlag() {}

AbstractObservableState::AbstractObservableState(
    MonitoringState state, AbstractObservableLocalReader* reader)
: mState(state), mReader(reader), mMonitoringEvent(nullptr), mCancellationFlag() {}

const MonitoringState& AbstractObservableState::getMonitoringState() const
{
    return mState;
}

void AbstractObservableState::switchState(const MonitoringState stateId)
{
    mReader->switchState(stateId);
}

void AbstractObservableState::onActivate()
{
    /* C++ vs. Java: thread management */
    if (mReader->mShuttingDown)
        return;

    mLogger->trace("[%] onActivate => %\n", mReader->getName(),  getMonitoringState());

    mCancellationFlag = false;

    /* Launch the mMonitoringJob is necessary */
    if (mMonitoringJob != nullptr) {
        if (mExecutorService == nullptr)
            throw AssertionError("ExecutorService must be set");

        mMonitoringEvent = mExecutorService->submit(mMonitoringJob, this, mCancellationFlag);
    }
}

void AbstractObservableState::onDeactivate()
{
    /* C++ vs. Java: thread management */
    if (mReader->mShuttingDown)
        return;

    mLogger->trace("[%] onDeactivate => %\n", mReader->getName(), getMonitoringState());

    /* Cancel the mMonitoringJob if necessary */
    if (mMonitoringEvent != nullptr) {// &&
        //mMonitoringEvent->wait_for(std::chrono::seconds(0)) != std::future_status::ready) {

        mLogger->debug("onDeactivate - cancelling monitoring job\n");
        mMonitoringJob->stop();

        /* TODO this could be inside the stop method? */
        mCancellationFlag = true;
        mMonitoringEvent = nullptr;
        mLogger->trace("[%] onDeactivate => cancel runnable waitForCarPresent by thead interrupt\n",
                       mReader->getName());
    }
}

std::ostream& operator<<(std::ostream& os, const MonitoringState& ms)
{
    switch (ms) {
    case MonitoringState::WAIT_FOR_START_DETECTION:
        os << "WAIT_FOR_START_DETECTION";
        break;
    case MonitoringState::WAIT_FOR_SE_INSERTION:
        os << "WAIT_FOR_SE_INSERTION";
        break;
    case MonitoringState::WAIT_FOR_SE_PROCESSING:
        os << "WAIT_FOR_SE_PROCESSING";
        break;
    case MonitoringState::WAIT_FOR_SE_REMOVAL:
        os << "WAIT_FOR_SE_REMOVAL";
        break;
    }

    return os;
}

}
}
}
}
