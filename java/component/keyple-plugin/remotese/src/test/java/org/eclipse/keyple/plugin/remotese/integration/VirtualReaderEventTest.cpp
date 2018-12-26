#include "VirtualReaderEventTest.h"
#include "../../../../../../../../../../stub/src/test/java/org/eclipse/keyple/plugin/stub/StubReaderTest.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "../../../../../../../../../../stub/src/main/java/org/eclipse/keyple/plugin/stub/StubPlugin.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelection.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/Protocol.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelector.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleIOReaderException.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/transaction/MatchingSe.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace integration {
//                        import static org.eclipse.keyple.plugin.stub.StubReaderTest.hoplinkSE;
                        using StubPlugin = org::eclipse::keyple::plugin::stub::StubPlugin;
                        using StubReaderTest = org::eclipse::keyple::plugin::stub::StubReaderTest;
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeypleIOReaderException = org::eclipse::keyple::seproxy::exception::KeypleIOReaderException;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using Protocol = org::eclipse::keyple::seproxy::protocol::Protocol;
                        using MatchingSe = org::eclipse::keyple::transaction::MatchingSe;
                        using SeSelection = org::eclipse::keyple::transaction::SeSelection;
                        using SeSelector = org::eclipse::keyple::transaction::SeSelector;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using namespace org::junit;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> VirtualReaderEventTest::logger = org::slf4j::LoggerFactory::getLogger(VirtualReaderEventTest::typeid);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsert() throws Exception
                        void VirtualReaderEventTest::testInsert() throw(std::runtime_error) {


                            // lock test until message is received
                            std::shared_ptr<CountDownLatch> * const lock = std::make_shared<CountDownLatch>(1);

                            // add stubPluginObserver
                            virtualReader->addObserver(std::make_shared<ReaderObserverAnonymousInnerClass>(shared_from_this(), lock));

                            logger->info("Insert a Hoplink SE and wait 5 seconds for a SE event to be thrown");

                            delay(500);

                            // insert SE
                            nativeReader->insertSe(StubReaderTest::hoplinkSE());
                            // wait 5 seconds
                            lock->await(5, TimeUnit::SECONDS);

                            Assert::assertEquals(0, lock->getCount());
                        }

                        VirtualReaderEventTest::ReaderObserverAnonymousInnerClass::ReaderObserverAnonymousInnerClass(std::shared_ptr<VirtualReaderEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock) {
                            this->outerInstance = outerInstance;
                            this->lock = lock;
                        }

                        void VirtualReaderEventTest::ReaderObserverAnonymousInnerClass::update(std::shared_ptr<ReaderEvent> event_Renamed) {
                            Assert::assertEquals(event_Renamed->getReaderName(), outerInstance->nativeReader->getName());
                            Assert::assertEquals(event_Renamed->getPluginName(), StubPlugin::getInstance()->getName());
                            Assert::assertEquals(ReaderEvent::EventType::SE_INSERTED, event_Renamed->getEventType());
                            logger->debug("Reader Event is correct, release lock");
                            lock->countDown();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRemoveEvent() throws Exception
                        void VirtualReaderEventTest::testRemoveEvent() throw(std::runtime_error) {

                            // lock test until two messages are received
                            std::shared_ptr<CountDownLatch> * const lock = std::make_shared<CountDownLatch>(2);

                            // add stubPluginObserver
                            virtualReader->addObserver(std::make_shared<ReaderObserverAnonymousInnerClass2>(shared_from_this(), lock));

                            logger->info("Insert and remove a Hoplink SE and wait 5 seconds for two SE events to be thrown");

                            // insert SE
                            nativeReader->insertSe(StubReaderTest::hoplinkSE());

                            // wait 1 second
                            delay(500);

                            // remove SE
                            nativeReader->removeSe();

                            // wait 5 seconds
                            lock->await(5, TimeUnit::SECONDS);

                            Assert::assertEquals(0, lock->getCount());

                            // https://github.com/calypsonet/keyple-java/issues/420
                            // Assert.assertEquals(0, virtualReaderService.getPlugin().getReaders().size());
                        }

                        VirtualReaderEventTest::ReaderObserverAnonymousInnerClass2::ReaderObserverAnonymousInnerClass2(std::shared_ptr<VirtualReaderEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock) {
                            this->outerInstance = outerInstance;
                            this->lock = lock;
                        }

                        void VirtualReaderEventTest::ReaderObserverAnonymousInnerClass2::update(std::shared_ptr<ReaderEvent> event_Renamed) {
                            if (event_Renamed->getEventType() == ReaderEvent::EventType::SE_INSERTED) {
                                // we expect the first event to be SE_INSERTED
                                Assert::assertEquals(2, lock->getCount());
                                lock->countDown();
                            }
                            else {
                                // the next event should be SE_REMOVAL
                                Assert::assertEquals(1, lock->getCount());
                                Assert::assertEquals(event_Renamed->getReaderName(), outerInstance->nativeReader->getName());
                                Assert::assertEquals(event_Renamed->getPluginName(), StubPlugin::getInstance()->getName());
                                Assert::assertEquals(ReaderEvent::EventType::SE_REMOVAL, event_Renamed->getEventType());
                                logger->debug("Reader Event is correct, release lock");
                                lock->countDown();

                            }
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsertMatchingSe() throws InterruptedException
                        void VirtualReaderEventTest::testInsertMatchingSe() throw(InterruptedException) {

                            // CountDown lock
                            std::shared_ptr<CountDownLatch> * const lock = std::make_shared<CountDownLatch>(1);
                            const std::string poAid = "A000000291A000000191";

                            // add observer
                            virtualReader->addObserver(std::make_shared<ReaderObserverAnonymousInnerClass3>(shared_from_this(), lock, poAid));

                            std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>(virtualReader);

                            std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(ByteArrayUtils::fromHex(poAid), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, Protocol::ANY, "AID: " + poAid);

                            seSelection->prepareSelection(seSelector);

                            (std::static_pointer_cast<ObservableReader>(virtualReader))->setDefaultSelectionRequest(seSelection->getSelectionOperation(), ObservableReader::NotificationMode::MATCHED_ONLY);

                            // wait 1 second
                            delay(1000);

                            // test
                            nativeReader->insertSe(StubReaderTest::hoplinkSE());

                            // lock thread for 2 seconds max to wait for the event
                            lock->await(5, TimeUnit::SECONDS);
                            Assert::assertEquals(0, lock->getCount()); // should be 0 because countDown is called by
                            // observer

                        }

                        VirtualReaderEventTest::ReaderObserverAnonymousInnerClass3::ReaderObserverAnonymousInnerClass3(std::shared_ptr<VirtualReaderEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock, const std::string &poAid) {
                            this->outerInstance = outerInstance;
                            this->lock = lock;
                            this->poAid = poAid;
                        }

                        void VirtualReaderEventTest::ReaderObserverAnonymousInnerClass3::update(std::shared_ptr<ReaderEvent> event_Renamed) {
                            Assert::assertEquals(event_Renamed->getReaderName(), outerInstance->nativeReader->getName());
                            Assert::assertEquals(event_Renamed->getPluginName(), StubPlugin::getInstance()->getName());
                            Assert::assertEquals(ReaderEvent::EventType::SE_MATCHED, event_Renamed->getEventType());
                            Assert::assertTrue(event_Renamed->getDefaultSelectionResponse()->getSelectionSeResponseSet()->getSingleResponse()->getSelectionStatus()->hasMatched());

                            Assert::assertArrayEquals(event_Renamed->getDefaultSelectionResponse()->getSelectionSeResponseSet()->getSingleResponse()->getSelectionStatus()->getAtr()->getBytes(), StubReaderTest::hoplinkSE()->getATR());

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
                            try {
                                fci = StubReaderTest::hoplinkSE()->processApdu(selectApplicationCommand);
                            }
                            catch (const KeypleIOReaderException &e) {
                                e->printStackTrace();
                            }

                            Assert::assertArrayEquals(event_Renamed->getDefaultSelectionResponse()->getSelectionSeResponseSet()->getSingleResponse()->getSelectionStatus()->getFci()->getBytes(), fci);

                            logger->debug("match event is correct");
                            // unlock thread
                            lock->countDown();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsertNotMatching_MatchedOnly() throws InterruptedException
                        void VirtualReaderEventTest::testInsertNotMatching_MatchedOnly() throw(InterruptedException) {

                            // CountDown lock
                            std::shared_ptr<CountDownLatch> * const lock = std::make_shared<CountDownLatch>(1);

                            // add observer
                            virtualReader->addObserver(std::make_shared<ReaderObserverAnonymousInnerClass4>(shared_from_this(), lock));
                            std::string poAid = "A000000291A000000192"; // not matching poAid

                            std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>(virtualReader);

                            std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(ByteArrayUtils::fromHex(poAid), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, Protocol::ANY, "AID: " + poAid);

                            seSelection->prepareSelection(seSelector);

                            (std::static_pointer_cast<ObservableReader>(virtualReader))->setDefaultSelectionRequest(seSelection->getSelectionOperation(), ObservableReader::NotificationMode::MATCHED_ONLY);

                            // wait 1 second
                            logger->debug("Wait 1 second before inserting SE");
                            delay(500);

                            // test
                            nativeReader->insertSe(StubReaderTest::hoplinkSE());


                            // lock thread for 2 seconds max to wait for the event
                            lock->await(3, TimeUnit::SECONDS);
                            Assert::assertEquals(1, lock->getCount()); // should be 1 because countDown is never called
                        }

                        VirtualReaderEventTest::ReaderObserverAnonymousInnerClass4::ReaderObserverAnonymousInnerClass4(std::shared_ptr<VirtualReaderEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock) {
                            this->outerInstance = outerInstance;
                            this->lock = lock;
                        }

                        void VirtualReaderEventTest::ReaderObserverAnonymousInnerClass4::update(std::shared_ptr<ReaderEvent> event_Renamed) {
                            // no event should be thrown
                            Assert::fail();
                            lock->countDown(); // should not be called
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsertNotMatching_Always() throws InterruptedException
                        void VirtualReaderEventTest::testInsertNotMatching_Always() throw(InterruptedException) {

                            // CountDown lock
                            std::shared_ptr<CountDownLatch> * const lock = std::make_shared<CountDownLatch>(1);

                            // add observer
                            virtualReader->addObserver(std::make_shared<ReaderObserverAnonymousInnerClass5>(shared_from_this(), lock));
                            std::string poAid = "A000000291A000000192"; // not matching poAid

                            std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>(virtualReader);

                            std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(ByteArrayUtils::fromHex(poAid), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, Protocol::ANY, "AID: " + poAid);

                            seSelection->prepareSelection(seSelector);

                            (std::static_pointer_cast<ObservableReader>(virtualReader))->setDefaultSelectionRequest(seSelection->getSelectionOperation(), ObservableReader::NotificationMode::ALWAYS);

                            // wait 1 second
                            logger->debug("Wait 1 second before inserting SE");
                            delay(500);

                            // test
                            nativeReader->insertSe(StubReaderTest::hoplinkSE());

                            // lock thread for 2 seconds max to wait for the event
                            lock->await(5, TimeUnit::SECONDS);
                            Assert::assertEquals(0, lock->getCount()); // should be 0 because countDown is called by
                            // observer
                        }

                        VirtualReaderEventTest::ReaderObserverAnonymousInnerClass5::ReaderObserverAnonymousInnerClass5(std::shared_ptr<VirtualReaderEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock) {
                            this->outerInstance = outerInstance;
                            this->lock = lock;
                        }

                        void VirtualReaderEventTest::ReaderObserverAnonymousInnerClass5::update(std::shared_ptr<ReaderEvent> event_Renamed) {
                            Assert::assertEquals(event_Renamed->getReaderName(), outerInstance->nativeReader->getName());
                            Assert::assertEquals(event_Renamed->getPluginName(), StubPlugin::getInstance()->getName());

                            // an SE_INSERTED event is thrown
                            Assert::assertEquals(ReaderEvent::EventType::SE_INSERTED, event_Renamed->getEventType());

                            // card has not match
                            Assert::assertFalse(event_Renamed->getDefaultSelectionResponse()->getSelectionSeResponseSet()->getSingleResponse()->getSelectionStatus()->hasMatched());

                            lock->countDown(); // should be called
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testATR() throws InterruptedException
                        void VirtualReaderEventTest::testATR() throw(InterruptedException) {

                            // CountDown lock
                            std::shared_ptr<CountDownLatch> * const lock = std::make_shared<CountDownLatch>(1);

                            // add observer
                            virtualReader->addObserver(std::make_shared<ReaderObserverAnonymousInnerClass6>(shared_from_this(), lock));

                            // wait 1 second
                            logger->debug("Wait 1 second before inserting SE");
                            delay(500);

                            // test
                            nativeReader->insertSe(StubReaderTest::hoplinkSE());

                            // lock thread for 2 seconds max to wait for the event
                            lock->await(5, TimeUnit::SECONDS);

                        }

                        VirtualReaderEventTest::ReaderObserverAnonymousInnerClass6::ReaderObserverAnonymousInnerClass6(std::shared_ptr<VirtualReaderEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock) {
                            this->outerInstance = outerInstance;
                            this->lock = lock;
                        }

                        void VirtualReaderEventTest::ReaderObserverAnonymousInnerClass6::update(std::shared_ptr<ReaderEvent> event_Renamed) {

                            Assert::assertEquals(ReaderEvent::EventType::SE_INSERTED, event_Renamed->getEventType());

                            std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>(outerInstance->virtualReader);
                            std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>("3B.*", ChannelState::KEEP_OPEN, Protocol::ANY, "Test ATR");

                            /* Prepare selector, ignore MatchingSe here */
                            seSelection->prepareSelection(seSelector);

                            try {
                                seSelection->processExplicitSelection();

                                std::shared_ptr<MatchingSe> matchingSe = seSelection->getSelectedSe();

                                Assert::assertNotNull(matchingSe);

                            }
                            catch (const KeypleReaderException &e) {
                                Assert::fail("Unexcepted exception");
                            }
                            // unlock thread
                            lock->countDown();
                        }
                    }
                }
            }
        }
    }
}
