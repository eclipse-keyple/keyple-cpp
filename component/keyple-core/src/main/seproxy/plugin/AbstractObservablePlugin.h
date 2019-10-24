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
#include <set>
#include <string>

/* Common*/
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"

/* Core*/
#include "AbstractLoggedObservable.h"
#include "AbstractObservableReader.h"
#include "ReaderPlugin.h"
#include "ObservablePlugin.h"
#include "PluginEvent.h"
#include "ProxyReader.h"

/* Forward declaration */
namespace keyple { namespace core { namespace seproxy { namespace plugin {
    class AbstractObservableReader; }}}}

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::common;

/**
 * Observable plugin. These plugin can report when a reader is added or removed.
 */
class EXPORT AbstractObservablePlugin
: public AbstractLoggedObservable<PluginEvent>, public virtual ReaderPlugin {
public:
    /**
     *
     */
    void initReaders();

    /**
     * Returns the current readers list.
     *
     * The list is initialized in the constructor and may be updated in
     * background in the case of a threaded plugin {@link
     * AbstractThreadedObservablePlugin}
     *
     * @return the current reader list, can be null if the
     *
     * Alex: Java implementation returns a set of AbstractObservableReader(s)
     *       when the base class function returns.a set of SeReader(s). C++
     *       doesn't like this covariant return type even though
     *       AbstractObservableReader is derived from SeReader. That's probably
     *       due to set::set or std::shared_ptr preventing the base-derived
     *       mechanism to work.
     */
    std::shared_ptr<std::set<std::shared_ptr<SeReader>>> getReaders() override;

    /**
     * Returns the current list of reader names.
     *
     * The list of names is built from the current readers list
     *
     * @return a list of String
     */
    std::shared_ptr<std::set<std::string>> getReaderNames() override;

    /**
     * Add a plugin observer.
     * <p>
     * The observer will receive all the events produced by this plugin (reader
     * insertion, removal, etc.)
     * <p>
     * The monitoring thread is started when the first observer is added.
     *
     * @param observer the observer object
     */
    void addObserver(
      std::shared_ptr<ObservablePlugin::PluginObserver> observer);

    /**
     * Remove a plugin observer.
     * <p>
     * The observer will do not receive any of the events produced by this
     * plugin.
     * <p>
     * The monitoring thread is ended when the last observer is removed.
     *
     * @param observer the observer object
     */
    void removeObserver(
      std::shared_ptr<ObservablePlugin::PluginObserver> observer);

    /**
     * Compare the name of the current ReaderPlugin to the name of the
     * ReaderPlugin provided in argument
     *
     * @param plugin a {@link ReaderPlugin} object
     * @return true if the names match (The method is needed for the std::set
     * lists)
     */
    int compareTo(std::shared_ptr<ReaderPlugin> plugin);

    /**
     * Gets a specific reader designated by its name in the current readers list
     *
     * @param name of the reader
     * @return the reader
     * @throws KeypleReaderNotFoundException if the wanted reader is not found
     *
     * Alex: once again the std::shared_ptr return type prevents us from using
     *       covariant return type. Downcasting to SeReader for now, but should
     *       be looked into, maybe returning a reference would
     *       could be best here?
     */
    std::shared_ptr<SeReader> getReader(const std::string &name) override;

protected:
    /**
     *
     */
    std::shared_ptr<AbstractObservablePlugin> shared_from_this()
    {
        return std::static_pointer_cast<AbstractObservablePlugin>(
                   AbstractLoggedObservable<PluginEvent>::shared_from_this());
    }

    /**
     * The list of readers
     */
    std::shared_ptr<std::set<std::shared_ptr<SeReader>>> readers = nullptr;

    /**
     * Instanciates a new ReaderPlugin. Retrieve the current readers list.
     *
     * Gets the list for the native method the first time (null)
     *
     * @param name name of the plugin
     */
    AbstractObservablePlugin(const std::string &name);

    /**
     * Fetch connected native readers (from third party library) and returns a
     * list of corresponding
     * {@link org.eclipse.keyple.seproxy.plugin.AbstractObservableReader}
     * {@link org.eclipse.keyple.seproxy.plugin.AbstractObservableReader} are
     * new instances.
     *
     * @return the list of AbstractObservableReader objects.
     * @throws KeypleReaderException if a reader error occurs
     *
     * Alex: using SeReader instead of AbstractObservableReader
     */
    virtual
    std::shared_ptr<std::set<std::shared_ptr<SeReader>>> initNativeReaders() =0;

    /**
     * Fetch connected native reader (from third party library) by its name
     * Returns the current {@link
     * org.eclipse.keyple.seproxy.plugin.AbstractObservableReader} if it is
     * already listed. Creates and returns a new {@link
     * org.eclipse.keyple.seproxy.plugin.AbstractObservableReader} if not.
     *
     * @return the list of AbstractObservableReader objects.
     * @throws KeypleReaderException if a reader error occurs
     */
    virtual std::shared_ptr<AbstractObservableReader>
      fetchNativeReader(const std::string &name) = 0;

    /**
     * Starts the monitoring thread
     * <p>
     * This abstract method has to be implemented by the class that handle the
     * monitoring thread. It will be called when a first observer is added.
     */
    virtual void startObservation() = 0;

    /**
     * Ends the monitoring thread
     * <p>
     * This abstract method has to be implemented by the class that handle the
     * monitoring thread. It will be called when the observer is removed.
     */
    virtual void stopObservation() = 0;

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(AbstractObservablePlugin));
};

}
}
}
}
