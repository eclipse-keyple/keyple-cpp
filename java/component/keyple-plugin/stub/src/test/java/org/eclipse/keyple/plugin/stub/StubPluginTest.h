#pragma once

#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservablePlugin.h"
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace stub { class StubPlugin; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class PluginEvent; } } } } }

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
                namespace stub {


                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using namespace org::junit;
                    using org::slf4j::Logger;
                    using org::slf4j::LoggerFactory;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(MockitoJUnitRunner.class) @FixMethodOrder(MethodSorters.NAME_ASCENDING) public class StubPluginTest
                    class StubPluginTest : public std::enable_shared_from_this<StubPluginTest> {

                    public:
                        std::shared_ptr<StubPlugin> stubPlugin;
                        std::shared_ptr<Logger> logger = LoggerFactory::getLogger(StubPluginTest::typeid);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void setUp() throw(InterruptedException, KeypleReaderException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws InterruptedException
                        virtual void tearDown() throw(InterruptedException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testA_PlugReaders() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void testA_PlugReaders() throw(InterruptedException, KeypleReaderException);

                    private:
                        class PluginObserverAnonymousInnerClass : public std::enable_shared_from_this<PluginObserverAnonymousInnerClass>, public ObservablePlugin::PluginObserver {
                        private:
                            std::shared_ptr<StubPluginTest> outerInstance;

                        public:
                            PluginObserverAnonymousInnerClass(std::shared_ptr<StubPluginTest> outerInstance);

                            bool first = false;

                            void update(std::shared_ptr<PluginEvent> event_Renamed);
                        };

                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testB_UnplugReaders() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void testB_UnplugReaders() throw(InterruptedException, KeypleReaderException);

                    private:
                        class PluginObserverAnonymousInnerClass2 : public std::enable_shared_from_this<PluginObserverAnonymousInnerClass2>, public ObservablePlugin::PluginObserver {
                        private:
                            std::shared_ptr<StubPluginTest> outerInstance;

                        public:
                            PluginObserverAnonymousInnerClass2(std::shared_ptr<StubPluginTest> outerInstance);

                            bool first = false;

                            void update(std::shared_ptr<PluginEvent> event_Renamed);
                        };

                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testC_PlugSameReaderTwice() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void testC_PlugSameReaderTwice() throw(InterruptedException, KeypleReaderException);

                    private:
                        class PluginObserverAnonymousInnerClass3 : public std::enable_shared_from_this<PluginObserverAnonymousInnerClass3>, public ObservablePlugin::PluginObserver {
                        private:
                            std::shared_ptr<StubPluginTest> outerInstance;

                        public:
                            PluginObserverAnonymousInnerClass3(std::shared_ptr<StubPluginTest> outerInstance);

                            void update(std::shared_ptr<PluginEvent> event_Renamed);
                        };

                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testD_GetName()
                        virtual void testD_GetName();
                    };

                }
            }
        }
    }
}
