
#pragma once

#include "BaseStubTest.h"
#include "StubPlugin.h"
#include "StubReader.h"
#include "ObservablePlugin.h"
#include "CountDownLatch.h"
#include "StubSecureElement.h"
#include "KeypleReaderException.h"
#include "PluginEvent.h"
#include "SeSelection.h"
#include "ChannelState.h"
#include "AbstractSeSelectionRequest.h"
#include "ByteArrayUtil.h"
#include "KeypleIOReaderException.h"
#include "AbstractMatchingSe.h"
#include "StubProtocolSetting.h"
#include "PoClass.h"
#include "ReadRecordsCmdBuild.h"
#include "IncreaseCmdBuild.h"
#include "KeypleChannelStateException.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <memory>
#include <typeinfo>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

/* Common */
#include "LoggerFactory.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace keyple { namespace plugin { namespace stub { class StubReader; } } }
namespace keyple { namespace plugin { namespace stub { class StubPlugin; } } }
namespace keyple { namespace plugin { namespace stub { class StubSecureElement; } } }

namespace keyple { namespace core { namespace seproxy { namespace exception { class KeypleReaderException; } } } }
namespace keyple { namespace core { namespace seproxy { namespace event { class PluginEvent; } } } }
namespace keyple { namespace core { namespace seproxy { class SeReader; } } }
namespace keyple { namespace core { namespace seproxy { namespace event { class ReaderEvent; } } } }
namespace keyple { namespace core { namespace seproxy { namespace event { class ObservableReader; } } } }
namespace keyple { namespace core { namespace seproxy { namespace event { class ReaderObserver; } } } }
namespace keyple { namespace core { namespace seproxy { namespace exception { class KeypleIOReaderException; } } } }
namespace keyple { namespace core { namespace seproxy { namespace exception { class KeypleChannelStateException; } } } }
namespace keyple { namespace core { namespace seproxy { namespace event { class ObservablePlugin; } } } }

