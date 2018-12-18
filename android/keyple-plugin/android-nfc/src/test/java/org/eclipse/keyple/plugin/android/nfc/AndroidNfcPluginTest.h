#pragma once

#include <string>
#include <unordered_map>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace android { namespace nfc { class AndroidNfcPlugin; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleBaseException; } } } } }
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
//                        import static org.hamcrest.CoreMatchers.instanceOf;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                        import static org.junit.Assert.assertThat;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                        import static org.powermock.api.mockito.PowerMockito.when;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(PowerMockRunner.class) @PrepareForTest({AndroidNfcReader.class}) public class AndroidNfcPluginTest
                        class AndroidNfcPluginTest : public std::enable_shared_from_this<AndroidNfcPluginTest> {

                        public:
                            std::shared_ptr<AndroidNfcPlugin> plugin;

                            // init before each test
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void SetUp() throws java.io.IOException
                            virtual void SetUp() throw(IOException);



                            /*
                             * TEST PUBLIC METHODS
                             */


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getInstance() throws java.io.IOException
                            virtual void getInstance() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setParameters() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException
                            virtual void setParameters() throw(KeypleBaseException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getParameters() throws java.io.IOException
                            virtual void getParameters() throw(IOException);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setParameter() throws java.io.IOException
                            virtual void setParameter() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getReaders() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                            virtual void getReaders() throw(KeypleReaderException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getName() throws java.io.IOException
                            virtual void getName() throw(IOException);

                            /*
                             * TEST INTERNAL METHODS
                             */

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getNativeReader() throws java.io.IOException
                            virtual void getNativeReader() throw(IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getNativeReaders() throws java.io.IOException
                            virtual void getNativeReaders() throw(IOException);



                        };

                    }
                }
            }
        }
    }
}
