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

#include <memory>

/* Core */
#include "KeypleCoreExport.h"
#include "ReaderPlugin.h"

/* Forward class declarations */
namespace keyple { namespace core { namespace seproxy { namespace event {
    class PluginEvent; } } } }

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using namespace keyple::core::seproxy;

/**
 * The ObservablePlugin interface provides the API to observe readers in
 * plugins.
 * <p>
 * Allows subscribed observers to receive a PluginEvent when a reader is
 * connected/disconnected
 */
class KEYPLECORE_API ObservablePlugin : public virtual ReaderPlugin {
public:
    /**
     * Interface to be implemented by plugin observers.
     */
    class PluginObserver {
    public:
        /**
         *
         */
        virtual ~PluginObserver()
        {
        }

        /**
         *
         */
        virtual void update(const std::shared_ptr<PluginEvent> event) = 0;
    };

    /**
     * Add a plugin observer.
     * <p>
     * The observer will receive all the events produced by this plugin (reader
     * connection, disconnection)
     *
     * @param observer the observer object
     */
    virtual
        void addObserver(const std::shared_ptr<PluginObserver> observer) = 0;

    /**
     * Remove a plugin observer.
     * <p>
     * The observer will no longer receive any of the events produced by this
     * plugin.
     *
     * @param observer the observer object
     */
    virtual
        void removeObserver(const std::shared_ptr<PluginObserver> observer) = 0;

    /**
     * Push a PluginEvent of the {@link ObservablePlugin} to its registered
     * observers.
     *
     * @param event the event (see {@link PluginEvent})
     */
    virtual void notifyObservers(const std::shared_ptr<PluginEvent> event) = 0;

    /**
     * Remove all observers at once
     */
    virtual void clearObservers() = 0;

    /**
     * @return the number of observers
     */
    virtual int countObservers() const = 0;
};

}
}
}
}
