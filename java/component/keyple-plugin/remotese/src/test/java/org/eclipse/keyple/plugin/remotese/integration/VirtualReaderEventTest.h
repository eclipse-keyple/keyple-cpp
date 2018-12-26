#pragma once

#include "VirtualReaderBaseTest.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ReaderEvent; } } } } }

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
                namespace remotese {
                    namespace integration {

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                        import static org.eclipse.keyple.plugin.stub.StubReaderTest.hoplinkSE;
                        using namespace org::junit;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * Test Virtual Reader Service with stub plugin and hoplink SE
                         */
                        class VirtualReaderEventTest : public VirtualReaderBaseTest {

                        private:
                            static const std::shared_ptr<Logger> logger;


                            /*
                             * SE EVENTS
                             */


                            /**
                             * Test SE_INSERTED Reader Event throwing and catching
                             * 
                             * @throws Exception
                             */
                        public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsert() throws Exception
                            virtual void testInsert() throw(std::runtime_error);

                        private:
                            class ReaderObserverAnonymousInnerClass : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass>, public ObservableReader::ReaderObserver {
                            private:
                                std::shared_ptr<VirtualReaderEventTest> outerInstance;

                                std::shared_ptr<CountDownLatch> lock;

                            public:
                                ReaderObserverAnonymousInnerClass(std::shared_ptr<VirtualReaderEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock);

                                void update(std::shared_ptr<ReaderEvent> event_Renamed);
                            };


                            /**
                             * Test SE_REMOVED Reader Event throwing and catching
                             * 
                             * @throws Exception
                             */
                        public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testRemoveEvent() throws Exception
                            virtual void testRemoveEvent() throw(std::runtime_error);

                        private:
                            class ReaderObserverAnonymousInnerClass2 : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass2>, public ObservableReader::ReaderObserver {
                            private:
                                std::shared_ptr<VirtualReaderEventTest> outerInstance;

                                std::shared_ptr<CountDownLatch> lock;

                            public:
                                ReaderObserverAnonymousInnerClass2(std::shared_ptr<VirtualReaderEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock);

                                void update(std::shared_ptr<ReaderEvent> event_Renamed);
                            };


                        public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsertMatchingSe() throws InterruptedException
                            virtual void testInsertMatchingSe() throw(InterruptedException);

                        private:
                            class ReaderObserverAnonymousInnerClass3 : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass3>, public ObservableReader::ReaderObserver {
                            private:
                                std::shared_ptr<VirtualReaderEventTest> outerInstance;

                                std::shared_ptr<CountDownLatch> lock;
                                std::string poAid;

                            public:
                                ReaderObserverAnonymousInnerClass3(std::shared_ptr<VirtualReaderEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock, const std::string &poAid);

                                void update(std::shared_ptr<ReaderEvent> event_Renamed);
                            };


                        public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsertNotMatching_MatchedOnly() throws InterruptedException
                            virtual void testInsertNotMatching_MatchedOnly() throw(InterruptedException);

                        private:
                            class ReaderObserverAnonymousInnerClass4 : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass4>, public ObservableReader::ReaderObserver {
                            private:
                                std::shared_ptr<VirtualReaderEventTest> outerInstance;

                                std::shared_ptr<CountDownLatch> lock;

                            public:
                                ReaderObserverAnonymousInnerClass4(std::shared_ptr<VirtualReaderEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock);

                                void update(std::shared_ptr<ReaderEvent> event_Renamed);
                            };

                        public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsertNotMatching_Always() throws InterruptedException
                            virtual void testInsertNotMatching_Always() throw(InterruptedException);

                        private:
                            class ReaderObserverAnonymousInnerClass5 : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass5>, public ObservableReader::ReaderObserver {
                            private:
                                std::shared_ptr<VirtualReaderEventTest> outerInstance;

                                std::shared_ptr<CountDownLatch> lock;

                            public:
                                ReaderObserverAnonymousInnerClass5(std::shared_ptr<VirtualReaderEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock);

                                void update(std::shared_ptr<ReaderEvent> event_Renamed);
                            };

                        public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testATR() throws InterruptedException
                            virtual void testATR() throw(InterruptedException);

                        private:
                            class ReaderObserverAnonymousInnerClass6 : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass6>, public ObservableReader::ReaderObserver {
                            private:
                                std::shared_ptr<VirtualReaderEventTest> outerInstance;

                                std::shared_ptr<CountDownLatch> lock;

                            public:
                                ReaderObserverAnonymousInnerClass6(std::shared_ptr<VirtualReaderEventTest> outerInstance, std::shared_ptr<CountDownLatch> lock);

                                void update(std::shared_ptr<ReaderEvent> event_Renamed);
                            };



protected:
                            std::shared_ptr<VirtualReaderEventTest> shared_from_this() {
                                return std::static_pointer_cast<VirtualReaderEventTest>(VirtualReaderBaseTest::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
