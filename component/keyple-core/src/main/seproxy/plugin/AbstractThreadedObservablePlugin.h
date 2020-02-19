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

#include <string>
#include <stdexcept>
#include <memory>

#include "exceptionhelper.h"

/* Core */
#include "AbstractPlugin.h"
#include "ObservablePlugin.h"
#include "ObservableReader_Import.h"
#include "SeReader.h"

/* Common */
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "Thread.h"

/* Forward class declarations  */
namespace keyple { namespace core { namespace seproxy { namespace plugin {
    class EventThread; } } } }

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::common;

/**
 * The {@link AbstractThreadedObservablePlugin} class provides the means to
 * observe a plugin (insertion/removal of readers) using a monitoring thread.
 */
class EXPORT AbstractThreadedObservablePlugin : public AbstractPlugin {
public:
    /**
     * Add a plugin observer.
     * <p>
     * The observer will receive all the events produced by this plugin (reader
     * insertion, removal, etc.)
     * <p>
     * In the case of a {@link AbstractThreadedObservablePlugin}, a thread is
     * created if it does not already exist (when the first observer is added).
     *
     * @param observer the observer object
     */
    void addObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer)
        override;

    /**
     * Remove a plugin observer.
     * <p>
     * The observer will do not receive any of the events produced by this
     * plugin.
     * <p>
     * In the case of a {@link AbstractThreadedObservablePlugin}, the monitoring
     * thread is ended when the last observer is removed.
     *
     * @param observer the observer object
     */
    void removeObserver(
        std::shared_ptr<ObservablePlugin::PluginObserver> observer)
        override;

    /**
     *
     */
    void clearObservers() override;

protected:
    /**
     * Instantiates an observable plugin
     *
     * @param name name of the plugin
     */
    AbstractThreadedObservablePlugin(const std::string& name);

    /**
     * Thread wait timeout in ms
     *
     * This timeout value will determined the latency to detect changes
     */
    long long threadWaitTimeout = SETTING_THREAD_TIMEOUT_DEFAULT;

    /**
     * Fetch the list of connected native reader (usually from third party
     * library) and returns their names (or id)
     *
     * @return connected readers' name list
     * @throws KeypleReaderException if a reader error occurs
     */
    virtual const std::set<std::string>& fetchNativeReadersNames() = 0;

    /**
     * Fetch connected native reader (from third party library) by its name
     * Returns the current {@link AbstractReader} if it is already listed.
     * Creates and returns a new {@link AbstractReader} if not.
     *
     * @param name the reader name
     * @return the list of AbstractReader objects.
     * @throws KeypleReaderException if a reader error occurs
     */
    virtual
       std::shared_ptr<SeReader> fetchNativeReader(const std::string& name) = 0;

    /**
     * Check weither the background job is monitoring for new readers
     *
     * @return true, if the background job is monitoring, false in all other
     *         cases.
     */
    bool isMonitoring();

    /**
     * Called when the class is unloaded. Attempt to do a clean exit.
     *
     * @throws Throwable a generic exception
     */
    void finalize() /* override */;

    /**
     *
     */
    std::shared_ptr<AbstractThreadedObservablePlugin> shared_from_this()
    {
        return std::static_pointer_cast<AbstractThreadedObservablePlugin>(
            AbstractPlugin::shared_from_this());
    }

private:
    /**
     * Reader insertion/removal management
     */
    static const long SETTING_THREAD_TIMEOUT_DEFAULT = 1000;

    /**
     *
     */
    class EventThread : public Thread {
    public:
        /**
         * Constructor
         */

        EventThread(
            std::shared_ptr<AbstractThreadedObservablePlugin> outerInstance,
            const std::string &pluginName);

        /**
         * Marks the thread as one that should end when the last cardWaitTimeout
         * occurs
         */
        virtual void end();

        /**
         *
         */
        virtual void *run();

        /**
         *
         */
        virtual ~EventThread() {}

        /**
         *
         */
        bool isMonitoring();

        /**
         *
         */
        bool isAlive();

    private:
        /**
         *
         */
        std::shared_ptr<AbstractThreadedObservablePlugin> outerInstance;

        /**
         *
         */
        const std::string pluginName;

        /**
         *
         */
        bool running = true;
    };

    /**
     *
     */
    const std::shared_ptr<Logger> logger =
             LoggerFactory::getLogger(typeid(AbstractThreadedObservablePlugin));

    /**
     * Local thread to monitoring readers presence
     */
    std::shared_ptr<EventThread> thread;

    /**
     * List of names of the physical (native) connected readers This list helps
     * synchronizing physical readers managed by third-party library such as
     * smardcard.io and the list of keyple {@link keyple::seproxy::SeReader}
     * Insertion, removal, and access operations safely execute concurrently by
     * multiple threads.
     */
    std::set<std::string> nativeReadersNames;
};

}
}
}
}
