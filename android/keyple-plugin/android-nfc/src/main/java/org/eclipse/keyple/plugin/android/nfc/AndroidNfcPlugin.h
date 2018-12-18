#pragma once

#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractStaticPlugin.h"
#include <string>
#include <unordered_map>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
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
                namespace android {
                    namespace nfc {

                        using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                        using AbstractStaticPlugin = org::eclipse::keyple::seproxy::plugin::AbstractStaticPlugin;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;



                        /**
                         * Enables Keyple to communicate with the the Android device embedded NFC reader. In the Android
                         * platform, NFC reader must be link to an application activity.
                         *
                         *
                         * To activate NFC Keyple capabilities, add {@link AndroidNfcFragment} to the application activity.
                         * getFragmentManager().beginTransaction().add(AndroidNfcFragment.newInstance(),
                         * "myFragmentId").commit();
                         *
                         * Then set the Activity as an observer of the plugin as any Keyple plugin :
                         *
                         * SeProxyService seProxyService = SeProxyService.getInstance(); List<ReaderPlugin> plugins = new
                         * ArrayList<ReaderPlugin>(); plugins.add(AndroidNfcPlugin.getInstance());
                         * seProxyService.setPlugins(plugins);
                         *
                         * ProxyReader reader = seProxyService.getPlugins().get(0).getReaders().get(0);
                         * ((AbstractObservableReader) reader).addObserver(this);
                         *
                         *
                         *
                         */

                        class AndroidNfcPlugin final : public AbstractStaticPlugin {

                        private:
                            static const std::shared_ptr<Logger> LOG;

                            static const std::shared_ptr<AndroidNfcPlugin> uniqueInstance;

                        public:
                            static const std::string PLUGIN_NAME;

                        private:
                            const std::unordered_map<std::string, std::string> parameters = std::unordered_map<std::string, std::string>(); // not in use in
                                                                                                         // this

                            // plugin

                            AndroidNfcPlugin();

                        public:
                            static std::shared_ptr<AndroidNfcPlugin> getInstance();

                            std::unordered_map<std::string, std::string> getParameters() override;

                            void setParameter(const std::string &key, const std::string &value) override;


                            /**
                             * For an Android NFC device, the Android NFC Plugin manages only one @{@link AndroidNfcReader}.
                             * 
                             * @return SortedSet<ProxyReader> : contains only one element, the
                             *         singleton @{@link AndroidNfcReader}
                             */
                        protected:
                            std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> getNativeReaders() override;


                            /**
                             * Return the AndroidNfcReader whatever is the provided name
                             * 
                             * @param name : name of the reader to retrieve
                             * @return instance of @{@link AndroidNfcReader}
                             */
                            std::shared_ptr<AbstractObservableReader> getNativeReader(const std::string &name) override;

protected:
                            std::shared_ptr<AndroidNfcPlugin> shared_from_this() {
                                return std::static_pointer_cast<AndroidNfcPlugin>(org.eclipse.keyple.seproxy.plugin.AbstractStaticPlugin::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
