#pragma once

#include <string>
#include <vector>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
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
                namespace android {
                    namespace nfc {

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                        import static junit.framework.Assert.assertEquals;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                        import static org.mockito.Mockito.when;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using org::junit::rules::ExpectedException;
                        using android::nfc::Tag;
                        using android::nfc::tech::IsoDep;
                        using android::nfc::tech::MifareClassic;
                        using android::nfc::tech::MifareUltralight;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(PowerMockRunner.class) @PrepareForTest({IsoDep.class, MifareUltralight.class, MifareClassic.class}) public class TagProxyTest
                        class TagProxyTest : public std::enable_shared_from_this<TagProxyTest> {
                        public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Rule public org.junit.rules.ExpectedException thrown = org.junit.rules.ExpectedException.none();
                            std::shared_ptr<ExpectedException> thrown = ExpectedException::none();

                            std::shared_ptr<Tag> tagIso;
                            std::shared_ptr<Tag> tagMifare;
                            std::shared_ptr<Tag> tagMifareUL;


                            std::shared_ptr<IsoDep> isoDep;
                            std::shared_ptr<MifareClassic> mifare;
                            std::shared_ptr<MifareUltralight> mifareUL;

                            // init before each test
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void SetUp() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                            virtual void SetUp() throw(KeypleReaderException);



                            /*
                             * PUBLIC METHODS
                             */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getTagProxyIsoDep() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                            virtual void getTagProxyIsoDep() throw(KeypleReaderException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getTagProxyMifareClassic() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                            virtual void getTagProxyMifareClassic() throw(KeypleReaderException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getTagProxyMifareUltralight() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                            virtual void getTagProxyMifareUltralight() throw(KeypleReaderException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void getTagProxyNull() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                            virtual void getTagProxyNull() throw(KeypleReaderException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void getTag() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                            virtual void getTag() throw(KeypleReaderException, IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void connect() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                            virtual void connect() throw(KeypleReaderException, IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void close() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                            virtual ~TagProxyTest();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void isConnected() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                            virtual void isConnected() throw(KeypleReaderException, IOException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void transceive() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                            virtual void transceive() throw(KeypleReaderException, IOException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void getATRMifare() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                            virtual void getATRMifare() throw(KeypleReaderException, IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void getATRIsodep() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                            virtual void getATRIsodep() throw(KeypleReaderException, IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void getATRMifareUL() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                            virtual void getATRMifareUL() throw(KeypleReaderException, IOException);


                            /*
                             * HELPERS
                             */

                            virtual void initIsoDep();

                            virtual void initMifare();

                            virtual void initMifareUL();



                        };

                    }
                }
            }
        }
    }
}
