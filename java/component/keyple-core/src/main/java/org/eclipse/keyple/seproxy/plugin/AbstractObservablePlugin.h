#pragma once

#include <set>
#include "../ReaderPlugin.h"
#include "AbstractLoggedObservable.h"
#include <string>
#include <memory>
#include "PluginEvent.h"
#include "ObservablePlugin.h"

#include "Logger.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace event { class PluginEvent; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace plugin { class AbstractObservableReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace event { class PluginObserver; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class ReaderPlugin; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderNotFoundException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ProxyReader; } } } } }

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

                    using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                    using ObservablePlugin = org::eclipse::keyple::seproxy::event::ObservablePlugin;
                    using PluginEvent = org::eclipse::keyple::seproxy::event::PluginEvent;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                    using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;

                    /**
                     * Observable plugin. These plugin can report when a reader is added or removed.
                     */
                    class AbstractObservablePlugin : public org::eclipse::keyple::seproxy::plugin::AbstractLoggedObservable<PluginEvent>, public ReaderPlugin {

                    private:
                        const std::shared_ptr<Logger> logger;

                        /**
                         * The list of readers
                         */
                    protected:
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
                         * Returns the current readers list.
                         *
                         * The list is initialized in the constructor and may be updated in background in the case of a
                         * threaded plugin {@link AbstractThreadedObservablePlugin}
                         *
                         * @return the current reader list, can be null if the
                         */
                    public:
                        std::shared_ptr<std::set<std::shared_ptr<SeReader>>> getReaders() throw(KeypleReaderException) override;

                        /**
                         * Gets a list of native readers from the native methods
                         *
                         * @return the list of AbstractObservableReader objects.
                         * @throws KeypleReaderException if a reader error occurs
                         */
                    protected:
                        virtual std::shared_ptr<std::set<std::shared_ptr<SeReader>>> getNativeReaders()
                        {
                            return nullptr;
                        }

                        /**
                         * Gets the specific reader whose is provided as an argument.
                         *
                         * @param name the of the reader
                         * @return the AbstractObservableReader object (null if not found)
                         * @throws KeypleReaderException if a reader error occurs
                         */
                        virtual std::shared_ptr<SeReader> getNativeReader(const std::string &name) = 0;

                        /**
                         * Starts the monitoring thread
                         * <p>
                         * This method has to be overloaded by the class that handle the monitoring thread. It will be
                         * called when a first observer is added.
                         */
                        virtual void startObservation();

                        /**
                         * Ends the monitoring thread
                         * <p>
                         * This method has to be overloaded by the class that handle the monitoring thread. It will be
                         * called when the observer is removed.
                         */
                        virtual void stopObservation();

                        /**
                         * Add a plugin observer.
                         * <p>
                         * The observer will receive all the events produced by this plugin (reader insertion, removal,
                         * etc.)
                         * <p>
                         * The monitoring thread is started when the first observer is added.
                         *
                         * @param observer the observer object
                         */
                    public:
                        void addObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer);

                        /**
                         * Remove a plugin observer.
                         * <p>
                         * The observer will do not receive any of the events produced by this plugin.
                         * <p>
                         * The monitoring thread is ended when the last observer is removed.
                         *
                         * @param observer the observer object
                         */
                        void removeObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer);

                        /**
                         * Compare the name of the current ReaderPlugin to the name of the ReaderPlugin provided in
                         * argument
                         *
                         * @param plugin a {@link ReaderPlugin} object
                         * @return true if the names match (The method is needed for the SortedSet lists)
                         */
                        int compareTo(std::shared_ptr<ReaderPlugin> plugin);

                        /**
                         * Gets a specific reader designated by its name in the current readers list
                         *
                         * @param name of the reader
                         * @return the reader
                         * @throws KeypleReaderNotFoundException if the wanted reader is not found
                         *
                         * /!\ Covariant return type (prototype is SeReader but ProxyReader actually returned)
                         */
                        std::shared_ptr<SeReader> getReader(const std::string &name) override;

protected:

                        std::shared_ptr<AbstractObservablePlugin> shared_from_this() {
                            return std::static_pointer_cast<AbstractObservablePlugin>(AbstractLoggedObservable<PluginEvent>::shared_from_this());
                        }

                    };

                }
            }
        }
    }
}
