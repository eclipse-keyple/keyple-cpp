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

#include <mutex>

/* Core */
#include "AbstractPlugin.h"
#include "KeypleCoreExport.h"
#include "ObservablePluginNotifier.h"
#include "PluginEvent.h"

/* Common */
#include "LoggerFactory.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy::plugin::local;

/**
 * The {@link AbstractObservablePlugin} class provides the means to observe a
 * plugin (insertion/removal of readers).
 */
class KEYPLECORE_API AbstractObservablePlugin
: public AbstractPlugin, public virtual ObservablePluginNotifier {
public:
    /**
     * Add a plugin observer.
     * <p>
     * The observer will receive all the events produced by this plugin (reader
     * insertion, removal, etc.)
     *
     * @param observer the observer object
     */
    virtual void addObserver(
        const std::shared_ptr<ObservablePlugin::PluginObserver> observer)
        override;

    /**
     * Remove a plugin observer.
     * <p>
     * The observer will do not receive any of the events produced by this plugin.
     *
     * @param observer the observer object
     */
    virtual void removeObserver(
        const std::shared_ptr<ObservablePlugin::PluginObserver> observer)
        override;

    /**
     * Remove all observers at once
     *
     * @deprecated will change in a later version
     */
    virtual void clearObservers() override;

    /**
     * @return the number of observers
     */
    virtual int countObservers() const final;

    /**
     * This method shall be called only from a SE Proxy plugin implementing
     * AbstractPlugin. Push a PluginEvent of the selected AbstractPlugin to its
     * registered Observer.
     *
     * @param event the event
     */
    virtual void notifyObservers(const std::shared_ptr<PluginEvent> event)
        final;

protected:
    /**
     * Instantiates a observable plugin.
     *
     * @param name name of the plugin
     * @throw KeypleReaderException when an issue is raised with reader
     */
    AbstractObservablePlugin(const std::string& name);

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(AbstractObservablePlugin));

    /**
     * The observers of this object
     */
    std::list<std::shared_ptr<ObservablePlugin::PluginObserver>> observers;

    /*
     * This object will be used to synchronize the access to the observers list
     * in order to be thread safe
     */
    std::mutex mtx;
};

}
}
}
}
