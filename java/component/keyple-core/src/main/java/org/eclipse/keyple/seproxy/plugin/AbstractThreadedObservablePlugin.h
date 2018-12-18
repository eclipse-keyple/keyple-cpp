#pragma once

#include "../event/ObservablePlugin.h"
#include "AbstractObservablePlugin.h"
#include <string>
#include <stdexcept>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace plugin { class EventThread; } } } } }

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
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {

                    using ObservablePlugin = org::eclipse::keyple::seproxy::event_Renamed::ObservablePlugin;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;

                    class AbstractThreadedObservablePlugin : public AbstractObservablePlugin, public ObservablePlugin {

                    private:
                        static const std::shared_ptr<Logger> logger;

                        static constexpr long long SETTING_THREAD_TIMEOUT_DEFAULT = 1000;

                        /**
                         * Local thread to monitoring readers presence
                         */
                        std::shared_ptr<EventThread> thread;

                        /**
                         * Thread wait timeout in ms
                         *
                         * This timeout value will determined the latency to detect changes
                         */
                    protected:
                        long long threadWaitTimeout = SETTING_THREAD_TIMEOUT_DEFAULT;

                        /**
                         * List of names of the connected readers
                         */
                    private:
                        static std::shared_ptr<SortedSet<std::string>> nativeReadersNames;

                        /**
                         * Returns the list of names of all connected readers
                         *
                         * @return readers names list
                         * @throws KeypleReaderException if a reader error occurs
                         */
                    protected:
                        virtual std::shared_ptr<SortedSet<std::string>> getNativeReadersNames() = 0;

                        /**
                         * Constructor
                         *
                         * @param name name of the plugin
                         */
                    public:
                        AbstractThreadedObservablePlugin(const std::string &name);

                        /**
                         * Start the monitoring thread.
                         * <p>
                         * The thread is created if it does not already exist
                         */
                    protected:
                        void startObservation() override;

                        /**
                         * Terminate the monitoring thread
                         */
                        void stopObservation() override;

                        /**
                         * Thread in charge of reporting live events
                         */
                    private:
                        class EventThread : public Thread {
                                        private:
                                            std::shared_ptr<AbstractThreadedObservablePlugin> outerInstance;

                            const std::string pluginName;
                            bool running = true;

                            EventThread(std::shared_ptr<AbstractThreadedObservablePlugin> outerInstance, const std::string &pluginName);

                            /**
                             * Marks the thread as one that should end when the last cardWaitTimeout occurs
                             */
                        public:
                            virtual void end();

                            virtual void run();

protected:
                            std::shared_ptr<EventThread> shared_from_this() {
                                return std::static_pointer_cast<EventThread>(Thread::shared_from_this());
                            }
                        };

                        /**
                         * Called when the class is unloaded. Attempt to do a clean exit.
                         *
                         * @throws Throwable a generic exception
                         */
                    protected:
//JAVA TO C++ CONVERTER WARNING: Unlike Java, there is no automatic call to this finalizer method in native C++:
                        void finalize() throw(std::runtime_error) override;

protected:
                        std::shared_ptr<AbstractThreadedObservablePlugin> shared_from_this() {
                            return std::static_pointer_cast<AbstractThreadedObservablePlugin>(AbstractObservablePlugin::shared_from_this());
                        }
                    };

                }
            }
        }
    }
}
