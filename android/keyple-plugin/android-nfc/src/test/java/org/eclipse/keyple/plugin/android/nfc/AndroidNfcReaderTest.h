#pragma once

#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace android { namespace nfc { class AndroidNfcReader; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace android { namespace nfc { class TagProxy; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleBaseException; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace @event { class ReaderEvent; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SeRequestSet; } } } } }

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
                namespace android {
                    namespace nfc {

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                        import static junit.framework.Assert.assertEquals;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                        import static org.junit.Assert.assertArrayEquals;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                        import static org.mockito.Mockito.doThrow;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                        import static org.mockito.Mockito.when;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                        using android::app::Activity;
                        using android::content::Intent;
                        using android::nfc::NfcAdapter;
                        using android::nfc::Tag;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(PowerMockRunner.class) @PrepareForTest({TagProxy.class, NfcAdapter.class}) public class AndroidNfcReaderTest
                        class AndroidNfcReaderTest : public std::enable_shared_from_this<AndroidNfcReaderTest> {

                        public:
                            std::shared_ptr<AndroidNfcReader> reader;
                            std::shared_ptr<NfcAdapter> nfcAdapter;
                            std::shared_ptr<Tag> tag;
                            std::shared_ptr<TagProxy> tagProxy;
                            std::shared_ptr<Intent> intent;
                            std::shared_ptr<Activity> activity;

                            // init before each test
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void SetUp() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                            virtual void SetUp() throw(KeypleReaderException);


                            /*
                             * TEST HIGH LEVEL METHOD TRANSMIT
                             */

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitSuccessfull() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                            virtual void transmitSuccessfull() throw(KeypleBaseException, IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitWrongProtocols() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                            virtual void transmitWrongProtocols() throw(KeypleBaseException, IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitWrongProtocol2() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                            virtual void transmitWrongProtocol2() throw(KeypleBaseException, IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void transmitCardNotConnected() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                            virtual void transmitCardNotConnected() throw(KeypleBaseException, IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitUnkownCard() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                            virtual void transmitUnkownCard() throw(KeypleBaseException, IOException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitUnknownApplication() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                            virtual void transmitUnknownApplication() throw(KeypleBaseException, IOException);

                            /*
                             * Test PUBLIC methods
                             */

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void setUnknownParameter() throws IllegalArgumentException
                            virtual void setUnknownParameter() throw(std::invalid_argument);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setCorrectParameter() throws IllegalArgumentException
                            virtual void setCorrectParameter() throw(std::invalid_argument);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void setUnCorrectParameter() throws IllegalArgumentException
                            virtual void setUnCorrectParameter() throw(std::invalid_argument);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void setUnCorrectParameter2() throws IllegalArgumentException
                            virtual void setUnCorrectParameter2() throw(std::invalid_argument);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void setUnCorrectParameter3() throws IllegalArgumentException
                            virtual void setUnCorrectParameter3() throw(std::invalid_argument);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setIsoDepProtocol()
                            virtual void setIsoDepProtocol();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setMifareProtocol()
                            virtual void setMifareProtocol();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setMifareULProtocol()
                            virtual void setMifareULProtocol();


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void insertEvent()
                            virtual void insertEvent();

                        private:
                            class ReaderObserverAnonymousInnerClass : public std::enable_shared_from_this<ReaderObserverAnonymousInnerClass>, public ObservableReader::ReaderObserver {
                            private:
                                std::shared_ptr<AndroidNfcReaderTest> outerInstance;

                            public:
                                ReaderObserverAnonymousInnerClass(std::shared_ptr<AndroidNfcReaderTest> outerInstance);

                                void update(std::shared_ptr<ReaderEvent> event_Renamed);
                            };

                        public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isConnected()
                            virtual void isConnected();



                            /*
                             * Test internal methods
                             */


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getATR()
                            virtual void getATR();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isPhysicalChannelOpen()
                            virtual void isPhysicalChannelOpen();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void openPhysicalChannelSuccess() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                            virtual void openPhysicalChannelSuccess() throw(KeypleReaderException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void openPhysicalChannelError() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                            virtual void openPhysicalChannelError() throw(KeypleBaseException, IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void closePhysicalChannelSuccess() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                            virtual void closePhysicalChannelSuccess() throw(KeypleReaderException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void closePhysicalChannelError() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                            virtual void closePhysicalChannelError() throw(KeypleBaseException, IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitAPDUSuccess() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                            virtual void transmitAPDUSuccess() throw(KeypleBaseException, IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void transmitAPDUError() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                            virtual void transmitAPDUError() throw(KeypleBaseException, IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void protocolFlagMatchesTrue() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                            virtual void protocolFlagMatchesTrue() throw(KeypleBaseException, IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void processIntent()
                            virtual void processIntent();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void printTagId()
                            virtual void printTagId();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void enableReaderMode()
                            virtual void enableReaderMode();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void disableReaderMode()
                            virtual void disableReaderMode();


                            /*
                             * Helper method
                             */

                        private:
                            void insertSe();

                            std::shared_ptr<SeRequestSet> getRequestIsoDepSetSample();
                        };

                    }
                }
            }
        }
    }
}
