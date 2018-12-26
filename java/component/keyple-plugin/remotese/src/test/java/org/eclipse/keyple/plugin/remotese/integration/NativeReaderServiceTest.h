#pragma once

#include <string>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace pluginse { class VirtualReaderService; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace stub { class StubReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace pluginse { class VirtualReader; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace remotese { namespace nativese { class NativeReaderServiceImpl; } } } } } }

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
//                        import static org.mockito.Mockito.doAnswer;
                        using NativeReaderServiceImpl = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderServiceImpl;
                        using VirtualReader = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader;
                        using VirtualReaderService = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReaderService;
                        using namespace org::eclipse::keyple::plugin::remotese::transport;
                        using StubReader = org::eclipse::keyple::plugin::stub::StubReader;
                        using namespace org::mockito;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(MockitoJUnitRunner.class) public class NativeReaderServiceTest
                        class NativeReaderServiceTest : public std::enable_shared_from_this<NativeReaderServiceTest> {

                        private:
                            static const std::shared_ptr<Logger> logger;

                            // Real objects
                        public:
                            std::shared_ptr<TransportFactory> factory;
                            std::shared_ptr<VirtualReaderService> virtualReaderService;

                            std::shared_ptr<StubReader> nativeReader;
                            std::shared_ptr<VirtualReader> virtualReader;

                            const std::string NATIVE_READER_NAME = "testStubReader";
                            const std::string CLIENT_NODE_ID = "testClientNodeId";


                            // Spy Object
                            std::shared_ptr<NativeReaderServiceImpl> nativeReaderSpy;



//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setTup() throws Exception
                            virtual void setTup() throw(std::runtime_error);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws Exception
                            virtual void tearDown() throw(std::runtime_error);



                            /*
                             * CONNECT METHOD
                             */


                            /**
                             * Connect successfully a reader
                             * 
                             * @throws Exception
                             */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOKConnect() throws Exception
                            virtual void testOKConnect() throw(std::runtime_error);

                            /**
                             * Connect error : reader already exists
                             * 
                             * @throws Exception
                             */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testKOConnectError() throws Exception
                            virtual void testKOConnectError() throw(std::runtime_error);

                            /**
                             * Connect error : impossible to send DTO
                             * 
                             * @throws Exception
                             */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = KeypleRemoteException.class) public void testKOConnectServerError() throws Exception
                            virtual void testKOConnectServerError() throw(std::runtime_error);

                            /*
                             * DISCONNECT METHOD
                             */

                            /**
                             * Disconnect successfully a reader
                             * 
                             * @throws Exception
                             */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOKConnectDisconnect() throws Exception
                            virtual void testOKConnectDisconnect() throw(std::runtime_error);


                            /**
                             * Disconnect Error : reader not connected
                             * 
                             * @throws Exception
                             */
                            // @Test
                            // public void testKODisconnectNotFoundError() throws Exception {
                            //
                            // // assert an exception will be contained into keypleDto response
                            // doAnswer(Integration.assertContainsException()).when(nativeReaderSpy)
                            // .onDTO(ArgumentMatchers.<TransportDto>any());
                            //
                            // // disconnect
                            // nativeReaderSpy.disconnectReader(nativeReader, CLIENT_NODE_ID);
                            // // should throw exception in master side KeypleNotFound
                            //
                            // }


                            /**
                             * Disconnect error : impossible to send DTO
                             * 
                             * @throws Exception
                             */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = KeypleRemoteException.class) public void testKODisconnectServerError() throws Exception
                            virtual void testKODisconnectServerError() throw(std::runtime_error);


                            /*
                             * HELPERS
                             */



                        };

                    }
                }
            }
        }
    }
}
