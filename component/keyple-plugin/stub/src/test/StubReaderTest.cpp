
#include "StubReaderTest.h"
#include "LinkedHashSet.h"

        namespace keyple {
            namespace plugin {
                namespace stub {


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void SetUp() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubReaderTest::SetUp()
                    {
                        // clear observers from others tests as StubPlugin is a singleton
                        stubPlugin = std::make_shared<StubPlugin>( StubPlugin::getInstance() );

                        // add an observer to start the plugin monitoring thread
                        //stubPlugin->addObserver(std::make_shared<PluginObserverAnonymousInnerClass>(shared_from_this()));

                        logger->info("Stubplugin readers size {}", stubPlugin->getReaders()->size());
                        ASSERT_EQ(0, stubPlugin->getReaders()->size());

                        logger->info("Stubplugin observers size {}", stubPlugin->countObservers());
                        ASSERT_EQ(1, stubPlugin->countObservers());

                        stubPlugin->plugStubReader("StubReaderTest", true);

                        std::string szReader = "StubReaderTest";

                        reader = std::static_pointer_cast<StubReader>(stubPlugin->getReader(szReader));
                    }

                    StubReaderTest::PluginObserverAnonymousInnerClass::PluginObserverAnonymousInnerClass(std::shared_ptr<StubReaderTest> outerInstance) 
                    {
                        this->outerInstance = outerInstance;
                    }

                    void StubReaderTest::PluginObserverAnonymousInnerClass::update(std::shared_ptr<PluginEvent> event_Renamed)
                    {
                        event_Renamed = event_Renamed;
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubReaderTest::tearDown() 
                    {
                        std::shared_ptr<StubPlugin> stubPlugin = std::make_shared<StubPlugin>( StubPlugin::getInstance() );
                        stubPlugin->clearObservers();
                        reader->clearObservers();
                        stubPlugin->getInstance().unplugStubReader("StubReaderTest", true);
                    }

                    void StubReaderTest::selectSe(std::shared_ptr<SeReader> reader) 
                    {
                        std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>();//reader);
                        std::shared_ptr<AbstractSeSelectionRequest> seSelectionRequest;// = std::make_shared<AbstractSeSelectionRequest>(std::make_shared<SeSelector>(nullptr, std::make_shared<SeSelector::AtrFilter>("3B.*"), "ATR selection"), ChannelState::KEEP_OPEN, Protocol::ANY);

                        /* Prepare selector, ignore MatchingSe here */
                        seSelection->prepareSelection(seSelectionRequest);

                        seSelection->processExplicitSelection(reader);
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsert() throws InterruptedException
                    void StubReaderTest::testInsert() 
                    {
                        // CountDown lock
                        std::shared_ptr<CountDownLatch> lock = std::make_shared<CountDownLatch>(1);

                        // add observer
                        //reader->addObserver(std::make_shared<ReaderObserverAnonymousInnerClass>(shared_from_this(), lock));
                        // test
                        reader->insertSe(hoplinkSE());

                        // lock thread for 2 seconds max to wait for the event
                        //lock->await(2, TimeUnit::SECONDS);
                        ASSERT_EQ(0, lock->getCount()); // should be 0 because countDown is called by
                        // observer

                    }

                    StubReaderTest::ReaderObserverAnonymousInnerClass::ReaderObserverAnonymousInnerClass(std::shared_ptr<StubReaderTest> outerInstance, std::shared_ptr<CountDownLatch> lock) 
                    {
                        this->outerInstance = outerInstance;
                        this->lock = lock;
                    }

                    void StubReaderTest::ReaderObserverAnonymousInnerClass::update(std::shared_ptr<ReaderEvent> event_Renamed) 
                    {
                        ASSERT_EQ(event_Renamed->getReaderName(), outerInstance->reader->getName());
                        //ASSERT_EQ(event_Renamed->getPluginName(), StubPlugin::getInstance()->getName());
                        //ASSERT_EQ(ReaderEvent::EventType::SE_INSERTED, event_Renamed->getEventType());

                        outerInstance->logger->debug("testInsert event is correct");
                        // unlock thread
                        lock->countDown();
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsertMatchingSe() throws InterruptedException
                    void StubReaderTest::testInsertMatchingSe() 
                    {
                        // CountDown lock
                        std::shared_ptr<CountDownLatch> lock = std::make_shared<CountDownLatch>(1);
                        const std::string poAid = "A000000291A000000191";

                        // add observer
                        //reader->addObserver(std::make_shared<ReaderObserverAnonymousInnerClass2>(shared_from_this(), lock, poAid));

                        std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>();//reader);

                        std::shared_ptr<AbstractSeSelectionRequest> seSelectionRequest;// = std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(poAid), nullptr), nullptr, "AID: " + poAid), ChannelState::KEEP_OPEN, Protocol::ANY);

                        seSelection->prepareSelection(seSelectionRequest);

                        (std::static_pointer_cast<ObservableReader>(reader))->setDefaultSelectionRequest(seSelection->getSelectionOperation(), ObservableReader::NotificationMode::MATCHED_ONLY);

                        // test
                        reader->insertSe(hoplinkSE());

                        // lock thread for 2 seconds max to wait for the event
                        //lock->await(2, TimeUnit::SECONDS);
                        ASSERT_EQ(0, lock->getCount()); // should be 0 because countDown is called by
                        // observer
                    }

                    StubReaderTest::ReaderObserverAnonymousInnerClass2::ReaderObserverAnonymousInnerClass2(std::shared_ptr<StubReaderTest> outerInstance, std::shared_ptr<CountDownLatch> lock, const std::string &poAid) 
                    {
                        this->outerInstance = outerInstance;
                        this->lock = lock;
                        this->poAid = poAid;
                    }

                    void StubReaderTest::ReaderObserverAnonymousInnerClass2::update(std::shared_ptr<ReaderEvent> event_Renamed) 
                    {
                        ASSERT_EQ(event_Renamed->getReaderName(), outerInstance->reader->getName());
                        //ASSERT_EQ(event_Renamed->getPluginName(), StubPlugin::getInstance()->getName());
                        //ASSERT_EQ(ReaderEvent::EventType::SE_MATCHED, event_Renamed->getEventType());
                        //Assert::assertTrue(event_Renamed->getDefaultSelectionResponse()->getSelectionSeResponseSet()->getSingleResponse()->getSelectionStatus()->hasMatched());
                        //Assert::assertArrayEquals(event_Renamed->getDefaultSelectionResponse()->getSelectionSeResponseSet()->getSingleResponse()->getSelectionStatus()->getAtr()->getBytes(), hoplinkSE()->getATR());

                        // retrieve the expected FCI from the Stub SE running the select application command
                        std::vector<char> aid = ByteArrayUtils::fromHex(poAid);
                        std::vector<char> selectApplicationCommand(6 + aid.size());
                        selectApplicationCommand[0] = static_cast<char>(0x00); // CLA
                        selectApplicationCommand[1] = static_cast<char>(0xA4); // INS
                        selectApplicationCommand[2] = static_cast<char>(0x04); // P1: select by name
                        selectApplicationCommand[3] = static_cast<char>(0x00); // P2: requests the first
                        selectApplicationCommand[4] = static_cast<char>(aid.size()); // Lc
                        System::arraycopy(aid, 0, selectApplicationCommand, 5, aid.size()); // data

                        selectApplicationCommand[5 + aid.size()] = static_cast<char>(0x00); // Le
                        std::vector<char> fci;
                        try 
                        {
                            fci = hoplinkSE()->processApdu(selectApplicationCommand);
                        }
                        catch (const KeypleIOReaderException &e)
                        {
                            e.printStackTrace();
                        }

                        //Assert::assertArrayEquals(event_Renamed->getDefaultSelectionResponse()->getSelectionSeResponseSet()->getSingleResponse()->getSelectionStatus()->getFci()->getBytes(), fci);

                        outerInstance->logger->debug("match event is correct");
                        // unlock thread
                        lock->countDown();
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsertNotMatching_MatchedOnly() throws InterruptedException
                    void StubReaderTest::testInsertNotMatching_MatchedOnly() 
                    {
                        // CountDown lock
                        std::shared_ptr<CountDownLatch> lock = std::make_shared<CountDownLatch>(1);

                        // add observer
                        //reader->addObserver(std::make_shared<ReaderObserverAnonymousInnerClass3>(shared_from_this(), lock));
                        std::string poAid = "A000000291A000000192"; // not matching poAid

                        std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>();//reader);

                        std::shared_ptr<AbstractSeSelectionRequest> seSelectionRequest;// = std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(poAid), nullptr), nullptr, "AID: " + poAid), ChannelState::KEEP_OPEN, Protocol::ANY);

                        seSelection->prepareSelection(seSelectionRequest);

                        (std::static_pointer_cast<ObservableReader>(reader))->setDefaultSelectionRequest(seSelection->getSelectionOperation(), ObservableReader::NotificationMode::MATCHED_ONLY);

                        // test
                        reader->insertSe(hoplinkSE());

                        // lock thread for 2 seconds max to wait for the event
                        //lock->await(100, TimeUnit::MILLISECONDS);
                        ASSERT_EQ(1, lock->getCount()); // should be 1 because countDown is never called
                    }

                    StubReaderTest::ReaderObserverAnonymousInnerClass3::ReaderObserverAnonymousInnerClass3(std::shared_ptr<StubReaderTest> outerInstance, std::shared_ptr<CountDownLatch> lock) 
                    {
                        this->outerInstance = outerInstance;
                        this->lock = lock;
                    }

                    void StubReaderTest::ReaderObserverAnonymousInnerClass3::update(std::shared_ptr<ReaderEvent> event_Renamed) 
                    {
                        // no event is thrown
                        (void)event_Renamed;
                        lock->countDown(); // should not be called
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsertNotMatching_Always() throws InterruptedException
                    void StubReaderTest::testInsertNotMatching_Always() 
                    {
                        // CountDown lock
                        std::shared_ptr<CountDownLatch> lock = std::make_shared<CountDownLatch>(1);

                        // add observer
                        //reader->addObserver(std::make_shared<ReaderObserverAnonymousInnerClass4>(shared_from_this(), lock));
                        std::string poAid = "A000000291A000000192"; // not matching poAid

                        std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>();//reader);

                        std::shared_ptr<AbstractSeSelectionRequest> seSelectionRequest;// = std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(poAid), nullptr), nullptr, "AID: " + poAid), ChannelState::KEEP_OPEN, Protocol::ANY);

                        seSelection->prepareSelection(seSelectionRequest);

                        (std::static_pointer_cast<ObservableReader>(reader))->setDefaultSelectionRequest(seSelection->getSelectionOperation(), ObservableReader::NotificationMode::ALWAYS);

                        // test
                        reader->insertSe(hoplinkSE());

                        // lock thread for 2 seconds max to wait for the event
                        //lock->await(2, TimeUnit::SECONDS);
                        ASSERT_EQ(0, lock->getCount()); // should be 0 because countDown is called by
                        // observer
                    }

                    StubReaderTest::ReaderObserverAnonymousInnerClass4::ReaderObserverAnonymousInnerClass4(std::shared_ptr<StubReaderTest> outerInstance, std::shared_ptr<CountDownLatch> lock) 
                    {
                        this->outerInstance = outerInstance;
                        this->lock = lock;
                    }

                    void StubReaderTest::ReaderObserverAnonymousInnerClass4::update(std::shared_ptr<ReaderEvent> event_Renamed)
                    {
                        ASSERT_EQ(event_Renamed->getReaderName(), outerInstance->reader->getName());
                        ASSERT_EQ(event_Renamed->getPluginName(), StubPlugin::getInstance().getName());

                        // an SE_INSERTED event is thrown
                        //ASSERT_EQ(ReaderEvent::EventType::SE_INSERTED, event_Renamed->getEventType());

                        // card has not match
                        ASSERT_FALSE(event_Renamed->getDefaultSelectionsResponse()->getSelectionSeResponseSet()->getSingleResponse()->getSelectionStatus()->hasMatched());

                        lock->countDown(); // should be called
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testATR() throws InterruptedException
                    void StubReaderTest::testATR() 
                    {
                        // CountDown lock
                        std::shared_ptr<CountDownLatch> lock = std::make_shared<CountDownLatch>(1);

                        // add observer
                        //reader->addObserver(std::make_shared<ReaderObserverAnonymousInnerClass5>(shared_from_this(), lock));

                        // test
                        reader->insertSe(hoplinkSE());

                        // lock thread for 2 seconds max to wait for the event
                        //lock->await(2, TimeUnit::SECONDS);
                    }

                    StubReaderTest::ReaderObserverAnonymousInnerClass5::ReaderObserverAnonymousInnerClass5(std::shared_ptr<StubReaderTest> outerInstance, std::shared_ptr<CountDownLatch> lock) 
                    {
                        this->outerInstance = outerInstance;
                        this->lock = lock;
                    }

                    void StubReaderTest::ReaderObserverAnonymousInnerClass5::update(std::shared_ptr<ReaderEvent> event_Renamed) 
                    {
                        (void)event_Renamed;
                        //ASSERT_EQ(ReaderEvent::EventType::SE_INSERTED, event_Renamed->getEventType());

                        std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>();//outerInstance->reader);
                        std::shared_ptr<AbstractSeSelectionRequest> seSelectionRequest;// = std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(nullptr, std::make_shared<SeSelector::AtrFilter>("3B.*"), "Test ATR"), ChannelState::KEEP_OPEN, Protocol::ANY);

                        // Prepare selector, ignore MatchingSe here
                        seSelection->prepareSelection(seSelectionRequest);

                        try 
                        {
                            seSelection->processExplicitSelection(outerInstance->reader);

                            std::shared_ptr<MatchingSe> matchingSe;// = seSelection->getSelectedSe();

                            ASSERT_FALSE(matchingSe == nullptr);
                        }
                        catch (const KeypleReaderException &e) 
                        {
                            //Assert::fail("Unexcepted exception");
                        }
                        // unlock thread
                        lock->countDown();
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void transmit_Hoplink_null() throws Exception
                    void StubReaderTest::transmit_Hoplink_null() 
                    {
                        std::shared_ptr<SeRequestSet> requestSet = nullptr;

                        reader->insertSe(hoplinkSE());
                        reader->transmitSet(requestSet);

                        // throws exception
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_Hoplink_Successful() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                    void StubReaderTest::transmit_Hoplink_Successful() 
                    {
                        // init Request
                        std::shared_ptr<SeRequestSet> requests = getRequestIsoDepSetSample();

                        // init SE
                        reader->insertSe(hoplinkSE());

                        // send the selection request
                        selectSe(reader);

                        // add Protocol flag
                        //reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(SeCommonProtocols::PROTOCOL_ISO14443_4), "");
                        // test
                        std::shared_ptr<SeResponseSet> seResponse = reader->transmitSet(requests);

                        // assert
                        ASSERT_TRUE( (seResponse->getSingleResponse()->getApduResponses()[0])->isSuccessful() );
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void transmit_no_response() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                    void StubReaderTest::transmit_no_response() 
                    {
                        // init Request
                        std::shared_ptr<SeRequestSet> requests = getNoResponseRequest();

                        // init SE
                        reader->insertSe(noApduResponseSE());

                        // add Protocol flag
                        //reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4));

                        // send the selection request
                        selectSe(reader);

                        // test
                        std::shared_ptr<SeResponseSet> seResponse = reader->transmitSet(requests);
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_0() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                    void StubReaderTest::transmit_partial_response_set_0() 
                    {
                        // init Request
                        std::shared_ptr<SeRequestSet> seRequestSet = getPartialRequestSet(0);

                        // init SE
                        reader->insertSe(partialSE());

                        // add Protocol flag
                        //reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4));

                        // send the selection request
                        selectSe(reader);

                        // test
                        try 
                        {
                            std::shared_ptr<SeResponseSet> seResponseSet = reader->transmitSet(seRequestSet);
                        }
                        catch ( KeypleReaderException & ex) 
                        {
                            ASSERT_EQ( ex.getSeResponseSet()->getResponses().size(), 1);
                            ASSERT_EQ( (ex.getSeResponseSet()->getResponses()[0])->getApduResponses().size(), 2);
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_1() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                    void StubReaderTest::transmit_partial_response_set_1() 
                    {
                        // init Request
                        std::shared_ptr<SeRequestSet> seRequestSet = getPartialRequestSet(1);

                        // init SE
                        reader->insertSe(partialSE());

                        // add Protocol flag
                        //reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4));

                        // send the selection request
                        selectSe(reader);

                        // test
                        try 
                        {
                            std::shared_ptr<SeResponseSet> seResponseSet = reader->transmitSet(seRequestSet);
                        }
                        catch ( KeypleReaderException & ex)
                        {
                            ASSERT_EQ( ex.getSeResponseSet()->getResponses().size(), 2);
                            ASSERT_EQ( (ex.getSeResponseSet()->getResponses()[0])->getApduResponses().size(), 4);
                            ASSERT_EQ( (ex.getSeResponseSet()->getResponses()[1])->getApduResponses().size(), 2);
                            ASSERT_EQ( (ex.getSeResponseSet()->getResponses()[1])->getApduResponses().size(), 2);
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_2() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                    void StubReaderTest::transmit_partial_response_set_2() 
                    {
                        // init Request
                        std::shared_ptr<SeRequestSet> seRequestSet = getPartialRequestSet(2);

                        // init SE
                        reader->insertSe(partialSE());

                        // add Protocol flag
                        //reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4));

                        // send the selection request
                        selectSe(reader);

                        // test
                        try 
                        {
                            std::shared_ptr<SeResponseSet> seResponseSet = reader->transmitSet(seRequestSet);
                        }
                        catch ( KeypleReaderException & ex ) 
                        {
                            ASSERT_EQ( ex.getSeResponseSet()->getResponses().size(), 3);
                            ASSERT_EQ( (ex.getSeResponseSet()->getResponses()[0])->getApduResponses().size(), 4);
                            ASSERT_EQ( (ex.getSeResponseSet()->getResponses()[1])->getApduResponses().size(), 4);
                            ASSERT_EQ( (ex.getSeResponseSet()->getResponses()[2])->getApduResponses().size(), 2);
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_3() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                    void StubReaderTest::transmit_partial_response_set_3() 
                    {
                        // init Request
                        std::shared_ptr<SeRequestSet> seRequestSet = getPartialRequestSet(3);

                        // init SE
                        reader->insertSe(partialSE());

                        // add Protocol flag
                        //reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4));

                        // send the selection request
                        selectSe(reader);

                        // test
                        try 
                        {
                            std::shared_ptr<SeResponseSet> seResponseSet = reader->transmitSet(seRequestSet);
                        }
                        catch ( KeypleReaderException & ex )
                        {
                            ASSERT_EQ( ex.getSeResponseSet()->getResponses().size(), 3);
                            ASSERT_EQ( (ex.getSeResponseSet()->getResponses()[0])->getApduResponses().size(), 4);
                            ASSERT_EQ( (ex.getSeResponseSet()->getResponses()[1])->getApduResponses().size(), 4);
                            ASSERT_EQ( (ex.getSeResponseSet()->getResponses()[2])->getApduResponses().size(), 4);
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_0() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                    void StubReaderTest::transmit_partial_response_0() 
                    {
                        // init Request
                        std::shared_ptr<SeRequest> seRequest = getPartialRequest(0);

                        // init SE
                        reader->insertSe(partialSE());

                        // add Protocol flag
                        //reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4));

                        // send the selection request
                        selectSe(reader);

                        // test
                        try 
                        {
                            std::shared_ptr<SeResponse> seResponse = reader->transmit(seRequest);
                        }
                        catch ( KeypleReaderException & ex ) 
                        {
                            ASSERT_EQ( ex.getSeResponse()->getApduResponses().size(), 0 );
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_1() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                    void StubReaderTest::transmit_partial_response_1() 
                    {
                        // init Request
                        std::shared_ptr<SeRequest> seRequest = getPartialRequest(1);

                        // init SE
                        reader->insertSe(partialSE());

                        // add Protocol flag
                        //reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4));

                        // send the selection request
                        selectSe(reader);

                        // test
                        try 
                        {
                            std::shared_ptr<SeResponse> seResponse = reader->transmit(seRequest);
                        }
                        catch ( KeypleReaderException & ex ) 
                        {
                            ASSERT_EQ( ex.getSeResponse()->getApduResponses().size(), 1 );
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_2() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                    void StubReaderTest::transmit_partial_response_2() 
                    {
                        // init Request
                        std::shared_ptr<SeRequest> seRequest = getPartialRequest(2);

                        // init SE
                        reader->insertSe(partialSE());

                        // add Protocol flag
                        //reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4));

                        // send the selection request
                        selectSe(reader);

                        // test
                        try 
                        {
                            std::shared_ptr<SeResponse> seResponse = reader->transmit(seRequest);
                        }
                        catch ( KeypleReaderException & ex ) 
                        {
                            ASSERT_EQ( ex.getSeResponse()->getApduResponses().size(), 2 );
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_3() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                    void StubReaderTest::transmit_partial_response_3() 
                    {
                        // init Request
                        std::shared_ptr<SeRequest> seRequest = getPartialRequest(3);

                        // init SE
                        reader->insertSe(partialSE());

                        // add Protocol flag
                        //reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4));

                        // send the selection request
                        selectSe(reader);

                        // test
                        try 
                        {
                            std::shared_ptr<SeResponse> seResponse = reader->transmit(seRequest);
                        }
                        catch ( KeypleReaderException & ex )  
                        {
                            ASSERT_EQ( ex.getSeResponse()->getApduResponses().size(), 3 );
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetName()
                    void StubReaderTest::testGetName() 
                    {
                        //Assert::assertNotNull(reader->getName());
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void testSetWrongParameter() throws Exception
                    void StubReaderTest::testSetWrongParameter() 
                    {
                        reader->setParameter("WRONG_PARAMETER", "a");
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void testSetWrongParameters() throws Exception
                    void StubReaderTest::testSetWrongParameters() 
                    {
                        std::unordered_map<std::string, std::string> parameters;
                        parameters.emplace("WRONG_PARAMETER", "d");
                        parameters.emplace(StubReader::ALLOWED_PARAMETER_1, "a");
                        reader->setParameters(parameters);
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSetParameters() throws Exception
                    void StubReaderTest::testSetParameters() 
                    {
                        std::unordered_map<std::string, std::string> p1;
                        p1.emplace(StubReader::ALLOWED_PARAMETER_1, "a");
                        p1.emplace(StubReader::ALLOWED_PARAMETER_2, "a");

                        reader->setParameters(p1);
                        std::unordered_map<std::string, std::string> p2 = reader->getParameters();
                        //assert(p1.equals(p2));
                    }

                    void StubReaderTest::ReaderObserverAnonymousInnerClass6::update(std::shared_ptr<ReaderEvent> readerEvent)
                    {
                        (void)readerEvent;
                    }

                    void StubReaderTest::ReaderObserverAnonymousInnerClass7::update(std::shared_ptr<ReaderEvent> readerEvent) 
                    {
                        (void)readerEvent;
                    }

                    std::shared_ptr<SeRequestSet> StubReaderTest::getRequestIsoDepSetSample()
                    {
                        std::string poAid = "A000000291A000000191";

                        std::shared_ptr<ReadRecordsCmdBuild> poReadRecordCmd_T2Env;// = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, static_cast<char>(0x14), static_cast<char>(0x01), true, static_cast<char>(0x20), "");

                        std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;

                        poApduRequestList.push_back( poReadRecordCmd_T2Env->getApduRequest() );

                        std::shared_ptr<SeRequest> seRequest = std::make_shared<SeRequest>(poApduRequestList, ChannelState::CLOSE_AFTER);

                        return std::make_shared<SeRequestSet>(seRequest);
                    }

                    std::shared_ptr<SeRequestSet> StubReaderTest::getNoResponseRequest() 
                    {
                        std::shared_ptr<IncreaseCmdBuild> poIncreaseCmdBuild = std::make_shared<IncreaseCmdBuild>(PoClass::ISO, static_cast<char>(0x14), static_cast<char>(0x01), 0, "");

                        std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;

                        poApduRequestList.push_back( poIncreaseCmdBuild->getApduRequest() );

                        std::shared_ptr<SeRequest> seRequest = std::make_shared<SeRequest>(poApduRequestList, ChannelState::CLOSE_AFTER);

                        return std::make_shared<SeRequestSet>(seRequest);
                    }

                    std::shared_ptr<SeRequestSet> StubReaderTest::getPartialRequestSet(int scenario) 
                    {
                        std::string poAid = "A000000291A000000191";

                        std::shared_ptr<ReadRecordsCmdBuild> poReadRecord1CmdBuild;// = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, static_cast<char>(0x14), static_cast<char>(0x01), true, "");

                        /* this command doesn't in the PartialSE */
                        std::shared_ptr<ReadRecordsCmdBuild> poReadRecord2CmdBuild;// = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, static_cast<char>(0x1E), static_cast<char>(0x01), true, "");

                        std::vector<std::shared_ptr<ApduRequest>> poApduRequestList1;
                        poApduRequestList1.push_back(poReadRecord1CmdBuild->getApduRequest());
                        poApduRequestList1.push_back(poReadRecord1CmdBuild->getApduRequest());
                        poApduRequestList1.push_back(poReadRecord1CmdBuild->getApduRequest());
                        poApduRequestList1.push_back(poReadRecord1CmdBuild->getApduRequest());

                        std::vector<std::shared_ptr<ApduRequest>> poApduRequestList2;
                        poApduRequestList2.push_back(poReadRecord1CmdBuild->getApduRequest());
                        poApduRequestList2.push_back(poReadRecord1CmdBuild->getApduRequest());
                        poApduRequestList2.push_back(poReadRecord1CmdBuild->getApduRequest());
                        poApduRequestList2.push_back(poReadRecord1CmdBuild->getApduRequest());

                        std::vector<std::shared_ptr<ApduRequest>> poApduRequestList3;
                        poApduRequestList3.push_back(poReadRecord1CmdBuild->getApduRequest());
                        poApduRequestList3.push_back(poReadRecord1CmdBuild->getApduRequest());
                        poApduRequestList3.push_back(poReadRecord2CmdBuild->getApduRequest());
                        poApduRequestList3.push_back(poReadRecord1CmdBuild->getApduRequest());

                        std::shared_ptr<SeRequest> seRequest1 = std::make_shared<SeRequest>(poApduRequestList1, ChannelState::KEEP_OPEN);

                        std::shared_ptr<SeRequest> seRequest2 = std::make_shared<SeRequest>(poApduRequestList2, ChannelState::KEEP_OPEN);

                        /* This SeRequest fails at step 3 */
                        std::shared_ptr<SeRequest> seRequest3 = std::make_shared<SeRequest>(poApduRequestList3, ChannelState::KEEP_OPEN);

                        std::shared_ptr<SeRequest> seRequest4 = std::make_shared<SeRequest>(poApduRequestList1, ChannelState::KEEP_OPEN);

                        std::shared_ptr<std::set<std::shared_ptr<SeRequest>>> seRequestSets;
                        //seRequestSets-> = std::make_shared<LinkedHashSet<std::shared_ptr<SeRequest>>>();

                        switch (scenario) 
                        {
                            case 0:
                                /* 0 response Set */
                                seRequestSets->insert(seRequest3); // fails
                                seRequestSets->insert(seRequest1); // succeeds
                                seRequestSets->insert(seRequest2); // succeeds
                                break;
                            case 1:
                                /* 1 response Set */
                                seRequestSets->insert(seRequest1); // succeeds
                                seRequestSets->insert(seRequest3); // fails
                                seRequestSets->insert(seRequest2); // succeeds
                                break;
                            case 2:
                                /* 2 responses Set */
                                seRequestSets->insert(seRequest1); // succeeds
                                seRequestSets->insert(seRequest2); // succeeds
                                seRequestSets->insert(seRequest3); // fails
                                break;
                            case 3:
                                /* 3 responses Set */
                                seRequestSets->insert(seRequest1); // succeeds
                                seRequestSets->insert(seRequest2); // succeeds
                                seRequestSets->insert(seRequest4); // succeeds
                                break;
                            default:
                                break;
                        }

                        return std::make_shared<SeRequestSet>(seRequestSets);
                    }

                    std::shared_ptr<SeRequest> StubReaderTest::getPartialRequest(int scenario) 
                    {
                        std::string poAid = "A000000291A000000191";

                        std::shared_ptr<ReadRecordsCmdBuild> poReadRecord1CmdBuild;// = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, static_cast<char>(0x14), static_cast<char>(0x01), true, "");

                        /* this command doesn't in the PartialSE */
                        std::shared_ptr<ReadRecordsCmdBuild> poReadRecord2CmdBuild;// = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, static_cast<char>(0x1E), static_cast<char>(0x01), true, "");

                        std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;

                        switch (scenario) 
                        {
                            case 0:
                                poApduRequestList.push_back(poReadRecord2CmdBuild->getApduRequest()); // fails
                                poApduRequestList.push_back(poReadRecord1CmdBuild->getApduRequest()); // succeeds
                                poApduRequestList.push_back(poReadRecord1CmdBuild->getApduRequest()); // succeeds
                                break;
                            case 1:
                                poApduRequestList.push_back(poReadRecord1CmdBuild->getApduRequest()); // succeeds
                                poApduRequestList.push_back(poReadRecord2CmdBuild->getApduRequest()); // fails
                                poApduRequestList.push_back(poReadRecord1CmdBuild->getApduRequest()); // succeeds
                                break;
                            case 2:
                                poApduRequestList.push_back(poReadRecord1CmdBuild->getApduRequest()); // succeeds
                                poApduRequestList.push_back(poReadRecord1CmdBuild->getApduRequest()); // succeeds
                                poApduRequestList.push_back(poReadRecord2CmdBuild->getApduRequest()); // fails
                                break;
                            case 3:
                                poApduRequestList.push_back(poReadRecord1CmdBuild->getApduRequest()); // succeeds
                                poApduRequestList.push_back(poReadRecord1CmdBuild->getApduRequest()); // succeeds
                                poApduRequestList.push_back(poReadRecord1CmdBuild->getApduRequest()); // succeeds
                                break;
                            default:
                                break;
                        }

                        std::shared_ptr<SeSelector> selector;// = std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(poAid), nullptr), nullptr, nullptr);

                        return std::make_shared<SeRequest>(poApduRequestList, ChannelState::CLOSE_AFTER);
                    }

                    std::shared_ptr<StubSecureElement> StubReaderTest::hoplinkSE()
                     {
                        return std::make_shared<StubSecureElementAnonymousInnerClass>();
                    }

                    std::vector<char> StubReaderTest::StubSecureElementAnonymousInnerClass::processApdu(std::vector<char> &apduIn) 
                    {
                        addHexCommand("00 A4 04 00 0A A0 00 00 02 91 A0 00 00 01 91 00", "6F25840BA000000291A00000019102A516BF0C13C70800000000C0E11FA653070A3C230C1410019000");

                        addHexCommand("00 A4 04 00 0A A0 00 00 02 91 A0 00 00 01 92 00", "6A82");

                        addHexCommand("00 B2 01 A4 20", "00000000000000000000000000000000000000000000000000000000000000009000");

                        return StubSecureElement::processApdu(apduIn);
                    }

                    std::vector<char> StubReaderTest::StubSecureElementAnonymousInnerClass::getATR() 
                    {
                        return ByteArrayUtils::fromHex("3B 8E 80 01 80 31 80 66 40 90 89 12 08 02 83 01 90 00 0B");
                    }

                    std::string StubReaderTest::StubSecureElementAnonymousInnerClass::getSeProcotol() 
                    {
                        return "PROTOCOL_ISO14443_4";
                    }

                    std::shared_ptr<StubSecureElement> StubReaderTest::noApduResponseSE() 
                    {
                        return std::make_shared<StubSecureElementAnonymousInnerClass2>();
                    }

                    std::vector<char> StubReaderTest::StubSecureElementAnonymousInnerClass2::processApdu(std::vector<char> &apduIn) 
                    {
                        addHexCommand("00 A4 04 00 0A A0 00 00 02 91 A0 00 00 01 91 00", "6F25840BA000000291A00000019102A516BF0C13C70800000000C0E11FA653070A3C230C1410019000");

                        return StubSecureElement::processApdu(apduIn);
                    }

                    std::vector<char> StubReaderTest::StubSecureElementAnonymousInnerClass2::getATR() 
                    {
                        return ByteArrayUtils::fromHex("3B 8E 80 01 80 31 80 66 40 90 89 12 08 02 83 01 90 00 0B");
                    }

                    std::string StubReaderTest::StubSecureElementAnonymousInnerClass2::getSeProcotol() 
                    {
                        return "PROTOCOL_ISO14443_4";
                    }

                    std::shared_ptr<StubSecureElement> StubReaderTest::partialSE() 
                    {
                        return std::make_shared<StubSecureElementAnonymousInnerClass3>();
                    }

                    std::vector<char> StubReaderTest::StubSecureElementAnonymousInnerClass3::processApdu(std::vector<char> &apduIn) 
                    {
                        addHexCommand("00 A4 04 00 0A A0 00 00 02 91 A0 00 00 01 91 00", "6F25840BA000000291A00000019102A516BF0C13C70800000000C0E11FA653070A3C230C1410019000");
                        addHexCommand("00 B2 01 A4 00", "00000000000000000000000000000000000000000000000000000000009000");

                        return StubSecureElement::processApdu(apduIn);
                    }

                    std::vector<char> StubReaderTest::StubSecureElementAnonymousInnerClass3::getATR() 
                    {
                        return ByteArrayUtils::fromHex("3B 8E 80 01 80 31 80 66 40 90 89 12 08 02 83 01 90 00 0B");
                    }

                    std::string StubReaderTest::StubSecureElementAnonymousInnerClass3::getSeProcotol() 
                    {
                        return "PROTOCOL_ISO14443_4";
                    }

                    std::shared_ptr<StubSecureElement> StubReaderTest::getSENoconnection() 
                    {
                        return std::make_shared<StubSecureElementAnonymousInnerClass4>();
                    }

                    std::vector<char> StubReaderTest::StubSecureElementAnonymousInnerClass4::getATR()
                    {
                        return std::vector<char>(0);
                    }

                    bool StubReaderTest::StubSecureElementAnonymousInnerClass4::isPhysicalChannelOpen()
                    {
                        return false;
                    }

                    void StubReaderTest::StubSecureElementAnonymousInnerClass4::openPhysicalChannel() 
                    {
                        throw std::make_shared<KeypleChannelStateException>("Impossible to estasblish connection");
                    }

                    void StubReaderTest::StubSecureElementAnonymousInnerClass4::closePhysicalChannel() 
                    {
                        throw std::make_shared<KeypleChannelStateException>("Channel is not open");
                    }

                    std::vector<char> StubReaderTest::StubSecureElementAnonymousInnerClass4::processApdu(std::vector<char> &apduIn) 
                    {
                        (void)apduIn;
                        throw std::make_shared<KeypleIOReaderException>("Error while transmitting apdu");
                    }

                    std::string StubReaderTest::StubSecureElementAnonymousInnerClass4::getSeProcotol() 
                    {
                        return "";
                    }

                    std::shared_ptr<ApduRequest> StubReaderTest::getApduSample() 
                    {
                        std::vector<char> l = ByteArrayUtils::fromHex("FEDCBA98 9005h");
                        return std::make_shared<ApduRequest>( l, false );
                    }
                }
            }
        }
