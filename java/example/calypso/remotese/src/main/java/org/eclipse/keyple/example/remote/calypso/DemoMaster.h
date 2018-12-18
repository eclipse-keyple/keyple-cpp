#pragma once

#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/Observable.h"
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class TransportNode; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace transport { class TransportFactory; } } } } } }

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

                        using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;
                        using TransportNode = org::eclipse::keyple::plugin::remotese::transport::TransportNode;
                        using Observable = org::eclipse::keyple::util::Observable;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * DemoMaster is the Master Device that controls remotely native readers. A Slave Terminal delegates
                         * control of one of its native reader to the Master. In response a {@link VirtualReader} is created
                         * and accessible via {@link RemoteSePlugin} like any local reader
                         */
                        class DemoMaster : public std::enable_shared_from_this<DemoMaster>, public Observable::Observer {

                        private:
                            static const std::shared_ptr<Logger> logger;

                            // TransportNode used as to send and receive KeypleDto to Slaves
                            std::shared_ptr<TransportNode> node;

                            /**
                             * Constructor of the DemoMaster thread Starts a common node, can be server or client
                             * 
                             * @param transportFactory : type of transport used (websocket, webservice...)
                             * @param isServer : is Master the server?
                             */
                        public:
                            DemoMaster(std::shared_ptr<TransportFactory> transportFactory, Boolean isServer);

                        private:
                            class ThreadAnonymousInnerClass : public Thread {
                            private:
                                std::shared_ptr<DemoMaster> outerInstance;

                            public:
                                ThreadAnonymousInnerClass(std::shared_ptr<DemoMaster> outerInstance);

                                void run() override;


protected:
                                std::shared_ptr<ThreadAnonymousInnerClass> shared_from_this() {
                                    return std::static_pointer_cast<ThreadAnonymousInnerClass>(Thread::shared_from_this());
                                }
                            };

                            /**
                             * Initiate {@link VirtualReaderService} with both ingoing and outcoming {@link TransportNode}
                             */
                        public:
                            virtual void boot();



                            /**
                             * Process Events from {@link RemoteSePlugin} and {@link VirtualReader}
                             *
                             * @param o : can be a ReaderEvent or PluginEvent
                             */
                            void update(std::shared_ptr<void> o) override;

                        };

                    }
                }
            }
        }
    }
}
