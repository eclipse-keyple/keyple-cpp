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

#include "AbstractObservablePlugin.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

AbstractObservablePlugin::AbstractObservablePlugin(const std::string& name)
: AbstractPlugin(name)
{
}

void AbstractObservablePlugin::addObserver(
    const std::shared_ptr<ObservablePlugin::PluginObserver> observer)
{
    if (!observer)
        return;

    logger->trace("Adding '%' as an observer of '%'\n",
                  typeid(observer).name(), getName());

    mtx.lock();
    observers.push_back(observer);
    mtx.unlock();
}

void AbstractObservablePlugin::removeObserver(
    const std::shared_ptr<ObservablePlugin::PluginObserver> observer)
{
    if (!observer)
        return;

    logger->trace("[%] Deleting a plugin observer\n", getName());

    mtx.lock();
    observers.remove(observer);
    mtx.unlock();
}

void AbstractObservablePlugin::clearObservers()
{
	observers.clear();
}

int AbstractObservablePlugin::countObservers() const
{
    return observers.size();
}

void AbstractObservablePlugin::notifyObservers(
    const std::shared_ptr<PluginEvent> event)
{
    logger->trace("[%] Notifying a plugin event to % observers. EVENTNAME = " \
                  "%\n", getName(), countObservers(),
                  event->getEventType().getName());

    mtx.lock();
    std::list<std::shared_ptr<ObservablePlugin::PluginObserver>>
        observersCopy(observers);
    mtx.unlock();

    for (const auto& observer : observersCopy)
        observer->update(event);

}

}
}
}
}
