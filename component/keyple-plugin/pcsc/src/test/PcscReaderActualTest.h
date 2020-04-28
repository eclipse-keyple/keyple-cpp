#pragma once

#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/util/Observable.h"
#include <iostream>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ReaderEvent; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }

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
                namespace pcsc {

                    using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using Observable = org::eclipse::keyple::util::Observable;

                    class PcscReaderActualTest : public std::enable_shared_from_this<PcscReaderActualTest> {


                    public:
                        class MyReaderObserver : public std::enable_shared_from_this<MyReaderObserver>, public Observable::Observer<std::shared_ptr<ReaderEvent>> {

                                        private:
                                            std::shared_ptr<PcscReaderActualTest> outerInstance;

                                        public:
                                            MyReaderObserver(std::shared_ptr<PcscReaderActualTest> outerInstance);


                        private:
                            std::shared_ptr<Thread> lastThread;

                            /*
                             * @Override public void notify(ReaderEvent event) { lastThread = Thread.currentThread();
                             * System.out.println("Observer: " + event + " (from thread" +
                             * Thread.currentThread().getName() + ")"); if (event == ReaderEventSE_INSERTED) {
                             * synchronized (this) { notify(); // It's the standard java notify, nothing to do with
                             * *our* notify } } }
                             */

                        public:
                            void update(std::shared_ptr<ReaderEvent> event_Renamed) override;
                        };

                        /**
                         * This test registers/deregisters on an {@link AbstractObservableReader} twice. This allows to
                         * verify we create and dispose threads correctly.
                         * 
                         * @throws KeypleReaderException
                         * @throws InterruptedException
                         */
                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Ignore @Test public void testActual() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                        virtual void testActual() throw(KeypleReaderException, InterruptedException);
                    };

                }
            }
        }
    }
}
