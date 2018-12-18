#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ProxyReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class NoStackTraceThrowable; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleBaseException; } } } } }
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
                    namespace omapi {


//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                        import static org.powermock.api.mockito.PowerMockito.when;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                        using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                        using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                        using org::simalliance::openmobileapi::Reader;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(PowerMockRunner.class) public class AndroidOmapiReaderTest
                        class AndroidOmapiReaderTest : public std::enable_shared_from_this<AndroidOmapiReaderTest> {

                        public:
                            const std::string PLUGIN_NAME = "AndroidOmapiPlugin";
                            const std::string poAid = "A000000291A000000191";
                            const std::string poAidResponse = "6F25840BA000000291A00000019102A516BF0C13C70800000000C0E11FA653070A3C230C1410019000";

                            std::shared_ptr<Reader> omapiReader;
                            std::shared_ptr<ProxyReader> proxyReader;

                            // init before each test
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void SetUp() throws Exception
                            virtual void SetUp() throw(std::runtime_error);


                            /*
                             * TEST READERS
                             */

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getInstance() throws Exception
                            virtual void getInstance() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getName() throws Exception
                            virtual void getName() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isSEPresent() throws org.eclipse.keyple.seproxy.exception.NoStackTraceThrowable
                            virtual void isSEPresent() throw(NoStackTraceThrowable);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getParameters() throws org.eclipse.keyple.seproxy.exception.NoStackTraceThrowable
                            virtual void getParameters() throw(NoStackTraceThrowable);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setParameters() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException
                            virtual void setParameters() throw(KeypleBaseException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setParameter() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException
                            virtual void setParameter() throw(KeypleBaseException);

                            /*
                             * TRANSMIT
                             */

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitHoplinkSuccessfull() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException
                            virtual void transmitHoplinkSuccessfull() throw(KeypleBaseException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitNoAid() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                            virtual void transmitNoAid() throw(KeypleBaseException, IOException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitWrongProtocol() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException
                            virtual void transmitWrongProtocol() throw(KeypleBaseException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void transmitNotConnected() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                            virtual void transmitNotConnected() throw(KeypleBaseException, IOException);



                            /*
                             * HELPERS
                             */


                            virtual std::shared_ptr<Reader> mockReader() throw(IOException);

                            virtual std::shared_ptr<Reader> mockReaderWithNoAid() throw(KeypleBaseException, IOException);

                            virtual std::shared_ptr<SeRequestSet> getCalypsoRequestSample();


                        };

                    }
                }
            }
        }
    }
}
