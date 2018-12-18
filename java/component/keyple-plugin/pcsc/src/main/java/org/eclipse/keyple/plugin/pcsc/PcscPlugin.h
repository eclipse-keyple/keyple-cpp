#pragma once

#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractThreadedObservablePlugin.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleBaseException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace plugin { class AbstractObservableReader; } } } } }

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
            namespace plugin {
                namespace pcsc {

                    using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                    using AbstractThreadedObservablePlugin = org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;

                    class PcscPlugin final : public AbstractThreadedObservablePlugin {

                    private:
                        static const std::shared_ptr<Logger> logger;

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

                        void setParameter(const std::string &key, const std::string &value) throw(std::invalid_argument, KeypleBaseException) override;

                        /**
                         * Enable the logging
                         *
                         * @param logging If logging is enabled
                         * @return Same instance (fluent setter)
                         * @deprecated
                         */
                        std::shared_ptr<PcscPlugin> setLogging(bool logging);

                    protected:
                        std::shared_ptr<SortedSet<std::string>> getNativeReadersNames() throw(KeypleReaderException) override;

                        /**
                         * Gets the list of all native readers
                         *
                         * New reader objects are created.
                         *
                         * @return the list of new readers.
                         * @throws KeypleReaderException if a reader error occurs
                         */
                        std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> getNativeReaders() throw(KeypleReaderException) override;

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
                        std::shared_ptr<AbstractObservableReader> getNativeReader(const std::string &name) throw(KeypleReaderException) override;

                    private:
                        std::shared_ptr<CardTerminals> getCardTerminals();

protected:
                        std::shared_ptr<PcscPlugin> shared_from_this() {
                            return std::static_pointer_cast<PcscPlugin>(org.eclipse.keyple.seproxy.plugin.AbstractThreadedObservablePlugin::shared_from_this());
                        }
                    };

                }
            }
        }
    }
}
