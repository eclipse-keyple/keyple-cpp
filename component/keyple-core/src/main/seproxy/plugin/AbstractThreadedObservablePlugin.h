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

#pragma once

#include <string>
#include <stdexcept>
#include <memory>

/* Common */
#include "LoggerFactory.h"
#include "Thread.h"

/* Core */
#include "AbstractObservablePlugin.h"
#include "KeypleCoreExport.h"
#include "ObservablePlugin.h"
#include "ObservableReader.h"
#include "SeReader.h"

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
class KEYPLECORE_API AbstractThreadedObservablePlugin : public AbstractObservablePlugin {
public:
    /**
     * Add a plugin observer.
     * <p>
     * Overrides the method defined in {@link AbstractObservablePlugin}, a
     * thread is created if it does not already exist (when the first observer
     * is added).
     *
     * @param observer the observer object
     */
    void addObserver(const std::shared_ptr<ObservablePlugin::PluginObserver> observer) final;

    /**
     * Remove a plugin observer.
     * <p>
     * Overrides the method defined in {@link AbstractObservablePlugin}, the
     * monitoring thread is ended when the last observer is removed.
     *
     * @param observer the observer object
     */
    void removeObserver(const std::shared_ptr<ObservablePlugin::PluginObserver> observer) final;

    /**
     * Remove all observers at once
     * <p>
     * Overrides the method defined in {@link AbstractObservablePlugin}, the
     * thread is ended.
     */
    void clearObservers() final;

    /**
     *
     */
    virtual ~AbstractThreadedObservablePlugin() = default;

protected:

    /**
     * Thread wait timeout in ms
     *
     * This timeout value will determined the latency to detect changes
     */
    long long mThreadWaitTimeout = SETTING_THREAD_TIMEOUT_DEFAULT;

    /**
     * Instantiates an threaded observable plugin
     *
     * @param name name of the plugin
     * @throw KeypleReaderNotFoundException if the reader was not found by its
     *        name
     * @throw KeypleReaderIOException if the communication with the reader or
     *        the SE has failed
     */
    AbstractThreadedObservablePlugin(const std::string& name);

    /**
     * Fetch the list of connected native reader (usually from third party
     * library) and returns their names (or id)
     *
     * @return connected readers' name list
     * @throw KeypleReaderIOException if the communication with the reader or
     *        the SE has failed
     */
    virtual const std::set<std::string>& fetchNativeReadersNames() = 0;

    /**
     * Fetch connected native reader (from third party library) by its name
     * Returns the current {@link AbstractReader} if it is already listed.
     * Creates and returns a new {@link AbstractReader} if not.
     *
     * @param name the reader name
     * @return the list of AbstractReader objects.
     * @throw KeypleReaderNotFoundException if the reader was not found by its
     *        name
     * @throw KeypleReaderIOException if the communication with the reader or
     *        the SE has failed
     */
    virtual std::shared_ptr<SeReader> fetchNativeReader(const std::string& name) = 0;

    /**
     * Check weither the background job is monitoring for new readers
     *
     * @return true, if the background job is monitoring, false in all other
     *         cases.
     * @deprecated will change in a later version
     */
    bool isMonitoring();

    /**
     * Called when the class is unloaded. Attempt to do a clean exit.
     *
     * @throws Throwable a generic exception
     * @deprecated will change in a later version
     */
    void finalize() /* override */;

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
         *
         * Uses a raw pointer to not mess with weak_ptr & outer instances
         */
        EventThread(AbstractThreadedObservablePlugin* outerInstance, const std::string& pluginName);

        /**
         * Marks the thread as one that should end when the last cardWaitTimeout
         * occurs
         */
        virtual void end();

        /**
         *
         */
        virtual void* run();

        /**
         *
         */
        virtual ~EventThread()
        {
            this->end();
        }

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
        AbstractThreadedObservablePlugin* mParent;

        /**
         *
         */
        const std::string mPluginName;

        /**
         *
         */
        bool mRunning = true;
    };

    /**
     *
     */
    const std::shared_ptr<Logger> mLogger =
        LoggerFactory::getLogger(typeid(AbstractThreadedObservablePlugin));

    /**
     * Local thread to monitoring readers presence
     */
    std::shared_ptr<EventThread> mThread;

    /**
     * List of names of the physical (native) connected readers This list helps
     * synchronizing physical readers managed by third-party library such as
     * smardcard.io and the list of keyple {@link keyple::seproxy::SeReader}
     * Insertion, removal, and access operations safely execute concurrently by
     * multiple threads.
     */
    std::set<std::string> mNativeReadersNames;
};

}
}
}
}
