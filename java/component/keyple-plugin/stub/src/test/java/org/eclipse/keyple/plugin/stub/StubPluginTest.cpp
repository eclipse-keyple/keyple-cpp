#include "StubPluginTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/plugin/stub/StubPlugin.h"
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
//ORIGINAL LINE: @Before public void setUp() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::setUp() throw(InterruptedException, KeypleReaderException) {
                        logger->info("setUp, assert stubplugin is empty");
                        stubPlugin = StubPlugin::getInstance(); // singleton

                        logger->info("Stubplugin readers size {}", stubPlugin->getReaders()->size());
                        Assert::assertEquals(0, stubPlugin->getReaders()->size());

                        logger->info("Stubplugin observers size {}", stubPlugin->countObservers());
                        Assert::assertEquals(0, stubPlugin->countObservers());

                        delay(100);
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws InterruptedException
                    void StubPluginTest::tearDown() throw(InterruptedException) {
                        stubPlugin = StubPlugin::getInstance(); // singleton
                        stubPlugin->clearObservers();
                        delay(100);

                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testA_PlugReaders() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testA_PlugReaders() throw(InterruptedException, KeypleReaderException) {

                        std::shared_ptr<ObservablePlugin::PluginObserver> connected_obs = std::make_shared<PluginObserverAnonymousInnerClass>(shared_from_this());

                        // add READER_CONNECTED assert observer
                        stubPlugin->addObserver(connected_obs);

                        // connect reader
                        stubPlugin->plugStubReader("testA_PlugReaders");

                        delay(200);
                        logger->debug("Stubplugin readers size {} ", stubPlugin->getReaders()->size());

                        assert(stubPlugin->getReaders()->size() == 1);

                        // clean
                        stubPlugin->unplugReader("testA_PlugReaders");
                        delay(200);
                        stubPlugin->removeObserver(connected_obs);
                        delay(200);
                    }

                    StubPluginTest::PluginObserverAnonymousInnerClass::PluginObserverAnonymousInnerClass(std::shared_ptr<StubPluginTest> outerInstance) {
                        this->outerInstance = outerInstance;
                        first = true;
                    }

                    void StubPluginTest::PluginObserverAnonymousInnerClass::update(std::shared_ptr<PluginEvent> event_Renamed) {
                        if (first) {
                            Assert::assertEquals(PluginEvent::EventType::READER_CONNECTED, event_Renamed->getEventType());
                            first = false;
                        }
                        else {
                            Assert::assertEquals(PluginEvent::EventType::READER_DISCONNECTED, event_Renamed->getEventType());
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testB_UnplugReaders() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testB_UnplugReaders() throw(InterruptedException, KeypleReaderException) {

                        std::shared_ptr<ObservablePlugin::PluginObserver> disconnected_obs = std::make_shared<PluginObserverAnonymousInnerClass2>(shared_from_this());

                        // add READER_CONNECTED assert observer
                        stubPlugin->addObserver(disconnected_obs);

                        // add a reader
                        stubPlugin->plugStubReader("testB_UnplugReaders");

                        // let the monitor thread work
                        delay(100);

                        logger->debug("Stubplugin readers size {} ", stubPlugin->getReaders()->size());
                        assert(stubPlugin->getReaders()->size() == 1);

                        stubPlugin->unplugReader("testB_UnplugReaders");

                        delay(100);

                        Assert::assertEquals(0, stubPlugin->getReaders()->size());

                        // clean
                        stubPlugin->removeObserver(disconnected_obs);
                    }

                    StubPluginTest::PluginObserverAnonymousInnerClass2::PluginObserverAnonymousInnerClass2(std::shared_ptr<StubPluginTest> outerInstance) {
                        this->outerInstance = outerInstance;
                        first = true;
                    }

                    void StubPluginTest::PluginObserverAnonymousInnerClass2::update(std::shared_ptr<PluginEvent> event_Renamed) {
                        if (first) {
                            Assert::assertEquals(PluginEvent::EventType::READER_CONNECTED, event_Renamed->getEventType());
                            first = false;
                        }
                        else {
                            Assert::assertEquals(PluginEvent::EventType::READER_DISCONNECTED, event_Renamed->getEventType());
                        }
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testC_PlugSameReaderTwice() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void StubPluginTest::testC_PlugSameReaderTwice() throw(InterruptedException, KeypleReaderException) {

                        std::shared_ptr<ObservablePlugin::PluginObserver> observer = std::make_shared<PluginObserverAnonymousInnerClass3>(shared_from_this());

                        // add observer to have the reader management done by the monitoring thread
                        stubPlugin->addObserver(observer);

                        stubPlugin->plugStubReader("testC_PlugSameReaderTwice");
                        stubPlugin->plugStubReader("testC_PlugSameReaderTwice");
                        logger->debug("Stubplugin readers size {} ", stubPlugin->getReaders()->size());

                        // let the monitor thread work
                        delay(100);

                        assert(stubPlugin->getReaders()->size() == 1);
                        stubPlugin->unplugReader("testC_PlugSameReaderTwice");

                        // let the monitor thread work
                        delay(100);

                        logger->debug("Stubplugin readers size {} ", stubPlugin->getReaders()->size());
                        assert(stubPlugin->getReaders()->size() == 0);
                    }

                    StubPluginTest::PluginObserverAnonymousInnerClass3::PluginObserverAnonymousInnerClass3(std::shared_ptr<StubPluginTest> outerInstance) {
                        this->outerInstance = outerInstance;
                    }

                    void StubPluginTest::PluginObserverAnonymousInnerClass3::update(std::shared_ptr<PluginEvent> event_Renamed) {
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testD_GetName()
                    void StubPluginTest::testD_GetName() {
                        assert(stubPlugin->getName() != "");
                    }
                }
            }
        }
    }
}
