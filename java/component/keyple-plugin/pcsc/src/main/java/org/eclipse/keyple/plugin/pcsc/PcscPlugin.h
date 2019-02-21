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

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "exceptionhelper.h"

/* Common */
#include "Export.h"
#include "Logger.h"

/* Core */
#include "AbstractThreadedObservablePlugin.h"
#include "PluginEvent.h"
#include "SeReader.h"

/* Smartcard I/O */
#include "CardTerminal.h"
#include "CardTerminals.h"
#include "TerminalFactory.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace exception {
                    class KeypleBaseException;
                }
            } // namespace seproxy
        }     // namespace keyple
    }         // namespace eclipse
} // namespace org
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
                namespace pcsc {

                    using SeReader            = org::eclipse::keyple::seproxy::SeReader;
                    using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                    using KeypleReaderException =
                        org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using AbstractObservableReader =
                        org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                    using AbstractThreadedObservablePlugin =
                        org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin;
                    using PluginEvent = org::eclipse::keyple::seproxy::event::PluginEvent;

                    class EXPORT PcscPlugin : public AbstractThreadedObservablePlugin {

                      private:
                        const std::shared_ptr<Logger> logger;

                        static constexpr long long SETTING_THREAD_TIMEOUT_DEFAULT = 1000;

                        /**
                         * singleton instance of SeProxyService
                         */
                        static const std::shared_ptr<PcscPlugin> uniqueInstance;

                        static std::shared_ptr<TerminalFactory> factory;

                        bool logging = false;

                        PcscPlugin();

                        /**
                         * Gets the single instance of PcscPlugin.
                         *
                         * @return single instance of PcscPlugin
                         */
                      public:
                        static std::shared_ptr<PcscPlugin> getInstance();

                        std::unordered_map<std::string, std::string> getParameters() override;

                        void setParameter(const std::string &key,
                                          const std::string &value) throw(std::invalid_argument,
                                                                          KeypleBaseException) override;

                        /**
                         * Enable the logging
                         *
                         * @param logging If logging is enabled
                         * @return Same instance (fluent setter)
                         * @deprecated
                         */
                        std::shared_ptr<PcscPlugin> setLogging(bool logging);

                      protected:
                        std::shared_ptr<std::set<std::string>>
                        getNativeReadersNames() throw(KeypleReaderException) override;

                        /**
                         * Fetch connected native readers (from smartcard.io) and returns a list of corresponding
                         * {@link org.eclipse.keyple.seproxy.plugin.AbstractObservableReader}
                         * {@link org.eclipse.keyple.seproxy.plugin.AbstractObservableReader} are new instances.
                         *
                         * @return the list of AbstractObservableReader objects.
                         * @throws KeypleReaderException if a reader error occurs
                         */
                        std::shared_ptr<std::set<std::shared_ptr<SeReader>>>
                        initNativeReaders() throw(KeypleReaderException) override;

                        /**
                         * Gets the reader whose name is provided as an argument.
                         *
                         * Returns the current reader if it is already listed.
                         *
                         * Creates and returns a new reader if not.
                         *
                         * Throws an exception if the wanted reader is not found.
                         *
                         * @param name name of the reader
                         * @return the reader object
                         * @throws KeypleReaderException if a reader error occurs
                         */
                        std::shared_ptr<SeReader>
                        getNativeReader(const std::string &name) throw(KeypleReaderException) override;

                      private:
                        /**
                         *
                         */
                        std::shared_ptr<CardTerminals> getCardTerminals();

                      protected:
                        std::shared_ptr<PcscPlugin> shared_from_this()
                        {
                            return std::static_pointer_cast<PcscPlugin>(
                                AbstractThreadedObservablePlugin::shared_from_this());
                        }

                      public:
                        void setParameters(std::unordered_map<std::string, std::string> &parameters) override
                        {
                            return AbstractThreadedObservablePlugin::AbstractLoggedObservable::setParameters(
                                parameters);
                        }

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
                            logger->debug("[PcscPlugin::addObserver] observer: %p\n", observer);

                            return AbstractThreadedObservablePlugin::AbstractObservablePlugin::addObserver(
                                observer);
                        }

                        void removeObserver(std::shared_ptr<PluginObserver> observer) override
                        {
                            logger->debug("[PcscPlugin::removeObserver]\n");
                            return AbstractThreadedObservablePlugin::AbstractObservablePlugin::removeObserver(
                                observer);
                        }

                        void notifyObservers(std::shared_ptr<PluginEvent> event) override
                        {
                            logger->debug("[PcscPlugin::notifyObservers]\n");
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

                        /**
                         *
                         */
                        std::set<std::string> nativeReadersNames;
                    };

                } // namespace pcsc
            }     // namespace plugin
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org
