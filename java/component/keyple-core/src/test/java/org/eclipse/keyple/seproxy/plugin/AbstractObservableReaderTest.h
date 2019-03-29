#pragma once

#include "../../CoreBaseTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/plugin/AbstractLocalReader.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/protocol/TransmissionMode.h"
#include "../../../../../../../main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ReaderEvent; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ReaderObserver; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace plugin { class AbstractObservableReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace protocol { class SeProtocol; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { class SeSelector; } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SelectionStatus; } } } } }

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
            namespace seproxy {
                namespace plugin {


//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                    import static org.mockito.Mockito.doAnswer;
                    using CoreBaseTest = org::eclipse::keyple::CoreBaseTest;
                    using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                    using namespace org::eclipse::keyple::seproxy::exception;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;

                    /**
                     * Test methods linked to observability
                     */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(MockitoJUnitRunner.class) public class AbstractObservableReaderTest extends org.eclipse.keyple.CoreBaseTest
                    class AbstractObservableReaderTest : public CoreBaseTest {

                    private:
                        static const std::shared_ptr<Logger> logger;


                    public:
                        const std::string PLUGIN_NAME = "abstractObservablePluginTest";
                        const std::string READER_NAME = "abstractObservableReaderTest";

                        const std::shared_ptr<ObservableReader::ReaderObserver> obs1 = getObserver();
                        const std::shared_ptr<ObservableReader::ReaderObserver> obs2 = getObserver();

                        std::shared_ptr<AbstractObservableReader> spyReader;

                        std::shared_ptr<CountDownLatch> startObservationCall;
                        std::shared_ptr<CountDownLatch> stopObservationCall;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp()
                        virtual void setUp();

                        /*
                         * TESTS
                         */

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAddObserver()
                        virtual void testAddObserver();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRemoveObserver()
                        virtual void testRemoveObserver();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAddRemoveObserver()
                        virtual void testAddRemoveObserver();



                        /*
                         * HELPERS
                         */



                        virtual std::shared_ptr<AbstractObservableReader> getBlankAbstractObservableReader(const std::string &pluginName, const std::string &readerName);

                    private:
                        class AbstractLocalReaderAnonymousInnerClass : public AbstractLocalReader {
                        private:
                            std::shared_ptr<AbstractObservableReaderTest> outerInstance;

                        public:
                            AbstractLocalReaderAnonymousInnerClass(std::shared_ptr<AbstractObservableReaderTest> outerInstance, const std::string &pluginName, const std::string &readerName);


                        protected:
                            void startObservation() override;

                            void stopObservation() override;

                            void closePhysicalChannel() throw(KeypleChannelStateException) override;

                            bool isPhysicalChannelOpen() override;

                            std::vector<char> transmitApdu(std::vector<char> &apduIn) throw(KeypleIOReaderException) override;

                            bool protocolFlagMatches(std::shared_ptr<SeProtocol> protocolFlag) throw(KeypleReaderException) override;

                            bool checkSePresence() throw(NoStackTraceThrowable) override;

                            std::vector<char> getATR() override;

                            std::shared_ptr<SelectionStatus> openLogicalChannel(std::shared_ptr<SeSelector> selector) override;

                            void openPhysicalChannel() throw(KeypleChannelStateException) override;

                        public:
                            TransmissionMode getTransmissionMode() override;

                            std::unordered_map<std::string, std::string> getParameters() override;

                            void setParameter(const std::string &key, const std::string &value) throw(std::invalid_argument, KeypleBaseException) override;

protected:
                            std::shared_ptr<AbstractLocalReaderAnonymousInnerClass> shared_from_this() {
                                return std::static_pointer_cast<AbstractLocalReaderAnonymousInnerClass>(AbstractLocalReader::shared_from_this());
                            }
                        };



                    public:
                        virtual std::shared_ptr<ObservableReader::ReaderObserver> getObserver();

                    private:
                        class ReaderObserverAnonymousInnerClass : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass>, public ObservableReader::ReaderObserver {
                        private:
                            std::shared_ptr<AbstractObservableReaderTest> outerInstance;

                        public:
                            ReaderObserverAnonymousInnerClass(std::shared_ptr<AbstractObservableReaderTest> outerInstance);

                            void update(std::shared_ptr<ReaderEvent> readerEvent);
                        };


                    public:
                        virtual void initSpyReader();

                    private:
                        class AnswerAnonymousInnerClass : public Answer {
                        private:
                            std::shared_ptr<AbstractObservableReaderTest> outerInstance;

                        public:
                            AnswerAnonymousInnerClass(std::shared_ptr<AbstractObservableReaderTest> outerInstance);

                            std::shared_ptr<void> answer(std::shared_ptr<InvocationOnMock> invocation) throw(std::runtime_error) override;

protected:
                            std::shared_ptr<AnswerAnonymousInnerClass> shared_from_this() {
                                return std::static_pointer_cast<AnswerAnonymousInnerClass>(org.mockito.stubbing.Answer::shared_from_this());
                            }
                        };

                    private:
                        class AnswerAnonymousInnerClass2 : public Answer {
                        private:
                            std::shared_ptr<AbstractObservableReaderTest> outerInstance;

                        public:
                            AnswerAnonymousInnerClass2(std::shared_ptr<AbstractObservableReaderTest> outerInstance);

                            std::shared_ptr<void> answer(std::shared_ptr<InvocationOnMock> invocation) throw(std::runtime_error) override;

protected:
                            std::shared_ptr<AnswerAnonymousInnerClass2> shared_from_this() {
                                return std::static_pointer_cast<AnswerAnonymousInnerClass2>(org.mockito.stubbing.Answer::shared_from_this());
                            }
                        };



protected:
                        std::shared_ptr<AbstractObservableReaderTest> shared_from_this() {
                            return std::static_pointer_cast<AbstractObservableReaderTest>(org.eclipse.keyple.CoreBaseTest::shared_from_this());
                        }
                    };

                }
            }
        }
    }
}
