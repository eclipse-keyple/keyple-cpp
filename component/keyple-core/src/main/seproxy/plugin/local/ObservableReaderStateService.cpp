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

#include "ObservableReaderStateService.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {

ObservableReaderStateService::ObservableReaderStateService(
    AbstractObservableLocalReader* reader,
    std::map<MonitoringState, std::shared_ptr<AbstractObservableState>>& states,
    const MonitoringState initState)
: reader(reader), states(states)
{
    switchState(initState);
}

void ObservableReaderStateService::onEvent(const InternalEvent event)
{
    this->currentState->onEvent(event);
}

void ObservableReaderStateService::switchState(const MonitoringState stateId)
{
    if (currentState != nullptr) {
        logger->trace("[%s] Switch currentState from %d to %d\n",
                      this->reader->getName().c_str(),
                      this->currentState->getMonitoringState(), stateId);

        currentState->onDeactivate();
    } else {
        logger->trace("[%s] Switch to a new currentState %d\n",
                      this->reader->getName().c_str(), stateId);
    }

    /* Switch currentState */
    currentState = this->states.find(stateId)->second;

    logger->debug("[%s] New currentState %d\n",
                  this->reader->getName().c_str(),
                  currentState->getMonitoringState());

    /* onActivate the new current state */
    currentState->onActivate();
}

std::shared_ptr<AbstractObservableState>
ObservableReaderStateService::getCurrentState()
{
    return currentState;
}

MonitoringState ObservableReaderStateService::getCurrentMonitoringState()
{
    return this->currentState->getMonitoringState();
}

}
}
}
}
}
