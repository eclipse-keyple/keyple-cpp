#pragma once

#include <string>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ApduRequest; } } } } }

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
            namespace seproxy {
                namespace message {

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                    import static org.junit.Assert.*;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("PMD.SignatureDeclareThrowsException") @RunWith(MockitoJUnitRunner.class) public class ApduRequestTest
                    class ApduRequestTest : public std::enable_shared_from_this<ApduRequestTest> {
                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws Exception
                        virtual void setUp() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimpleAPDURequest()
                        virtual void testSimpleAPDURequest();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testAPDURequest()
                        virtual void testAPDURequest();



                        /*
                         * HELPERS
                         */

                        static std::shared_ptr<ApduRequest> getApduSample();

                        static std::vector<char> getACommand();

                        static std::shared_ptr<Set<Integer>> getASuccessFulStatusCode();

                        static std::string getAName();

                    };

                }
            }
        }
    }
}
