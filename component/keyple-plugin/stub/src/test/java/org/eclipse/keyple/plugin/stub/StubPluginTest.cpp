#include "StubPluginTest.h"

#include "KeypleReaderException.h"
#include "PluginEvent.h"

        namespace keyple {
            namespace plugin {
                namespace stub {
                    using StubPlugin            = keyple::plugin::stub::StubPlugin;
                    using ObservablePlugin      = keyple::core::seproxy::event::ObservablePlugin;
                    using PluginEvent           = keyple::core::seproxy::event::PluginEvent;
                    using KeypleReaderException = keyple::core::seproxy::exception::KeypleReaderException;
                    using Logger                = keyple::common::Logger;
                    using LoggerFactory         = keyple::common::LoggerFactory;
                    //using namespace ::testing::gtest;
                    //using namespace org::junit;
                    //using org::junit::runner::RunWith;
                    //using org::junit::runners::MethodSorters;
                    //using org::mockito::junit::MockitoJUnitRunner;
                    using namespace std::chrono; // nanoseconds, system_clock, seconds
                
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws Exception
                    void StubPluginTest::setUp() 
                    {
                        BaseStubTest::setUp();
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::tearDown()
                    {
                        BaseStubTest::tearDown();
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testA_PlugOneReaderCount() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testA_PlugOneReaderCount() 
                    {
                        const std::string READER_NAME = "testA_PlugOneReaderCount";

                        // connect reader
                        stubPlugin = std::make_shared<StubPlugin>( StubPlugin::getInstance() );
                        stubPlugin->plugStubReader(READER_NAME, true);
                        ASSERT_EQ(1, stubPlugin->getReaders()->size());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testA_PlugOneReaderEvent() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testA_PlugOneReaderEvent() 
                    {
                        std::shared_ptr<CountDownLatch> readerConnected = std::make_shared<CountDownLatch>(1);
                        const std::string READER_NAME = "testA_PlugReaders";

                        // add READER_CONNECTED assert observer
                        stubPlugin->addObserver(std::make_shared<PluginObserverAnonymousInnerClass>(shared_from_this(), readerConnected, READER_NAME));

                        stubPlugin->plugStubReader(READER_NAME, false);
                        //readerConnected->await(2, SECONDS );
                        sleep( 2 );
                        ASSERT_EQ(0, readerConnected->getCount());
                    }

                    StubPluginTest::PluginObserverAnonymousInnerClass::PluginObserverAnonymousInnerClass(std::shared_ptr<StubPluginTest> outerInstance, std::shared_ptr<CountDownLatch> readerConnected, const std::string &READER_NAME) 
                    {
                        this->outerInstance = outerInstance;
                        this->readerConnected = readerConnected;
                        this->READER_NAME = READER_NAME;
                    }

                    void StubPluginTest::PluginObserverAnonymousInnerClass::update(std::shared_ptr<PluginEvent> event_Renamed) 
                    {
                        //ASSERT_EQ(PluginEvent::EventType::READER_CONNECTED, event_Renamed->getEventType());
                        ASSERT_EQ(1, event_Renamed->getReaderNames()->size());
                        std::string Var1 = READER_NAME;
                        std::string Var2 = (*event_Renamed->getReaderNames()->begin());// first());
                        ASSERT_EQ(Var1, Var2 );
                        readerConnected->countDown();
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testA_UnplugOneReaderCount() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testA_UnplugOneReaderCount() 
                    {
                        const std::string READER_NAME = "testA_UnplugOneReaderCount";
                        // connect reader
                        stubPlugin->plugStubReader(READER_NAME, true);
                        ASSERT_EQ(1, stubPlugin->getReaders()->size());
                        stubPlugin->unplugStubReader(READER_NAME, true);
                        ASSERT_EQ(0, stubPlugin->getReaders()->size());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testB_UnplugOneReaderEvent() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testB_UnplugOneReaderEvent() 
                    {
                        std::shared_ptr<CountDownLatch> readerConnected = std::make_shared<CountDownLatch>(1);
                        std::shared_ptr<CountDownLatch> readerDisconnected = std::make_shared<CountDownLatch>(1);
                        const std::string READER_NAME = "testB_PlugUnplugOneReaders";

                        std::shared_ptr<ObservablePlugin::PluginObserver> disconnected_obs = std::make_shared<PluginObserverAnonymousInnerClass2>(shared_from_this(), readerConnected, readerDisconnected, READER_NAME);

                        // add READER_DISCONNECTED assert observer
                        stubPlugin->addObserver(disconnected_obs);

                        // plug a reader
                        stubPlugin->plugStubReader(READER_NAME, false);

                        //readerConnected->await(2, TimeUnit::SECONDS);

                        // unplug reader
                        stubPlugin->unplugStubReader(READER_NAME, false);

                        // wait for event to be raised
                        //readerDisconnected->await(2, TimeUnit::SECONDS);
                        ASSERT_EQ(0, readerDisconnected->getCount());
                    }

                    StubPluginTest::PluginObserverAnonymousInnerClass2::PluginObserverAnonymousInnerClass2(std::shared_ptr<StubPluginTest> outerInstance, std::shared_ptr<CountDownLatch> readerConnected, std::shared_ptr<CountDownLatch> readerDisconnected, const std::string &READER_NAME) 
                    {
                        this->outerInstance = outerInstance;
                        this->readerConnected = readerConnected;
                        this->readerDisconnected = readerDisconnected;
                        this->READER_NAME = READER_NAME;
                        event_i = 1;
                    }

                    void StubPluginTest::PluginObserverAnonymousInnerClass2::update(std::shared_ptr<PluginEvent> event_Renamed) 
                    {
                        outerInstance->logger->info("event {} {}", event_Renamed->getEventType(), event_Renamed->getReaderNames()->size());
                        if (event_i == 1) 
                        {
                            //ASSERT_EQ(PluginEvent::EventType::READER_CONNECTED, event_Renamed->getEventType());
                            readerConnected->countDown();
                        }
                        // analyze the second event, should be a READER_DISCONNECTED
                        if (event_i == 2) 
                        {
                            //ASSERT_EQ(PluginEvent::EventType::READER_DISCONNECTED, event_Renamed->getEventType());
                            ASSERT_EQ(1, event_Renamed->getReaderNames()->size());
                            ASSERT_EQ(READER_NAME, *event_Renamed->getReaderNames()->begin());
                            readerDisconnected->countDown();
                        }
                        event_i++;
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testC_PlugSameReaderTwice() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testC_PlugSameReaderTwice() 
                    {
                        const std::string READER_NAME = "testC_PlugSameReaderTwice";

                        stubPlugin->plugStubReader(READER_NAME, true);
                        stubPlugin->plugStubReader(READER_NAME, true);
                        logger->debug("Stubplugin readers size {} ", stubPlugin->getReaders()->size());

                        ASSERT_EQ(1, stubPlugin->getReaders()->size());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testD_GetName()
                    void StubPluginTest::testD_GetName() 
                    {
                        //ASSERT_STREQ( nullptr, stubPlugin->getName() );
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testE_PlugMultiReadersCount() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testE_PlugMultiReadersCount() 
                    {
                        std::set<std::string> READERS_LIST = {"E_Reader1", "E_Reader2", "E_Reader3"};

                        std::shared_ptr<std::set<std::string>> newReaders = std::make_shared<std::set<std::string>>(READERS_LIST);
                        // connect readers at once
                        stubPlugin->plugStubReaders(newReaders, true);
                        logger->info("Stub Readers connected {}", stubPlugin->getReaderNames());
                        ASSERT_EQ(newReaders, stubPlugin->getReaderNames());
                        ASSERT_EQ(3, stubPlugin->getReaders()->size());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testE_PlugMultiReadersEvent() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testE_PlugMultiReadersEvent() 
                    {
                        std::set<std::string> READERS_LIST = {"E_Reader1", "E_Reader2", "E_Reader3"};

                        std::shared_ptr<std::set<std::string>> READERS = std::make_shared<std::set<std::string>>(READERS_LIST);

                        // lock test until message is received
                        std::shared_ptr<CountDownLatch> readerConnected = std::make_shared<CountDownLatch>(1);

                        // add READER_CONNECTED assert observer
                        stubPlugin->addObserver(std::make_shared<PluginObserverAnonymousInnerClass3>(shared_from_this(), READERS, readerConnected));

                        // connect readers
                        stubPlugin->plugStubReaders(READERS, false);

                        // wait for event to be raised
                        //readerConnected->await(2, TimeUnit::SECONDS);
                        ASSERT_EQ(0, readerConnected->getCount());
                    }

                    StubPluginTest::PluginObserverAnonymousInnerClass3::PluginObserverAnonymousInnerClass3(std::shared_ptr<StubPluginTest> outerInstance, std::shared_ptr<std::set<std::string>> READERS, std::shared_ptr<CountDownLatch> readerConnected) 
                    {
                        this->outerInstance = outerInstance;
                        this->READERS = READERS;
                        this->readerConnected = readerConnected;
                    }

                    void StubPluginTest::PluginObserverAnonymousInnerClass3::update(std::shared_ptr<PluginEvent> event_Renamed) 
                    {
                        outerInstance->logger->info("event {} {}", event_Renamed->getEventType(), event_Renamed->getReaderNames()->size());
                        //ASSERT_EQ(PluginEvent::EventType::READER_CONNECTED, event_Renamed->getEventType());
                        ASSERT_EQ(3, event_Renamed->getReaderNames()->size());
                        ASSERT_EQ(READERS, event_Renamed->getReaderNames());
                        readerConnected->countDown();
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testF_PlugUnplugMultiReadersCount() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testF_PlugUnplugMultiReadersCount() 
                    {                    
                        std::set<std::string> READERS_LIST = {"FC_Reader1", "FC_Reader2", "FC_Reader3"};

                        std::shared_ptr<std::set<std::string>> READERS = std::make_shared<std::set<std::string>>(READERS_LIST);

                        // connect readers at once
                        stubPlugin->plugStubReaders(READERS, true);
                        ASSERT_EQ(3, stubPlugin->getReaders()->size());
                        stubPlugin->unplugStubReaders(READERS, true);
                        ASSERT_EQ(0, stubPlugin->getReaders()->size());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testF_PlugUnplugMultiReadersEvent() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testF_PlugUnplugMultiReadersEvent() 
                    {
                        std::set<std::string> READERS_LIST = {"F_Reader1", "F_Reader2", "F_Reader3"};

                        std::shared_ptr<std::set<std::string>> READERS = std::make_shared<std::set<std::string>>(READERS_LIST);

                        // lock test until message is received
                        std::shared_ptr<CountDownLatch> readerConnected = std::make_shared<CountDownLatch>(1);
                        std::shared_ptr<CountDownLatch> readerDisconnected = std::make_shared<CountDownLatch>(1);

                        std::shared_ptr<ObservablePlugin::PluginObserver> assertDisconnect = std::make_shared<PluginObserverAnonymousInnerClass4>(shared_from_this(), READERS, readerConnected, readerDisconnected);
                        // add assert DISCONNECT assert observer
                        stubPlugin->addObserver(assertDisconnect);

                        // connect reader
                        stubPlugin->plugStubReaders(READERS, false);

                        //ASSERT_TRUE(readerConnected->await(5, TimeUnit::SECONDS));

                        stubPlugin->unplugStubReaders(READERS, false);

                        //ASSERT_TRUE(readerDisconnected->await(5, TimeUnit::SECONDS));

                        sleep(1000); // Todo fix me, should works without sleep
                        logger->debug("Stub Readers connected {}", stubPlugin->getReaderNames());
                        ASSERT_EQ(0, stubPlugin->getReaders()->size());
                        ASSERT_EQ(0, readerConnected->getCount());
                        ASSERT_EQ(0, readerDisconnected->getCount());
                    }

                    StubPluginTest::PluginObserverAnonymousInnerClass4::PluginObserverAnonymousInnerClass4(std::shared_ptr<StubPluginTest> outerInstance, std::shared_ptr<std::set<std::string>> READERS, std::shared_ptr<CountDownLatch> readerConnected, std::shared_ptr<CountDownLatch> readerDisconnected) 
                    {
                        this->outerInstance = outerInstance;
                        this->READERS = READERS;
                        this->readerConnected = readerConnected;
                        this->readerDisconnected = readerDisconnected;
                        event_i = 1;
                    }

                    void StubPluginTest::PluginObserverAnonymousInnerClass4::update(std::shared_ptr<PluginEvent> event_Renamed) 
                    {
                        outerInstance->logger->info("event {} {}", event_Renamed->getEventType(), event_Renamed->getReaderNames()->size());
                        if (event_i == 1) 
                        {
                            //ASSERT_EQ(PluginEvent::EventType::READER_CONNECTED, event_Renamed->getEventType());
                            readerConnected->countDown();
                        }
                        // analyze the second event, should be a READER_DISCONNECTED
                        if (event_i == 2) 
                        {
                            //ASSERT_EQ(PluginEvent::EventType::READER_DISCONNECTED, event_Renamed->getEventType());
                            ASSERT_EQ(3, event_Renamed->getReaderNames()->size());
                            ASSERT_EQ(READERS, event_Renamed->getReaderNames());
                            readerDisconnected->countDown();
                        }
                        event_i++;
                    }
                }
            }
        }
