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

#pragma once

#include <string>
#include <stdexcept>
#include <memory>

#include "exceptionhelper.h"

/* Core */
#include "ObservablePlugin.h"
#include "AbstractObservablePlugin.h"

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

class EXPORT AbstractThreadedObservablePlugin
: public AbstractObservablePlugin, public ObservablePlugin, public Object {
public:
    /**
     * Constructor
     *
     * @param name name of the plugin
     */
    AbstractThreadedObservablePlugin(const std::string &name);

protected:
    /**
     * Thread wait timeout in ms
     *
     * This timeout value will determined the latency to detect changes
     */
    long long threadWaitTimeout = SETTING_THREAD_TIMEOUT_DEFAULT;

    /**
     * Fetch the list of connected native reader (usually from third party library) and returns
     * their names (or id)
     *
     * @return connected readers' name list
     * @throws KeypleReaderException if a reader error occurs
     */
    virtual std::shared_ptr<std::set<std::string>> fetchNativeReadersNames() = 0;

    /**
     * Start the monitoring thread.
     * <p>
     * The thread is created if it does not already exist
     */
    void startObservation() override;

    /**
     * Terminate the monitoring thread
     */
    void stopObservation() override;

    /**
     * Called when the class is unloaded. Attempt to do a clean exit.
     *
     * @throws Throwable a generic exception
     */
    void finalize() override;

    /**
     *
     */
    std::shared_ptr<AbstractThreadedObservablePlugin> shared_from_this()
    {
        return std::static_pointer_cast<AbstractThreadedObservablePlugin>(
            AbstractObservablePlugin::shared_from_this());
    }

private:
    /**
     *
     */
    class EventThread : public Thread {
    public:
        /**
         * Constructor
         */

        EventThread(std::shared_ptr<AbstractThreadedObservablePlugin> outerInstance,
                    const std::string &pluginName);

        /**
         * Marks the thread as one that should end when the last cardWaitTimeout occurs
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
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(AbstractThreadedObservablePlugin));

    /**
     *
     */
    static constexpr long long SETTING_THREAD_TIMEOUT_DEFAULT = 1000;

    /**
     * Local thread to monitoring readers presence
     */
    std::shared_ptr<EventThread> thread;

    /**
     * List of names of the physical (native) connected readers This list helps synchronizing
     * physical readers managed by third-party library such as smardcard.io and the list of keyple
     * {@link org.eclipse.keyple.seproxy.SeReader} Insertion, removal, and access operations safely
     * execute concurrently by multiple threads.
     */
    std::shared_ptr<std::set<std::string>> nativeReadersNames =
        std::make_shared<std::set<std::string>>(std::set<std::string>());

};

}
}
}
}
