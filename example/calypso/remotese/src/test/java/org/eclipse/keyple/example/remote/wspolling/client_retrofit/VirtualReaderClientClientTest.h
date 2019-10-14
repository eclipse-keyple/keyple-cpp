#pragma once

#include <string>
#include <memory>

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
            namespace example {
                namespace remote {
                    namespace wspolling {
                        namespace client_retrofit {

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                            import static org.junit.Assert.assertEquals;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                            import static org.junit.Assert.assertFalse;

                            class VirtualReaderClientClientTest : public std::enable_shared_from_this<VirtualReaderClientClientTest> {

                            private:
                                const std::string BASE_URL = "http://localhost:8081/";

                            public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void seTup()
                                virtual void seTup();

                                /**
                                 * Polling should failed after a timeout is raised on server
                                 *
                                 */
                                // @Test(expected = IOException.class)
                                virtual void testPolling() throw(IOException);

                                /**
                                 * Send a valid READER_CONNECT dto
                                 */
                                // @Test
                                virtual void testPostDto() throw(IOException);

                            };

                        }
                    }
                }
            }
        }
    }
}
