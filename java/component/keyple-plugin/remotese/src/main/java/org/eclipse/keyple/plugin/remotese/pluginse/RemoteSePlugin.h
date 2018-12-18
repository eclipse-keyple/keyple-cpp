#pragma once

#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/plugin/AbstractObservablePlugin.h"
#include <string>
#include <unordered_map>
#include <set>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace pluginse { class VirtualReaderSessionFactory; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class DtoSender; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeReader; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderNotFoundException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ProxyReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace pluginse { class VirtualReader; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ReaderEvent; } } } } }
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
                namespace remotese {
                    namespace pluginse {

                        using DtoSender = org::eclipse::keyple::plugin::remotese::transport::DtoSender;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                        using AbstractObservablePlugin = org::eclipse::keyple::seproxy::plugin::AbstractObservablePlugin;
                        using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;


                        /**
                         * Remote SE Plugin Creates a virtual reader when a remote readers connect Manages the dispatch of
                         * events received from remote readers
                         */
                        class RemoteSePlugin final : public AbstractObservablePlugin {

                        private:
                            static const std::shared_ptr<Logger> logger;
                        public:
                            static const std::string PLUGIN_NAME;

                            // private final VirtualReaderSessionFactory sessionManager;

                        private:
                            const std::shared_ptr<VirtualReaderSessionFactory> sessionManager;
                            const std::shared_ptr<DtoSender> sender;

                            /**
                             * Only {@link VirtualReaderService} can instanciate a RemoteSePlugin
                             */
                        public:
                            RemoteSePlugin(std::shared_ptr<VirtualReaderSessionFactory> sessionManager, std::shared_ptr<DtoSender> sender);

                            std::unordered_map<std::string, std::string> getParameters() override;

                            void setParameter(const std::string &key, const std::string &value) throw(std::invalid_argument) override;



                            std::shared_ptr<SeReader> getReaderByRemoteName(const std::string &remoteName) throw(KeypleReaderNotFoundException);

                            /**
                             * Create a virtual reader
                             *
                             */
                            std::shared_ptr<ProxyReader> createVirtualReader(const std::string &clientNodeId, const std::string &nativeReaderName, std::shared_ptr<DtoSender> dtoSender) throw(KeypleReaderException);

                        private:
                            class ThreadAnonymousInnerClass : public Thread {
                            private:
                                std::shared_ptr<RemoteSePlugin> outerInstance;

                                std::shared_ptr<org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader> virtualReader;

                            public:
                                ThreadAnonymousInnerClass(std::shared_ptr<RemoteSePlugin> outerInstance, std::shared_ptr<org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader> virtualReader);

                                void run();

protected:
                                std::shared_ptr<ThreadAnonymousInnerClass> shared_from_this() {
                                    return std::static_pointer_cast<ThreadAnonymousInnerClass>(Thread::shared_from_this());
                                }
                            };

                            /**
                             * Delete a virtual reader
                             * 
                             * @param nativeReaderName name of the virtual reader to be deleted
                             */
                        public:
                            void disconnectRemoteReader(const std::string &nativeReaderName) throw(KeypleReaderNotFoundException);

                        private:
                            class ThreadAnonymousInnerClass2 : public Thread {
                            private:
                                std::shared_ptr<RemoteSePlugin> outerInstance;

                                std::shared_ptr<org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader> virtualReader;

                            public:
                                ThreadAnonymousInnerClass2(std::shared_ptr<RemoteSePlugin> outerInstance, std::shared_ptr<org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader> virtualReader);

                                void run();

protected:
                                std::shared_ptr<ThreadAnonymousInnerClass2> shared_from_this() {
                                    return std::static_pointer_cast<ThreadAnonymousInnerClass2>(Thread::shared_from_this());
                                }
                            };

                            /**
                             * Propagate a received event from slave device
                             * 
                             * @param event
                             * @param sessionId : not used yet
                             */
                        public:
                            void onReaderEvent(std::shared_ptr<ReaderEvent> event_Renamed, const std::string &sessionId);


                        private:
                            Boolean isReaderConnected(const std::string &name);

                        protected:
                            std::shared_ptr<SortedSet<std::shared_ptr<AbstractObservableReader>>> getNativeReaders() throw(KeypleReaderException) override;

                            std::shared_ptr<AbstractObservableReader> getNativeReader(const std::string &s) throw(KeypleReaderException) override;

protected:
                            std::shared_ptr<RemoteSePlugin> shared_from_this() {
                                return std::static_pointer_cast<RemoteSePlugin>(org.eclipse.keyple.seproxy.plugin.AbstractObservablePlugin::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
