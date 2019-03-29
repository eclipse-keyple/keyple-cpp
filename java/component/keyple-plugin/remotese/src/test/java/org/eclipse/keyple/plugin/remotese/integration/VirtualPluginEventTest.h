#pragma once

#include "VirtualReaderBaseTest.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/Observable.h"
#include <stdexcept>
#include "exceptionhelper.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class PluginEvent; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace pluginse { class RemoteSePlugin; } } } } } }

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
                    namespace integration {


                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * Test VirtualPlugin event READER_CONNECTED and READER_DISCONNECTED
                         */
                        class VirtualPluginEventTest : public VirtualReaderBaseTest {

                        private:
                            static const std::shared_ptr<Logger> logger;

                        public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws Exception
                            virtual void setUp() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws Exception
                            virtual void tearDown() throw(std::runtime_error);

                            /**
                             * Test READER_CONNECTED Plugin Event
                             * 
                             * @throws Exception
                             */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testConnectOneReader() throws Exception
                            virtual void testConnectOneReader() throw(std::runtime_error);

                        private:
                            class ObserverAnonymousInnerClass : public std::enable_shared_from_this<ObserverAnonymousInnerClass>, public Observable::Observer<std::shared_ptr<PluginEvent>> {
                            private:
                                std::shared_ptr<VirtualPluginEventTest> outerInstance;

                                std::shared_ptr<CountDownLatch> lock;
                                std::shared_ptr<RemoteSePlugin> remoteSePlugin;

                            public:
                                ObserverAnonymousInnerClass(std::shared_ptr<VirtualPluginEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock, std::shared_ptr<RemoteSePlugin> remoteSePlugin);

                                void update(std::shared_ptr<PluginEvent> event_Renamed) override;
                            };


                            /**
                             * Test READER_DISCONNECTED Plugin Event
                             *
                             * @throws Exception
                             */
                        public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDisconnectEvent() throws Exception
                            virtual void testDisconnectEvent() throw(std::runtime_error);

                        private:
                            class ObserverAnonymousInnerClass2 : public std::enable_shared_from_this<ObserverAnonymousInnerClass2>, public Observable::Observer<std::shared_ptr<PluginEvent>> {
                            private:
                                std::shared_ptr<VirtualPluginEventTest> outerInstance;

                                std::shared_ptr<CountDownLatch> lock;
                                std::shared_ptr<RemoteSePlugin> remoteSePlugin;

                            public:
                                ObserverAnonymousInnerClass2(std::shared_ptr<VirtualPluginEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock, std::shared_ptr<RemoteSePlugin> remoteSePlugin);

                                void update(std::shared_ptr<PluginEvent> event_Renamed) override;
                            };

                            /**
                             * Test connect reader twice
                             *
                             * @throws Exception
                             */
                        public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void testConnectTwiceSameReader() throws Exception
                            virtual void testConnectTwiceSameReader() throw(std::runtime_error);

                        private:
                            class ObserverAnonymousInnerClass3 : public std::enable_shared_from_this<ObserverAnonymousInnerClass3>, public Observable::Observer<std::shared_ptr<PluginEvent>> {
                            private:
                                std::shared_ptr<VirtualPluginEventTest> outerInstance;

                                std::shared_ptr<CountDownLatch> lock;
                                std::shared_ptr<RemoteSePlugin> remoteSePlugin;

                            public:
                                ObserverAnonymousInnerClass3(std::shared_ptr<VirtualPluginEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock, std::shared_ptr<RemoteSePlugin> remoteSePlugin);

                                void update(std::shared_ptr<PluginEvent> event_Renamed) override;
                            };

                            /**
                             * Test disconnect a not connected reader
                             * 
                             * @throws Exception
                             */
                        public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void testDisconnectUnknownReader() throws Exception
                            virtual void testDisconnectUnknownReader() throw(std::runtime_error);

                        private:
                            class ObserverAnonymousInnerClass4 : public std::enable_shared_from_this<ObserverAnonymousInnerClass4>, public Observable::Observer<std::shared_ptr<PluginEvent>> {
                            private:
                                std::shared_ptr<VirtualPluginEventTest> outerInstance;

                            public:
                                ObserverAnonymousInnerClass4(std::shared_ptr<VirtualPluginEventTest> outerInstance);

                                void update(std::shared_ptr<PluginEvent> event_Renamed) override;
                            };




protected:
                            std::shared_ptr<VirtualPluginEventTest> shared_from_this() {
                                return std::static_pointer_cast<VirtualPluginEventTest>(VirtualReaderBaseTest::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
