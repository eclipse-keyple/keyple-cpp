/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#include "AbstractStaticPlugin.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

AbstractStaticPlugin::AbstractStaticPlugin(const std::string &name)
: AbstractObservablePlugin(name)
{
}

void AbstractStaticPlugin::startObservation()
{
}

void AbstractStaticPlugin::stopObservation()
{
}

void AbstractStaticPlugin::addObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer)
{
    (void)observer;
    throw std::runtime_error("Abstract Static Plugin does not support Observers, do not use this function");
}

void AbstractStaticPlugin::removeObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer)
{
    (void)observer;
    throw std::runtime_error("Abstract Static Plugin does not support Observers, do not use this function");
}

}
}
}
}
