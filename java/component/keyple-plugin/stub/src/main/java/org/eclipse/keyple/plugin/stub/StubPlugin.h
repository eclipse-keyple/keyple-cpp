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

#include <set>
#include <string>
#include <unordered_map>
#include <memory>

/* Core */
#include "AbstractThreadedObservablePlugin.h"

/* Common */
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "InterruptedException.h"
#include "ObservablePlugin.h"
#include "KeypleBaseException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace stub {

                    using KeypleReaderException =
                        org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using AbstractObservableReader =
                        org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                    using AbstractThreadedObservablePlugin =
                        org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin;
                    using Logger        = org::eclipse::keyple::common::Logger;
                    using LoggerFactory = org::eclipse::keyple::common::LoggerFactory;
                    using SeReader      = org::eclipse::keyple::seproxy::SeReader;
                    using PluginEvent   = org::eclipse::keyple::seproxy::event::PluginEvent;
                    using ObservablePlugin = org::eclipse::keyple::seproxy::event::ObservablePlugin;
                    using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;

                    class EXPORT StubPlugin : public AbstractThreadedObservablePlugin {

                      private:
                        /**
                         *
                         */
                        const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(StubPlugin));

                        /**
                         *
                         */
                        std::unordered_map<std::string, std::string> parameters = std::unordered_map<std::string, std::string>();

                        /*
                         * simulated list of real-time connected stubReader
                         */
                        static std::shared_ptr<std::set<std::string>> connectedStubNames;

                        /**
                         *
                         */
                        static std::shared_ptr<std::set<std::string>> nativeStubReadersNames;

                        /**
                         * Constructor 
                         */
                        StubPlugin();

                        /**
                         * Gets the single instance of StubPlugin.
                         *
                         * @return single instance of StubPlugin
                         */
                      public:
                        static StubPlugin& getInstance();

                        /**
                         *
                         */
                        ~StubPlugin();

                        /**
                         *
                         */
                        std::unordered_map<std::string, std::string> getParameters() override;

                        /**
                         *
                         */
                        void setParameter(const std::string &key, const std::string &value) override;

                      public:
                        /**
                         * Plug a Stub Reader
                         *
                         * @param name : name of the created reader
                         * @param synchronous : should the stubreader added synchronously (without waiting for the
                         *        observation thread)
                         */
                        void plugStubReader(const std::string &name, bool synchronous);

                        /**
                         * Plug a list of stub Reader at once
                         * 
                         * @param names : names of readers to be connected
                         */
                        void plugStubReaders(std::shared_ptr<std::set<std::string>> names, bool synchronous);


                        /**
                         * Unplug a Stub Reader
                         * 
                         * @param name the name of the reader
                         * @throws KeypleReaderException in case of a reader exception
                         */
                        void unplugStubReader(const std::string &name, bool synchronous) throw(KeypleReaderException, InterruptedException);


                        void unplugStubReaders(std::shared_ptr<std::set<std::string>> names, bool synchronous);


                        /**
                         * Fetch the list of connected native reader (from a simulated list) and returns their names (or
                         * id)
                         * 
                         * @return connected readers' name list
                         */
                      protected:
                        std::shared_ptr<std::set<std::string>> fetchNativeReadersNames() override;

                        /**
                         * Init native Readers to empty Set
                         *
                         * @return the list of AbstractObservableReader objects.
                         * @throws KeypleReaderException if a reader error occurs
                         */
                        std::shared_ptr<std::set<std::shared_ptr<SeReader>>> initNativeReaders() throw(KeypleReaderException) override;

                        /**
                         * Fetch the reader whose name is provided as an argument. Returns the current reader if it is
                         * already listed. Creates and returns a new reader if not.
                         *
                         * Throws an exception if the wanted reader is not found.
                         *
                         * @param name name of the reader
                         * @return the reader object
                         */
                        std::shared_ptr<AbstractObservableReader> fetchNativeReader(const std::string &name) override;

protected:
                        std::shared_ptr<StubPlugin> shared_from_this() {
                            return std::static_pointer_cast<StubPlugin>(AbstractThreadedObservablePlugin::shared_from_this());
                        }

                        void removeObserver(std::shared_ptr<ObservablePlugin::PluginObserver> observer) override
                        {
                            logger->debug("\n");
                            return AbstractThreadedObservablePlugin::AbstractObservablePlugin::removeObserver(observer);
                        }

                        void notifyObservers(std::shared_ptr<PluginEvent> event) override
                        {
                            logger->debug("\n");
                            return AbstractThreadedObservablePlugin::AbstractLoggedObservable::notifyObservers(event);
                        }

                        bool equals(std::shared_ptr<void> o) override
                        {
                            return true; //AbstractThreadedObservablePlugin::equals(o);
                        }

                        int hashCode() override
                        {
                            return 0; //AbstractThreadedObservablePlugin::hashCode();
                        }

                        void setParameters(std::unordered_map<std::string, std::string> &parameters) throw(std::invalid_argument, KeypleBaseException) override
                        {
                            return AbstractThreadedObservablePlugin::AbstractLoggedObservable::setParameters(parameters);
                        }

                        std::string getName() override
                        {
                            AbstractThreadedObservablePlugin::AbstractLoggedObservable::getName();
                        }

                        void addObserver(std::shared_ptr<PluginObserver> observer)
                        {
                        }
                    };

                } // namespace stub
            }     // namespace plugin
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org
