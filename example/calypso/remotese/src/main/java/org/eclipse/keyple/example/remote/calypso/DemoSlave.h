#pragma once

#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservablePlugin.h"
#include <string>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace stub { class StubReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace nativese { class NativeReaderServiceImpl; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class PluginEvent; } } } } }

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
            namespace example {
                namespace remote {
                    namespace calypso {

                        using NativeReaderServiceImpl = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderServiceImpl;
                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using StubReader = org::eclipse::keyple::plugin::stub::StubReader;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * DemoSlave is where slave readers are physically located It connects one native reader to the
                         * master to delegate control of it
                         */
                        class DemoSlave : public std::enable_shared_from_this<DemoSlave> {

                        private:
                            static const std::shared_ptr<Logger> logger;

                            // physical reader, in this case a StubReader
                            std::shared_ptr<StubReader> localReader;

                            // TransportNode used as to send and receive KeypleDto to Master
                            std::shared_ptr<TransportNode> node;

                            // NativeReaderServiceImpl, used to connectAReader and disconnect readers
                            std::shared_ptr<NativeReaderServiceImpl> nativeReaderService;

                            // Client NodeId used to identify this terminal
                            const std::string nodeId = "node1";


                            /**
                             * At startup, create the {@link TransportNode} object, either a {@link ClientNode} or a
                             * {@link ServerNode}
                             * 
                             * @param transportFactory : factory to get the type of transport needed (websocket,
                             *        webservice...)
                             * @param isServer : true if a Server is wanted
                             */
                        public:
                            DemoSlave(std::shared_ptr<TransportFactory> transportFactory, Boolean isServer);

                        private:
                            class ThreadAnonymousInnerClass : public Thread {
                            private:
                                std::shared_ptr<DemoSlave> outerInstance;

                            public:
                                ThreadAnonymousInnerClass(std::shared_ptr<DemoSlave> outerInstance);

                                void run() override;

protected:
                                std::shared_ptr<ThreadAnonymousInnerClass> shared_from_this() {
                                    return std::static_pointer_cast<ThreadAnonymousInnerClass>(Thread::shared_from_this());
                                }
                            };

                            /**
                             * Creates a {@link StubReader} and connects it to the Master terminal via the
                             * {@link org.eclipse.keyple.plugin.remotese.nativese.NativeReaderService}
                             * 
                             * @throws KeypleReaderException
                             * @throws InterruptedException
                             */
                        public:
                            virtual void connectAReader() throw(KeypleReaderException, InterruptedException, KeypleRemoteException);

                        private:
                            class PluginObserverAnonymousInnerClass : public std::enable_shared_from_this<PluginObserverAnonymousInnerClass>, public ObservablePlugin::PluginObserver {
                            private:
                                std::shared_ptr<DemoSlave> outerInstance;

                            public:
                                PluginObserverAnonymousInnerClass(std::shared_ptr<DemoSlave> outerInstance);

                                void update(std::shared_ptr<PluginEvent> event_Renamed);
                            };

                        public:
                            virtual void insertSe();

                            virtual void removeSe();

                            virtual void disconnect() throw(KeypleReaderException, KeypleRemoteException);



                        };

                    }
                }
            }
        }
    }
}
