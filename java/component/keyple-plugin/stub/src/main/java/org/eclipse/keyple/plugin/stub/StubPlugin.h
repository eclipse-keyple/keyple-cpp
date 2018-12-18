#pragma once

#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractThreadedObservablePlugin.h"
#include <string>
#include <unordered_map>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
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
                namespace stub {

                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                    using AbstractThreadedObservablePlugin = org::eclipse::keyple::seproxy::plugin::AbstractThreadedObservablePlugin;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;

                    class StubPlugin final : public AbstractThreadedObservablePlugin {

                    private:
                        static const std::shared_ptr<StubPlugin> uniqueInstance;

                        static const std::shared_ptr<Logger> logger;

                        const std::unordered_map<std::string, std::string> parameters = std::unordered_map<std::string, std::string>();

                        static std::shared_ptr<SortedSet<std::string>> nativeStubReadersNames;

                        StubPlugin();

                        /**
                         * Gets the single instance of StubPlugin.
                         *
                         * @return single instance of StubPlugin
                         */
                    public:
                        static std::shared_ptr<StubPlugin> getInstance();

                        std::unordered_map<std::string, std::string> getParameters() override;

                        void setParameter(const std::string &key, const std::string &value) override;

                    protected:
                        std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> getNativeReaders() throw(KeypleReaderException) override;

                        std::shared_ptr<AbstractObservableReader> getNativeReader(const std::string &name) override;

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
                        std::shared_ptr<SortedSet<std::string>> getNativeReadersNames() override;

protected:
                        std::shared_ptr<StubPlugin> shared_from_this() {
                            return std::static_pointer_cast<StubPlugin>(org.eclipse.keyple.seproxy.plugin.AbstractThreadedObservablePlugin::shared_from_this());
                        }
                    };

                }
            }
        }
    }
}
