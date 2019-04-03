#include "VirtualPluginEventTest.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/pluginse/RemoteSePlugin.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/PluginEvent.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace integration {
                        using RemoteSePlugin = org::eclipse::keyple::plugin::remotese::pluginse::RemoteSePlugin;
                        using PluginEvent = org::eclipse::keyple::seproxy::event_Renamed::PluginEvent;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using Observable = org::eclipse::keyple::util::Observable;
                        using org::junit::After;
                        using org::junit::Assert;
                        using org::junit::Before;
                        using org::junit::Test;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> VirtualPluginEventTest::logger = org::slf4j::LoggerFactory::getLogger(VirtualPluginEventTest::typeid);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws Exception
                        void VirtualPluginEventTest::setUp() throw(std::runtime_error) {
                            // restore plugin state
                            clearStubpluginReaders();

                            initKeypleServices();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws Exception
                        void VirtualPluginEventTest::tearDown() throw(std::runtime_error) {
                            clearStubpluginReaders();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testConnectOneReader() throws Exception
                        void VirtualPluginEventTest::testConnectOneReader() throw(std::runtime_error) {
                            // lock test until message is received
                            std::shared_ptr<CountDownLatch> * const lock = std::make_shared<CountDownLatch>(1);

                            std::shared_ptr<RemoteSePlugin> * const remoteSePlugin = virtualReaderService->getPlugin();

                            remoteSePlugin->addObserver(std::make_shared<ObserverAnonymousInnerClass>(shared_from_this(), lock, remoteSePlugin));

                            // connect a Stub Native reader
                            nativeReader = this->connectStubReader(NATIVE_READER_NAME, CLIENT_NODE_ID);

                            // wait 5 seconds
                            lock->await(5, TimeUnit::SECONDS);

                            Assert::assertEquals(0, lock->getCount());
                        }

                        VirtualPluginEventTest::ObserverAnonymousInnerClass::ObserverAnonymousInnerClass(std::shared_ptr<VirtualPluginEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock, std::shared_ptr<RemoteSePlugin> remoteSePlugin) {
                            this->outerInstance = outerInstance;
                            this->lock = lock;
                            this->remoteSePlugin = remoteSePlugin;
                        }

                        void VirtualPluginEventTest::ObserverAnonymousInnerClass::update(std::shared_ptr<PluginEvent> event_Renamed) {
                            Assert::assertNotNull(event_Renamed->getReaderNames()->first());
                            Assert::assertEquals(1, event_Renamed->getReaderNames()->size());
                            Assert::assertEquals(remoteSePlugin->getName(), event_Renamed->getPluginName());
                            Assert::assertEquals(PluginEvent::EventType::READER_CONNECTED, event_Renamed->getEventType());
                            lock->countDown();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testDisconnectEvent() throws Exception
                        void VirtualPluginEventTest::testDisconnectEvent() throw(std::runtime_error) {
                            // lock test until two messages are received
                            std::shared_ptr<CountDownLatch> * const lock = std::make_shared<CountDownLatch>(2);

                            std::shared_ptr<RemoteSePlugin> * const remoteSePlugin = virtualReaderService->getPlugin();

                            remoteSePlugin->addObserver(std::make_shared<ObserverAnonymousInnerClass2>(shared_from_this(), lock, remoteSePlugin));

                            // connect a Stub Native reader
                            nativeReader = this->connectStubReader(NATIVE_READER_NAME, CLIENT_NODE_ID);

                            // wait 1 second
                            delay(1000);

                            this->disconnectStubReader("anysession", NATIVE_READER_NAME, CLIENT_NODE_ID);

                            // wait 5 seconds
                            lock->await(5, TimeUnit::SECONDS);

                            Assert::assertEquals(0, lock->getCount());
                        }

                        VirtualPluginEventTest::ObserverAnonymousInnerClass2::ObserverAnonymousInnerClass2(std::shared_ptr<VirtualPluginEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock, std::shared_ptr<RemoteSePlugin> remoteSePlugin) {
                            this->outerInstance = outerInstance;
                            this->lock = lock;
                            this->remoteSePlugin = remoteSePlugin;
                        }

                        void VirtualPluginEventTest::ObserverAnonymousInnerClass2::update(std::shared_ptr<PluginEvent> event_Renamed) {

                            // we expect the first event to be READER_CONNECTED
                            if (event_Renamed->getEventType() == PluginEvent::EventType::READER_CONNECTED) {
                                Assert::assertEquals(2, lock->getCount());
                                lock->countDown();
                            }
                            else {
                                // second event should be a READER_DISCONNECTED
                                Assert::assertNotNull(event_Renamed->getReaderNames()->first());
                                Assert::assertEquals(1, event_Renamed->getReaderNames()->size());
                                Assert::assertEquals(remoteSePlugin->getName(), event_Renamed->getPluginName());
                                Assert::assertEquals(PluginEvent::EventType::READER_DISCONNECTED, event_Renamed->getEventType());
                                lock->countDown();
                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void testConnectTwiceSameReader() throws Exception
                        void VirtualPluginEventTest::testConnectTwiceSameReader() throw(std::runtime_error) {
                            // lock test until message is received
                            std::shared_ptr<CountDownLatch> * const lock = std::make_shared<CountDownLatch>(1);

                            std::shared_ptr<RemoteSePlugin> * const remoteSePlugin = virtualReaderService->getPlugin();

                            remoteSePlugin->addObserver(std::make_shared<ObserverAnonymousInnerClass3>(shared_from_this(), lock, remoteSePlugin));

                            // connect a Stub Native reader
                            nativeReader = this->connectStubReader(NATIVE_READER_NAME, CLIENT_NODE_ID);

                            // wait 2 seconds
                            lock->await(2, TimeUnit::SECONDS);

                            // connect twice
                            nativeReader = this->connectStubReader(NATIVE_READER_NAME, CLIENT_NODE_ID);

                            // a KeypleReaderException exception is thrown
                        }

                        VirtualPluginEventTest::ObserverAnonymousInnerClass3::ObserverAnonymousInnerClass3(std::shared_ptr<VirtualPluginEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock, std::shared_ptr<RemoteSePlugin> remoteSePlugin) {
                            this->outerInstance = outerInstance;
                            this->lock = lock;
                            this->remoteSePlugin = remoteSePlugin;
                        }

                        void VirtualPluginEventTest::ObserverAnonymousInnerClass3::update(std::shared_ptr<PluginEvent> event_Renamed) {
                            // READER_CONNECTED should be raised only once, so lock.getCount() should be equals
                            // to 1
                            if (1 != lock->getCount()) {
                                throw std::make_shared<IllegalStateException>();
                            };
                            Assert::assertNotNull(event_Renamed->getReaderNames()->first());
                            Assert::assertEquals(1, event_Renamed->getReaderNames()->size());
                            Assert::assertEquals(remoteSePlugin->getName(), event_Renamed->getPluginName());
                            Assert::assertEquals(PluginEvent::EventType::READER_CONNECTED, event_Renamed->getEventType());
                            lock->countDown();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void testDisconnectUnknownReader() throws Exception
                        void VirtualPluginEventTest::testDisconnectUnknownReader() throw(std::runtime_error) {
                            std::shared_ptr<RemoteSePlugin> * const remoteSePlugin = virtualReaderService->getPlugin();

                            remoteSePlugin->addObserver(std::make_shared<ObserverAnonymousInnerClass4>(shared_from_this()));
                            this->disconnectStubReader("anysession", "A_NOT_CONNECTED_READER", CLIENT_NODE_ID);

                            // a KeypleReaderException exception is thrown
                        }

                        VirtualPluginEventTest::ObserverAnonymousInnerClass4::ObserverAnonymousInnerClass4(std::shared_ptr<VirtualPluginEventTest> outerInstance) {
                            this->outerInstance = outerInstance;
                        }

                        void VirtualPluginEventTest::ObserverAnonymousInnerClass4::update(std::shared_ptr<PluginEvent> event_Renamed) {
                            // READER_CONNECTED should not be called
                            throw std::make_shared<IllegalStateException>();
                        }
                    }
                }
            }
        }
    }
}
