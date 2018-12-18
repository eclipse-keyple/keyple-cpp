#pragma once

#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractStaticPlugin.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace android { namespace omapi { class ISeServiceFactory; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace plugin { class AbstractObservableReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }

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
                namespace android {
                    namespace omapi {

                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                        using AbstractStaticPlugin = org::eclipse::keyple::seproxy::plugin::AbstractStaticPlugin;
                        using org::simalliance::openmobileapi::SEService;

                        /**
                         * Loads and configures {@link AndroidOmapiReader} for each SE Reader in the platform TODO : filters
                         * readers to load by parameters with a regex
                         */
                        class AndroidOmapiPlugin final : public AbstractStaticPlugin, public SEService::CallBack {

                        private:
                            static const std::string TAG;
                        public:
                            static const std::string PLUGIN_NAME;

                        private:
                            std::shared_ptr<SEService> seService;
                            std::shared_ptr<ISeServiceFactory> seServiceFactory;


                            // singleton methods
                            static std::shared_ptr<AndroidOmapiPlugin> uniqueInstance;

                        public:
                            static std::shared_ptr<ISeServiceFactory> getSeServiceFactory();


                            /**
                             * Initialize plugin by connecting to {@link SEService} ; Make sure to instantiate Android Omapi
                             * Plugin from a Android Context Application
                             */
                            AndroidOmapiPlugin();


                            static std::shared_ptr<AndroidOmapiPlugin> getInstance();


                            /**
                             * Returns all {@link AndroidOmapiReader} readers loaded by the plugin
                             * 
                             * @return {@link AndroidOmapiReader} readers loaded by the plugin
                             * @throws KeypleReaderException
                             */
                            /*
                             * @Override public SortedSet<? extends ProxyReader> getReaders() throws KeypleReaderException {
                             * return new TreeSet<ProxyReader>(proxyReaders.values()); }
                             */

                        protected:
                            std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> getNativeReaders() override;

                            std::shared_ptr<AbstractObservableReader> getNativeReader(const std::string &name) throw(KeypleReaderException) override;

                            /**
                             * Do not call this method directly. Invoked by Open Mobile {@link SEService} when connected
                             * Instanciates {@link AndroidOmapiReader} for each SE Reader detected in the platform
                             * 
                             * @param seService
                             */
                        public:
                            void serviceConnected(std::shared_ptr<SEService> seService) override;

                        private:
                            std::unordered_map<std::string, std::string> parameters = std::unordered_map<std::string, std::string>(); // not in use in this
                            // plugin

                        public:
                            std::unordered_map<std::string, std::string> getParameters() override;

                            void setParameter(const std::string &key, const std::string &value) override;



protected:
                            std::shared_ptr<AndroidOmapiPlugin> shared_from_this() {
                                return std::static_pointer_cast<AndroidOmapiPlugin>(org.eclipse.keyple.seproxy.plugin.AbstractStaticPlugin::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
