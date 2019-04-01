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
#include <atomic>

/* Common */
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "Thread.h"

/* Core */
#include "AbstractSelectionLocalReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {

                    using NoStackTraceThrowable =
                        org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                    using Logger        = org::eclipse::keyple::common::Logger;
                    using LoggerFactory = org::eclipse::keyple::common::LoggerFactory;

                        /**
                     * Abstract definition of an threader local reader. Factorizes the observation mechanism through the
                     * implementation of a monitoring thread.
                     */
                    class AbstractThreadedLocalReader : public AbstractSelectionLocalReader, public Object {

                        /**
                         * Thread in charge of reporting live events
                         */
                    private:
                        class EventThread : public Thread {
                                        private:
                                            std::shared_ptr<AbstractThreadedLocalReader> outerInstance;

                            /**
                             * Plugin name
                             */
                            const std::string pluginName;

                            /**
                             * Reader that we'll report about
                             */
                            const std::string readerName;

                            /**
                             * If the thread should be kept a alive
                             */
                            bool running = true;

                            /**
                             * Constructor
                             * 
                             * @param pluginName name of the plugin that instantiated the reader
                             * @param readerName name of the reader who owns this thread
                             */
                        public:
                            EventThread(std::shared_ptr<AbstractThreadedLocalReader> outerInstance, const std::string &pluginName, const std::string &readerName);

                            /**
                             * Marks the thread as one that should end when the last cardWaitTimeout occurs
                             */
                            virtual void end();

                            virtual void *run();

protected:
/*
                            std::shared_ptr<EventThread> shared_from_this() {
                                return std::static_pointer_cast<EventThread>(Thread::shared_from_this());
                            }
 */
                       };
                      private:
                        const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(AbstractThreadedLocalReader));
                        std::shared_ptr<EventThread> thread;
                        std::atomic<int> threadCount;
                        /**
                         * Thread wait timeout in ms
                         */
                      protected:
                        long long threadWaitTimeout = 0;

                        AbstractThreadedLocalReader(const std::string &pluginName, const std::string &readerName);

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
                         * setter to fix the wait timeout in ms.
                         *
                         * @param timeout Timeout to use
                         */
                        void setThreadWaitTimeout(long long timeout);

                        /**
                         * Waits for a card. Returns true if a card is detected before the end of the provided timeout.
                         * Returns false if no card detected within the delay.
                         *
                         * @param timeout the delay in millisecond we wait for a card insertion
                         * @return presence status
                         * @throws NoStackTraceThrowable a exception without stack trace in order to be catched and
                         *         processed silently
                         */
                        virtual bool waitForCardPresent(long long timeout) = 0;

                        /**
                         * Wait until the card disappears. Returns true if a card has disappeared before the end of the
                         * provided timeout. Returns false if the is still present within the delay. Closes the physical
                         * channel when the card has disappeared.
                         *
                         * @param timeout the delay in millisecond we wait for a card to be withdrawn
                         * @return presence status
                         * @throws NoStackTraceThrowable a exception without stack trace in order to be catched and
                         *         processed silently
                         */
                        virtual bool waitForCardAbsent(long long timeout) = 0;


                        /**
                         * Called when the class is unloaded. Attempt to do a clean exit.
                         *
                         * @throws Throwable a generic exception
                         */
                      protected:
                        //JAVA TO C++ CONVERTER WARNING: Unlike Java, there is no automatic call to this finalizer method in native C++:
                        void finalize() throw(std::runtime_error) override;

                      protected:
                        std::shared_ptr<AbstractThreadedLocalReader> shared_from_this() {
                            return std::static_pointer_cast<AbstractThreadedLocalReader>(AbstractSelectionLocalReader::shared_from_this());
                        }
                    };

                } // namespace plugin
            }     // namespace seproxy
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org
