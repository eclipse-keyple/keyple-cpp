#pragma once

#include "VirtualReaderBaseTest.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
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
