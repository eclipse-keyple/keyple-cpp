#pragma once

#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
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
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        class DemoThreads : public std::enable_shared_from_this<DemoThreads> {

                        private:
                            static const std::shared_ptr<Logger> logger;


                        public:
                            static void startServer(Boolean const isMaster, std::shared_ptr<TransportFactory> factory);

                        private:
                            class ThreadAnonymousInnerClass : public Thread {
                            private:
                                Boolean isMaster;
                                std::shared_ptr<TransportFactory> factory;

                            public:
                                ThreadAnonymousInnerClass(Boolean isMaster, std::shared_ptr<TransportFactory> factory);

                                void run() override;

protected:
                                std::shared_ptr<ThreadAnonymousInnerClass> shared_from_this() {
                                    return std::static_pointer_cast<ThreadAnonymousInnerClass>(Thread::shared_from_this());
                                }
                            };

                        public:
                            static void startClient(Boolean const isMaster, std::shared_ptr<TransportFactory> factory);

                        private:
                            class ThreadAnonymousInnerClass2 : public Thread {
                            private:
                                Boolean isMaster;
                                std::shared_ptr<TransportFactory> factory;

                            public:
                                ThreadAnonymousInnerClass2(Boolean isMaster, std::shared_ptr<TransportFactory> factory);

                                void run() override;

protected:
                                std::shared_ptr<ThreadAnonymousInnerClass2> shared_from_this() {
                                    return std::static_pointer_cast<ThreadAnonymousInnerClass2>(Thread::shared_from_this());
                                }
                            };


                        };

                    }
                }
            }
        }
    }
}