namespace keyple { namespace util { class CountDownLatch; } }

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
                    using StubPlugin = keyple::plugin::stub::StubPlugin;
                    using StubReader = keyple::plugin::stub::StubReader;
                    using PoClass = keyple::calypso::command::PoClass;
                    using IncreaseCmdBuild = keyple::calypso::command::po::builder::IncreaseCmdBuild;
                    using ReadRecordsCmdBuild = keyple::calypso::command::po::builder::ReadRecordsCmdBuild;
                    using ChannelState = keyple::core::seproxy::ChannelState;
                    using SeSelector = keyple::core::seproxy::SeSelector;
                    using KeypleChannelStateException = keyple::core::seproxy::exception::KeypleChannelStateException;
                    using KeypleIOReaderException = keyple::core::seproxy::exception::KeypleIOReaderException;
                    using KeypleReaderException = keyple::core::seproxy::exception::KeypleReaderException;
                    using namespace keyple::core::seproxy::message;
                    using Protocol = keyple::core::seproxy::protocol::SeProtocol;
                    using SeProtocolSetting = keyple::core::seproxy::protocol::SeProtocolSetting;
                    using MatchingSe = keyple::core::selection::AbstractMatchingSe;
                    using SeSelection = keyple::core::selection::SeSelection;
                    using AbstractSeSelectionRequest = keyple::core::selection::AbstractSeSelectionRequest;
                    using ByteArrayUtils = keyple::core::util::ByteArrayUtil;
                    using Logger                = keyple::common::Logger;
                    using LoggerFactory         = keyple::common::LoggerFactory;

                    //using org::junit::runner::RunWith;
                    //using org::junit::runners::MethodSorters;
                    //using org::mockito::junit::MockitoJUnitRunner;

                    using SeReader              = keyple::core::seproxy::SeReader;
                    using ObservablePlugin      = keyple::core::seproxy::event::ObservablePlugin;
                    using ObservableReader      = keyple::core::seproxy::event::ObservableReader;
                    using PluginEvent           = keyple::core::seproxy::event::PluginEvent;
                    using ReaderEvent           = keyple::core::seproxy::event::ReaderEvent;

                    using namespace testing::gtest;


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("PMD.SignatureDeclareThrowsException") @RunWith(MockitoJUnitRunner.class) @FixMethodOrder(MethodSorters.NAME_ASCENDING) public class StubReaderTest
                    class StubReaderTest : public std::enable_shared_from_this<StubReaderTest>, public BaseStubTest, public testing::Test
                    {

                    public:
                        std::shared_ptr<StubReader> reader;

                        std::shared_ptr<StubReader> spyReader;

                        std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(StubReaderTest));

                        // init before each test
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void SetUp() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void SetUp();

                    private:
                        class PluginObserverAnonymousInnerClass : public std::enable_shared_from_this<PluginObserverAnonymousInnerClass>, public ObservablePlugin::PluginObserver
                        {
                        private:
                            std::shared_ptr<StubReaderTest> outerInstance;

                        public:
                            PluginObserverAnonymousInnerClass(std::shared_ptr<StubReaderTest> outerInstance);

                            void update(std::shared_ptr<PluginEvent> event_Renamed);
                        };

                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws InterruptedException, org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void tearDown();


                        static void selectSe(std::shared_ptr<SeReader> reader);

                        /*
                         * TRANSMIT
                         */


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsert() throws InterruptedException
                        virtual void testInsert();

                    private:
                        class ReaderObserverAnonymousInnerClass : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass>, public ObservableReader::ReaderObserver
                        {
                        private:
                            std::shared_ptr<StubReaderTest> outerInstance;

                            std::shared_ptr<CountDownLatch> lock;

                        public:
                            ReaderObserverAnonymousInnerClass(std::shared_ptr<StubReaderTest> outerInstance, std::shared_ptr<CountDownLatch> lock);

                            void update(std::shared_ptr<ReaderEvent> event_Renamed);
                        };

                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsertMatchingSe() throws InterruptedException
                        virtual void testInsertMatchingSe();

                    private:
                        class ReaderObserverAnonymousInnerClass2 : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass2>, public ObservableReader::ReaderObserver {
                        private:
                            std::shared_ptr<StubReaderTest> outerInstance;

                            std::shared_ptr<CountDownLatch> lock;
                            std::string poAid;

                        public:
                            ReaderObserverAnonymousInnerClass2(std::shared_ptr<StubReaderTest> outerInstance, std::shared_ptr<CountDownLatch> lock, const std::string &poAid);

                            void update(std::shared_ptr<ReaderEvent> event_Renamed);
                        };


                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsertNotMatching_MatchedOnly() throws InterruptedException
                        virtual void testInsertNotMatching_MatchedOnly();

                    private:
                        class ReaderObserverAnonymousInnerClass3 : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass3>, public ObservableReader::ReaderObserver
                        {
                        private:
                            std::shared_ptr<StubReaderTest> outerInstance;

                            std::shared_ptr<CountDownLatch> lock;

                        public:
                            ReaderObserverAnonymousInnerClass3(std::shared_ptr<StubReaderTest> outerInstance, std::shared_ptr<CountDownLatch> lock);

                            void update(std::shared_ptr<ReaderEvent> event_Renamed);
                        };

                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testInsertNotMatching_Always() throws InterruptedException
                        virtual void testInsertNotMatching_Always();

                    private:
                        class ReaderObserverAnonymousInnerClass4 : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass4>, public ObservableReader::ReaderObserver {
                        private:
                            std::shared_ptr<StubReaderTest> outerInstance;

                            std::shared_ptr<CountDownLatch> lock;

                        public:
                            ReaderObserverAnonymousInnerClass4(std::shared_ptr<StubReaderTest> outerInstance, std::shared_ptr<CountDownLatch> lock);

                            void update(std::shared_ptr<ReaderEvent> event_Renamed);
                        };

                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testATR() throws InterruptedException
                        virtual void testATR();

                    private:
                        class ReaderObserverAnonymousInnerClass5 : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass5>, public ObservableReader::ReaderObserver {
                        private:
                            std::shared_ptr<StubReaderTest> outerInstance;

                            std::shared_ptr<CountDownLatch> lock;

                        public:
                            ReaderObserverAnonymousInnerClass5(std::shared_ptr<StubReaderTest> outerInstance, std::shared_ptr<CountDownLatch> lock);

                            void update(std::shared_ptr<ReaderEvent> event_Renamed);
                        };


                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void transmit_Hoplink_null() throws Exception
                        virtual void transmit_Hoplink_null();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_Hoplink_Successful() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                        virtual void transmit_Hoplink_Successful();


                        // @Test
                        // public void transmit_null_Selection() throws KeypleReaderException {
                        // // init SE
                        // // no SE
                        //
                        // // init request
                        // SeRequestSet seRequest = getRequestIsoDepSetSample();
                        //
                        // // add Protocol flag
                        // reader.addSeProtocolSetting(
                        // new SeProtocolSetting(StubProtocolSetting.SETTING_PROTOCOL_ISO14443_4));
                        //
                        // // test
                        // SeResponseSet resp = reader.transmit(seRequest);
                        //
                        // Assert.assertNull(resp.getSingleResponse());
                        // }


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void transmit_no_response() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                        virtual void transmit_no_response();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_0() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                        virtual void transmit_partial_response_set_0();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_1() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                        virtual void transmit_partial_response_set_1();


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_2() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                        virtual void transmit_partial_response_set_2();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_3() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                        virtual void transmit_partial_response_set_3();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_0() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                        virtual void transmit_partial_response_0();


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_1() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                        virtual void transmit_partial_response_1();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_2() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                        virtual void transmit_partial_response_2();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_3() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, InterruptedException
                        virtual void transmit_partial_response_3();


                        /*
                         * NAME and PARAMETERS
                         */

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetName()
                        virtual void testGetName();

                        // Set wrong parameter
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void testSetWrongParameter() throws Exception
                        virtual void testSetWrongParameter();

                        // Set wrong parameters
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void testSetWrongParameters() throws Exception
                        virtual void testSetWrongParameters();

                        // Set correct parameters
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSetParameters() throws Exception
                        virtual void testSetParameters();


                        /**
                         * AbstractObservableReader methods test
                         *
                         * @throws Exception
                         */

                    private:
                        class ReaderObserverAnonymousInnerClass6 : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass6>, public ObservableReader::ReaderObserver {
                        public:
                            void update(std::shared_ptr<ReaderEvent> readerEvent);
                        };

                    private:
                        class ReaderObserverAnonymousInnerClass7 : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass7>, public ObservableReader::ReaderObserver {
                        public:
                            void update(std::shared_ptr<ReaderEvent> readerEvent);
                        };

                        std::shared_ptr<ObservableReader::ReaderObserver> obs1 = std::make_shared<ReaderObserverAnonymousInnerClass6>();

                        std::shared_ptr<ObservableReader::ReaderObserver> obs2 = std::make_shared<ReaderObserverAnonymousInnerClass7>();

                        /*
                         * HELPER METHODS
                         */


                    public:
                        static std::shared_ptr<SeRequestSet> getRequestIsoDepSetSample();

                        /*
                         * No Response: increase command is not defined in the StubSE
                         *
                         * An Exception will be thrown.
                         */
                        static std::shared_ptr<SeRequestSet> getNoResponseRequest();

                        /*
                         * Partial response set: multiple read records commands, one is not defined in the StubSE
                         *
                         * An Exception will be thrown.
                         */
                        static std::shared_ptr<SeRequestSet> getPartialRequestSet(int scenario);

                        /*
                         * Partial response: multiple read records commands, one is not defined in the StubSE
                         *
                         * An Exception will be thrown.
                         */
                        static std::shared_ptr<SeRequest> getPartialRequest(int scenario);

                        static std::shared_ptr<StubSecureElement> hoplinkSE();

                    private:
                        class StubSecureElementAnonymousInnerClass : public std::enable_shared_from_this<StubSecureElementAnonymousInnerClass>, public StubSecureElement
                        {

                        public:
                            std::vector<char> processApdu(std::vector<char> &apduIn) override;

                            std::vector<char> getATR() override;

                            std::string getSeProcotol() override;

                    //protected:
                    //        std::shared_ptr<StubSecureElementAnonymousInnerClass> shared_from_this()
                    //        {
                    //            return std::static_pointer_cast<StubSecureElementAnonymousInnerClass>(StubSecureElement::shared_from_this());
                    //        }
                        };

                    public:
                        static std::shared_ptr<StubSecureElement> noApduResponseSE();

                    private:
                        class StubSecureElementAnonymousInnerClass2 : public StubSecureElement
                        {

                        public:
                            std::vector<char> processApdu(std::vector<char> &apduIn) override;

                            std::vector<char> getATR() override;

                            std::string getSeProcotol() override;

                        //protected:
                        //    std::shared_ptr<StubSecureElementAnonymousInnerClass2> shared_from_this() {
                        //        return std::static_pointer_cast<StubSecureElementAnonymousInnerClass2>(StubSecureElement::shared_from_this());
                        //    }
                        };

                    public:
                        static std::shared_ptr<StubSecureElement> partialSE();

                    private:
                        class StubSecureElementAnonymousInnerClass3 : public StubSecureElement {

                        public:
                            std::vector<char> processApdu(std::vector<char> &apduIn) override;

                            std::vector<char> getATR() override;

                            std::string getSeProcotol() override;

protected:
                            std::shared_ptr<StubSecureElementAnonymousInnerClass3> shared_from_this() {
                                return std::static_pointer_cast<StubSecureElementAnonymousInnerClass3>(StubSecureElement::shared_from_this());
                            }
                        };

                    public:
                        static std::shared_ptr<StubSecureElement> getSENoconnection();

                    private:
                        class StubSecureElementAnonymousInnerClass4 : public StubSecureElement {
                        public:
                            std::vector<char> getATR() override;

                            bool isPhysicalChannelOpen() override;

                            // override methods to fail open connection
                            void openPhysicalChannel() override;

                            void closePhysicalChannel() override;

                            std::vector<char> processApdu(std::vector<char> &apduIn) override;

                            std::string getSeProcotol() override;

protected:
                            std::shared_ptr<StubSecureElementAnonymousInnerClass4> shared_from_this() {
                                return std::static_pointer_cast<StubSecureElementAnonymousInnerClass4>(StubSecureElement::shared_from_this());
                            }
                        };

                    public:
                        static std::shared_ptr<ApduRequest> getApduSample();
                    };

                }
            }
        }
    }
}
