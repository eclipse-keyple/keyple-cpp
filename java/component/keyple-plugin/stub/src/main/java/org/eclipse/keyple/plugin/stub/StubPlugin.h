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

/* Forward class declarations */
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace exception {
                    class KeypleReaderException;
                }
            } // namespace seproxy
        }     // namespace keyple
    }         // namespace eclipse
} // namespace org
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace plugin {
                    class AbstractObservableReader;
                }
            } // namespace seproxy
        }     // namespace keyple
    }         // namespace eclipse
} // namespace org

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

                    class EXPORT StubPlugin : public AbstractThreadedObservablePlugin {

                      private:
                        /**
                         *
                         */
                        const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(StubPlugin));

                        /**
                         *
                         */
                        std::unordered_map<std::string, std::string> parameters =
                            std::unordered_map<std::string, std::string>();

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
                        static std::shared_ptr<StubPlugin> getInstance();

                        /**
                         *
                         */
                        std::unordered_map<std::string, std::string> getParameters() override;

                        /**
                         *
                         */
                        void setParameter(const std::string &key, const std::string &value) override;

                      protected:
                        /**
                         *
                         */
                        std::shared_ptr<std::set<std::shared_ptr<SeReader>>>
                        initNativeReaders() throw(KeypleReaderException) override;

                        /**
                         *
                         */
                        std::shared_ptr<SeReader> getNativeReader(const std::string &name) override;

                        /**
                         * Plug a Stub Reader
                         * 
                         * @param name : name of the reader
                         */
                      public:
                        void plugStubReader(const std::string &name);

                        /**
                         * Unplug a Stub Reader
                         * 
                         * @param name the name of the reader
                         * @throws KeypleReaderException in case of a reader exception
                         */
                        void unplugReader(const std::string &name) throw(KeypleReaderException);

                        /**
                         * Get a list of available reader names
                         * 
                         * @return String list
                         */
                      protected:
                        std::shared_ptr<std::set<std::string>> getNativeReadersNames() override;

                      protected:
                        /**
                         *
                         */
                        std::shared_ptr<StubPlugin> shared_from_this()
                        {
                            return std::static_pointer_cast<StubPlugin>(
                                AbstractThreadedObservablePlugin::shared_from_this());
                        }

                      public:
                        std::shared_ptr<std::set<std::shared_ptr<SeReader>>>
                        getReaders() throw(KeypleReaderException) override
                        {
                            return AbstractThreadedObservablePlugin::AbstractObservablePlugin::getReaders();
                        }

                        std::shared_ptr<SeReader> getReader(const std::string &name) override
                        {
                            return AbstractThreadedObservablePlugin::AbstractObservablePlugin::getReader(
                                name);
                        }

                        void addObserver(std::shared_ptr<PluginObserver> observer) override
                        {
                            logger->debug(" observer: %p\n", observer);

                            return AbstractThreadedObservablePlugin::AbstractObservablePlugin::addObserver(
                                observer);
                        }

                        void removeObserver(std::shared_ptr<PluginObserver> observer) override
                        {
                            logger->debug("\n");
                            return AbstractThreadedObservablePlugin::AbstractObservablePlugin::removeObserver(
                                observer);
                        }

                        void notifyObservers(std::shared_ptr<PluginEvent> event) override
                        {
                            logger->debug("\n");
                            return AbstractThreadedObservablePlugin::AbstractLoggedObservable::
                                notifyObservers(event);
                        }

                        bool equals(std::shared_ptr<void> o) override
                        {
                            return true; //AbstractThreadedObservablePlugin::equals(o);
                        }

                        int hashCode() override
                        {
                            return 0; //AbstractThreadedObservablePlugin::hashCode();
                        }

                        void setParameters(std::unordered_map<std::string, std::string> &parameters) override
                        {
                            return AbstractThreadedObservablePlugin::AbstractLoggedObservable::setParameters(
                                parameters);
                        }
                    };

                } // namespace stub
            }     // namespace plugin
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org
