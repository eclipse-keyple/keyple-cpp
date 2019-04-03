#pragma once

#include "BaseStubTest.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservablePlugin.h"
#include <string>
#include <unordered_set>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
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
            namespace plugin {
                namespace stub {


                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using namespace org::junit;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(MockitoJUnitRunner.class) @FixMethodOrder(MethodSorters.NAME_ASCENDING) public class StubPluginTest extends BaseStubTest
                    class StubPluginTest : public BaseStubTest {

                    public:
                        std::shared_ptr<Logger> logger = LoggerFactory::getLogger(StubPluginTest::typeid);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws Exception
                        void setUp() throw(std::runtime_error) override;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        void tearDown() throw(InterruptedException, KeypleReaderException) override;

                        /**
                         * Plug one reader and count if created
                         */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testA_PlugOneReaderCount() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void testA_PlugOneReaderCount() throw(InterruptedException, KeypleReaderException);


                        /**
                         * Plug one reader and wait for event
                         */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testA_PlugOneReaderEvent() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void testA_PlugOneReaderEvent() throw(InterruptedException, KeypleReaderException);

                    private:
                        class PluginObserverAnonymousInnerClass : public std::enable_shared_from_this<PluginObserverAnonymousInnerClass>, public ObservablePlugin::PluginObserver {
                        private:
                            std::shared_ptr<StubPluginTest> outerInstance;

                            std::shared_ptr<CountDownLatch> readerConnected;
                            std::string READER_NAME;

                        public:
                            PluginObserverAnonymousInnerClass(std::shared_ptr<StubPluginTest> outerInstance, std::shared_ptr<CountDownLatch> readerConnected, const std::string &READER_NAME);

                            void update(std::shared_ptr<PluginEvent> event_Renamed);
                        };

                        /**
                         * Plug one reader synchronously, an event is raised no matter what
                         * 
                         * @Test public void testA_PlugOneReaderEventSynchronous() throws InterruptedException,
                         *       KeypleReaderException { final CountDownLatch readerConnected = new CountDownLatch(1);
                         *       final String READER_NAME = "testA_PlugReaders";
                         * 
                         *       // add READER_CONNECTED assert observer stubPlugin.addObserver(new
                         *       ObservablePlugin.PluginObserver() {
                         * @Override public void update(PluginEvent event) {
                         *           Assert.assertEquals(PluginEvent.EventType.READER_CONNECTED, event.getEventType());
                         *           Assert.assertEquals(1, event.getReaderNames().size());
                         *           Assert.assertEquals(READER_NAME, event.getReaderNames().first());
                         *           readerConnected.countDown(); } });
                         * 
                         *           stubPlugin.plugStubReader(READER_NAME, true); readerConnected.await(2,
                         *           TimeUnit.SECONDS);
                         * 
                         *           //TODO event should not be raised Assert.assertEquals(1,
                         *           stubPlugin.getReaders().size()); Assert.assertEquals(1,
                         *           readerConnected.getCount()); }
                         */

                        /**
                         * Unplug one reader and count if removed
                         */
                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testA_UnplugOneReaderCount() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void testA_UnplugOneReaderCount() throw(InterruptedException, KeypleReaderException);

                        /**
                         * Unplug one reader and wait for event
                         */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testB_UnplugOneReaderEvent() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void testB_UnplugOneReaderEvent() throw(InterruptedException, KeypleReaderException);

                    private:
                        class PluginObserverAnonymousInnerClass2 : public std::enable_shared_from_this<PluginObserverAnonymousInnerClass2>, public ObservablePlugin::PluginObserver {
                        private:
                            std::shared_ptr<StubPluginTest> outerInstance;

                            std::shared_ptr<CountDownLatch> readerConnected;
                            std::shared_ptr<CountDownLatch> readerDisconnected;
                            std::string READER_NAME;

                        public:
                            PluginObserverAnonymousInnerClass2(std::shared_ptr<StubPluginTest> outerInstance, std::shared_ptr<CountDownLatch> readerConnected, std::shared_ptr<CountDownLatch> readerDisconnected, const std::string &READER_NAME);

                            int event_i = 0;

                            void update(std::shared_ptr<PluginEvent> event_Renamed);
                        };

                        /**
                         * Plug same reader twice
                         */
                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testC_PlugSameReaderTwice() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void testC_PlugSameReaderTwice() throw(InterruptedException, KeypleReaderException);

                        /**
                         * Get name
                         */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testD_GetName()
                        virtual void testD_GetName();

                        /**
                         * Plug many readers at once and count
                         */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testE_PlugMultiReadersCount() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void testE_PlugMultiReadersCount() throw(InterruptedException, KeypleReaderException);

                        /**
                         * Plug many readers at once and wait for event
                         */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testE_PlugMultiReadersEvent() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void testE_PlugMultiReadersEvent() throw(InterruptedException, KeypleReaderException);

                    private:
                        class PluginObserverAnonymousInnerClass3 : public std::enable_shared_from_this<PluginObserverAnonymousInnerClass3>, public ObservablePlugin::PluginObserver {
                        private:
                            std::shared_ptr<StubPluginTest> outerInstance;

                            std::shared_ptr<Set<std::string>> READERS;
                            std::shared_ptr<CountDownLatch> readerConnected;

                        public:
                            PluginObserverAnonymousInnerClass3(std::shared_ptr<StubPluginTest> outerInstance, std::shared_ptr<Set<std::string>> READERS, std::shared_ptr<CountDownLatch> readerConnected);

                            void update(std::shared_ptr<PluginEvent> event_Renamed);

                        };

                        /**
                         * Plug and unplug many readers at once and count
                         */
                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testF_PlugUnplugMultiReadersCount() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void testF_PlugUnplugMultiReadersCount() throw(InterruptedException, KeypleReaderException);

                        /**
                         * Plug and unplug many readers at once and wait for events
                         */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testF_PlugUnplugMultiReadersEvent() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void testF_PlugUnplugMultiReadersEvent() throw(InterruptedException, KeypleReaderException);

                    private:
                        class PluginObserverAnonymousInnerClass4 : public std::enable_shared_from_this<PluginObserverAnonymousInnerClass4>, public ObservablePlugin::PluginObserver {
                        private:
                            std::shared_ptr<StubPluginTest> outerInstance;

                            std::shared_ptr<Set<std::string>> READERS;
                            std::shared_ptr<CountDownLatch> readerConnected;
                            std::shared_ptr<CountDownLatch> readerDisconnected;

                        public:
                            PluginObserverAnonymousInnerClass4(std::shared_ptr<StubPluginTest> outerInstance, std::shared_ptr<Set<std::string>> READERS, std::shared_ptr<CountDownLatch> readerConnected, std::shared_ptr<CountDownLatch> readerDisconnected);

                            int event_i = 0;

                            void update(std::shared_ptr<PluginEvent> event_Renamed);
                        };

protected:
                        std::shared_ptr<StubPluginTest> shared_from_this() {
                            return std::static_pointer_cast<StubPluginTest>(BaseStubTest::shared_from_this());
                        }
                    };

                }
            }
        }
    }
}
