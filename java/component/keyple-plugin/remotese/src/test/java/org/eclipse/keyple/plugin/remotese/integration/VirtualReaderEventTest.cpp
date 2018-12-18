#include "VirtualReaderEventTest.h"
#include "../../../../../../../../../../stub/src/test/java/org/eclipse/keyple/plugin/stub/StubReaderTest.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "../../../../../../../../../../stub/src/main/java/org/eclipse/keyple/plugin/stub/StubPlugin.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace integration {
                        using StubPlugin = org::eclipse::keyple::plugin::stub::StubPlugin;
                        using StubReaderTest = org::eclipse::keyple::plugin::stub::StubReaderTest;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
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
                            delay(1000);

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
                    }
                }
            }
        }
    }
}
