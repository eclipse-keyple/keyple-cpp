#include "StubPluginTest.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/PluginEvent.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace stub {
                    using ObservablePlugin = org::eclipse::keyple::seproxy::event_Renamed::ObservablePlugin;
                    using PluginEvent = org::eclipse::keyple::seproxy::event_Renamed::PluginEvent;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using namespace org::junit;
                    using org::junit::runner::RunWith;
                    using org::junit::runners::MethodSorters;
                    using org::mockito::junit::MockitoJUnitRunner;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws Exception
                    void StubPluginTest::setUp() throw(std::runtime_error) {
                        BaseStubTest::setUp();
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::tearDown() throw(InterruptedException, KeypleReaderException) {
                        BaseStubTest::tearDown();
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testA_PlugOneReaderCount() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testA_PlugOneReaderCount() throw(InterruptedException, KeypleReaderException) {
                        const std::string READER_NAME = "testA_PlugOneReaderCount";

                        // connect reader
                        stubPlugin->plugStubReader(READER_NAME, true);
                        Assert::assertEquals(1, stubPlugin->getReaders()->size());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testA_PlugOneReaderEvent() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testA_PlugOneReaderEvent() throw(InterruptedException, KeypleReaderException) {
                        std::shared_ptr<CountDownLatch> * const readerConnected = std::make_shared<CountDownLatch>(1);
                        const std::string READER_NAME = "testA_PlugReaders";

                        // add READER_CONNECTED assert observer
                        stubPlugin->addObserver(std::make_shared<PluginObserverAnonymousInnerClass>(shared_from_this(), readerConnected, READER_NAME));

                        stubPlugin->plugStubReader(READER_NAME, false);
                        readerConnected->await(2, TimeUnit::SECONDS);
                        Assert::assertEquals(0, readerConnected->getCount());
                    }

                    StubPluginTest::PluginObserverAnonymousInnerClass::PluginObserverAnonymousInnerClass(std::shared_ptr<StubPluginTest> outerInstance, std::shared_ptr<CountDownLatch> readerConnected, const std::string &READER_NAME) {
                        this->outerInstance = outerInstance;
                        this->readerConnected = readerConnected;
                        this->READER_NAME = READER_NAME;
                    }

                    void StubPluginTest::PluginObserverAnonymousInnerClass::update(std::shared_ptr<PluginEvent> event_Renamed) {
                        Assert::assertEquals(PluginEvent::EventType::READER_CONNECTED, event_Renamed->getEventType());
                        Assert::assertEquals(1, event_Renamed->getReaderNames()->size());
                        Assert::assertEquals(READER_NAME, event_Renamed->getReaderNames()->first());
                        readerConnected->countDown();
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testA_UnplugOneReaderCount() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testA_UnplugOneReaderCount() throw(InterruptedException, KeypleReaderException) {
                        const std::string READER_NAME = "testA_UnplugOneReaderCount";
                        // connect reader
                        stubPlugin->plugStubReader(READER_NAME, true);
                        Assert::assertEquals(1, stubPlugin->getReaders()->size());
                        stubPlugin->unplugStubReader(READER_NAME, true);
                        Assert::assertEquals(0, stubPlugin->getReaders()->size());

                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testB_UnplugOneReaderEvent() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testB_UnplugOneReaderEvent() throw(InterruptedException, KeypleReaderException) {
                        std::shared_ptr<CountDownLatch> * const readerConnected = std::make_shared<CountDownLatch>(1);
                        std::shared_ptr<CountDownLatch> * const readerDisconnected = std::make_shared<CountDownLatch>(1);
                        const std::string READER_NAME = "testB_PlugUnplugOneReaders";

                        std::shared_ptr<ObservablePlugin::PluginObserver> disconnected_obs = std::make_shared<PluginObserverAnonymousInnerClass2>(shared_from_this(), readerConnected, readerDisconnected, READER_NAME);

                        // add READER_DISCONNECTED assert observer
                        stubPlugin->addObserver(disconnected_obs);

                        // plug a reader
                        stubPlugin->plugStubReader(READER_NAME, false);

                        readerConnected->await(2, TimeUnit::SECONDS);

                        // unplug reader
                        stubPlugin->unplugStubReader(READER_NAME, false);

                        // wait for event to be raised
                        readerDisconnected->await(2, TimeUnit::SECONDS);
                        Assert::assertEquals(0, readerDisconnected->getCount());
                    }

                    StubPluginTest::PluginObserverAnonymousInnerClass2::PluginObserverAnonymousInnerClass2(std::shared_ptr<StubPluginTest> outerInstance, std::shared_ptr<CountDownLatch> readerConnected, std::shared_ptr<CountDownLatch> readerDisconnected, const std::string &READER_NAME) {
                        this->outerInstance = outerInstance;
                        this->readerConnected = readerConnected;
                        this->readerDisconnected = readerDisconnected;
                        this->READER_NAME = READER_NAME;
                        event_i = 1;
                    }

                    void StubPluginTest::PluginObserverAnonymousInnerClass2::update(std::shared_ptr<PluginEvent> event_Renamed) {
                        outerInstance->logger->info("event {} {}", event_Renamed->getEventType(), event_Renamed->getReaderNames()->size());
                        if (event_i == 1) {
                            Assert::assertEquals(PluginEvent::EventType::READER_CONNECTED, event_Renamed->getEventType());
                            readerConnected->countDown();
                        }
                        // analyze the second event, should be a READER_DISCONNECTED
                        if (event_i == 2) {
                            Assert::assertEquals(PluginEvent::EventType::READER_DISCONNECTED, event_Renamed->getEventType());
                            Assert::assertEquals(1, event_Renamed->getReaderNames()->size());
                            Assert::assertEquals(READER_NAME, event_Renamed->getReaderNames()->first());
                            readerDisconnected->countDown();
                        }
                        event_i++;
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testC_PlugSameReaderTwice() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testC_PlugSameReaderTwice() throw(InterruptedException, KeypleReaderException) {
                        const std::string READER_NAME = "testC_PlugSameReaderTwice";

                        stubPlugin->plugStubReader(READER_NAME, true);
                        stubPlugin->plugStubReader(READER_NAME, true);
                        logger->debug("Stubplugin readers size {} ", stubPlugin->getReaders()->size());

                        Assert::assertEquals(1, stubPlugin->getReaders()->size());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testD_GetName()
                    void StubPluginTest::testD_GetName() {
                        Assert::assertNotNull(stubPlugin->getName());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testE_PlugMultiReadersCount() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testE_PlugMultiReadersCount() throw(InterruptedException, KeypleReaderException) {
                        std::shared_ptr<Set<std::string>> newReaders = std::unordered_set<std::string>(Arrays::asList("EC_reader1", "EC_reader2", "EC_reader3"));
                        // connect readers at once
                        stubPlugin->plugStubReaders(newReaders, true);
                        logger->info("Stub Readers connected {}", stubPlugin->getReaderNames());
                        Assert::assertEquals(newReaders, stubPlugin->getReaderNames());
                        Assert::assertEquals(3, stubPlugin->getReaders()->size());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testE_PlugMultiReadersEvent() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testE_PlugMultiReadersEvent() throw(InterruptedException, KeypleReaderException) {
                        std::shared_ptr<Set<std::string>> * const READERS = std::unordered_set<std::string>(Arrays::asList("E_Reader1", "E_Reader2", "E_Reader3"));

                        // lock test until message is received
                        std::shared_ptr<CountDownLatch> * const readerConnected = std::make_shared<CountDownLatch>(1);

                        // add READER_CONNECTED assert observer
                        stubPlugin->addObserver(std::make_shared<PluginObserverAnonymousInnerClass3>(shared_from_this(), READERS, readerConnected));


                        // connect readers
                        stubPlugin->plugStubReaders(READERS, false);

                        // wait for event to be raised
                        readerConnected->await(2, TimeUnit::SECONDS);
                        Assert::assertEquals(0, readerConnected->getCount());

                    }

                    StubPluginTest::PluginObserverAnonymousInnerClass3::PluginObserverAnonymousInnerClass3(std::shared_ptr<StubPluginTest> outerInstance, std::shared_ptr<Set<std::string>> READERS, std::shared_ptr<CountDownLatch> readerConnected) {
                        this->outerInstance = outerInstance;
                        this->READERS = READERS;
                        this->readerConnected = readerConnected;
                    }

                    void StubPluginTest::PluginObserverAnonymousInnerClass3::update(std::shared_ptr<PluginEvent> event_Renamed) {
                        outerInstance->logger->info("event {} {}", event_Renamed->getEventType(), event_Renamed->getReaderNames()->size());
                        Assert::assertEquals(PluginEvent::EventType::READER_CONNECTED, event_Renamed->getEventType());
                        Assert::assertEquals(3, event_Renamed->getReaderNames()->size());
                        Assert::assertEquals(READERS, event_Renamed->getReaderNames());
                        readerConnected->countDown();
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testF_PlugUnplugMultiReadersCount() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testF_PlugUnplugMultiReadersCount() throw(InterruptedException, KeypleReaderException) {
                        std::shared_ptr<Set<std::string>> * const READERS = std::unordered_set<std::string>(Arrays::asList("FC_Reader1", "FC_Reader2", "FC_Reader3"));
                        // connect readers at once
                        stubPlugin->plugStubReaders(READERS, true);
                        Assert::assertEquals(3, stubPlugin->getReaders()->size());
                        stubPlugin->unplugStubReaders(READERS, true);
                        Assert::assertEquals(0, stubPlugin->getReaders()->size());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testF_PlugUnplugMultiReadersEvent() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testF_PlugUnplugMultiReadersEvent() throw(InterruptedException, KeypleReaderException) {
                        std::shared_ptr<Set<std::string>> * const READERS = std::unordered_set<std::string>(Arrays::asList("F_Reader1", "F_Reader2", "F_Reader3"));

                        // lock test until message is received
                        std::shared_ptr<CountDownLatch> * const readerConnected = std::make_shared<CountDownLatch>(1);
                        std::shared_ptr<CountDownLatch> * const readerDisconnected = std::make_shared<CountDownLatch>(1);

                        std::shared_ptr<ObservablePlugin::PluginObserver> assertDisconnect = std::make_shared<PluginObserverAnonymousInnerClass4>(shared_from_this(), READERS, readerConnected, readerDisconnected);
                        // add assert DISCONNECT assert observer
                        stubPlugin->addObserver(assertDisconnect);

                        // connect reader
                        stubPlugin->plugStubReaders(READERS, false);

                        Assert::assertTrue(readerConnected->await(5, TimeUnit::SECONDS));

                        stubPlugin->unplugStubReaders(READERS, false);

                        Assert::assertTrue(readerDisconnected->await(5, TimeUnit::SECONDS));

                        delay(1000); // Todo fix me, should works without sleep
                        logger->debug("Stub Readers connected {}", stubPlugin->getReaderNames());
                        Assert::assertEquals(0, stubPlugin->getReaders()->size());
                        Assert::assertEquals(0, readerConnected->getCount());
                        Assert::assertEquals(0, readerDisconnected->getCount());
                    }

                    StubPluginTest::PluginObserverAnonymousInnerClass4::PluginObserverAnonymousInnerClass4(std::shared_ptr<StubPluginTest> outerInstance, std::shared_ptr<Set<std::string>> READERS, std::shared_ptr<CountDownLatch> readerConnected, std::shared_ptr<CountDownLatch> readerDisconnected) {
                        this->outerInstance = outerInstance;
                        this->READERS = READERS;
                        this->readerConnected = readerConnected;
                        this->readerDisconnected = readerDisconnected;
                        event_i = 1;
                    }

                    void StubPluginTest::PluginObserverAnonymousInnerClass4::update(std::shared_ptr<PluginEvent> event_Renamed) {
                        outerInstance->logger->info("event {} {}", event_Renamed->getEventType(), event_Renamed->getReaderNames()->size());
                        if (event_i == 1) {
                            Assert::assertEquals(PluginEvent::EventType::READER_CONNECTED, event_Renamed->getEventType());
                            readerConnected->countDown();
                        }
                        // analyze the second event, should be a READER_DISCONNECTED
                        if (event_i == 2) {
                            Assert::assertEquals(PluginEvent::EventType::READER_DISCONNECTED, event_Renamed->getEventType());
                            Assert::assertEquals(3, event_Renamed->getReaderNames()->size());
                            Assert::assertEquals(READERS, event_Renamed->getReaderNames());
                            readerDisconnected->countDown();
                        }
                        event_i++;
                    }
                }
            }
        }
    }
}
