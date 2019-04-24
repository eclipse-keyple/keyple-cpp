/* Core */
#include "ReaderEvent_Import.h"
#include "KeypleReaderException.h"

/* PC/SC plugin */
#include "PcscReaderActualTest.h"
#include "PcscPlugin.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace pcsc {
                    using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using AbstractObservableReader = org::eclipse::keyple::seproxy::plugin::AbstractObservableReader;
                    using Observable = org::eclipse::keyple::util::Observable;
                    using org::junit::Assert;
                    using org::junit::Ignore;
                    using org::junit::Test;

                    PcscReaderActualTest::MyReaderObserver::MyReaderObserver(std::shared_ptr<PcscReaderActualTest> outerInstance) : outerInstance(outerInstance) {
                    }

                    void PcscReaderActualTest::MyReaderObserver::update(std::shared_ptr<ReaderEvent> event_Renamed) {
                        lastThread = Thread::currentThread();
                        std::cout << "Observer: " << event_Renamed << " (from thread" << Thread::currentThread().getName() << ")" << std::endl;
                        if (event_Renamed->getEventType() == ReaderEvent::EventType::SE_INSERTED) {
//JAVA TO C++ CONVERTER TODO TASK: Multithread locking is not converted to native C++ unless you choose one of the options on the 'Modern C++ Options' dialog:
                            synchronized(shared_from_this()) {
                                notify(); // It's the standard java notify, nothing to do with *our* notify
                            }
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Ignore @Test public void testActual() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                    void PcscReaderActualTest::testActual() throw(KeypleReaderException, InterruptedException) {
                        std::shared_ptr<PcscPlugin> plugin = PcscPlugin::getInstance()->setLogging(true);

                        std::shared_ptr<MyReaderObserver> * const observer = std::make_shared<MyReaderObserver>(shared_from_this());
                        for (auto reader : plugin->getReaders()) {
                            reader->addObserver(observer);
                        }

                        // We wait to see if the thread management works correctly (thread is created here)
                        std::cout << "Waiting for card insertion (1/3)... " << std::endl;
//JAVA TO C++ CONVERTER TODO TASK: Multithread locking is not converted to native C++ unless you choose one of the options on the 'Modern C++ Options' dialog:
                        synchronized(observer) {
                            observer->wait();
                        }
                        std::cout << "OK" << std::endl;


                        // And then one more time to make sure we can do it twice
                        std::cout << "Waiting for card insertion (2/3)..." << std::endl;
//JAVA TO C++ CONVERTER TODO TASK: Multithread locking is not converted to native C++ unless you choose one of the options on the 'Modern C++ Options' dialog:
                        synchronized(observer) {
                            observer->wait();
                        }
                        std::cout << "OK" << std::endl;

                        // We look at the thread that was used
                        std::shared_ptr<Thread> firstThread = observer->lastThread;
                        std::cout << "First thread: " << firstThread << std::endl;

                        // Remove the observer from the observable (thread disappears)
                        for (auto reader : plugin->getReaders()) {
                            reader->removeObserver(observer);
                        }

                        // Re-add it (thread is created)
                        for (auto reader : plugin->getReaders()) {
                            reader->addObserver(observer);
                        }

                        // Wait for the card event
                        std::cout << "Waiting for card insertion (3/3)..." << std::endl;
//JAVA TO C++ CONVERTER TODO TASK: Multithread locking is not converted to native C++ unless you choose one of the options on the 'Modern C++ Options' dialog:
                        synchronized(observer) {
                            observer->wait();
                        }

                        // We look at the second thread that was used
                        std::shared_ptr<Thread> secondThread = observer->lastThread;

                        // Making sure it's not the same
                        Assert::assertNotEquals(firstThread, secondThread);

                        Assert::assertEquals(secondThread->getState(), Thread::State::RUNNABLE);

                        // Now if things went fast enough the first thread (which consumes the same PCSC resources)
                        // isn't dead yet.
                        std::cout << "Waiting for first thread..." << std::endl;
                        firstThread->join();
                        std::cout << "Done !" << std::endl;
                        std::cout << "Thread " << firstThread->getName() << " is now " << firstThread->getState() << " !" << std::endl;

                        // Remove the observer from the observable (thread disappears)
                        for (auto reader : plugin->getReaders()) {
                            reader->removeObserver(observer);
                        }
                        std::cout << "Waiting for last thread..." << std::endl;
                        secondThread->join();
                        std::cout << "Done !" << std::endl;
                    }
                }
            }
        }
    }
}
