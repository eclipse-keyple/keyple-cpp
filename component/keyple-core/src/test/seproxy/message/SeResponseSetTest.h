#pragma once

#include <vector>
#include <stdexcept>
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
            namespace seproxy {
                namespace message {

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                    import static org.junit.Assert.*;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings("PMD.SignatureDeclareThrowsException") @RunWith(MockitoJUnitRunner.class) public class SeResponseSetTest
                    class SeResponseSetTest : public std::enable_shared_from_this<SeResponseSetTest> {
                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getSingleResponse() throws Exception
                        virtual void getSingleResponse() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalStateException.class) public void getSingleResponseFail() throws Exception
                        virtual void getSingleResponseFail() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getResponses() throws Exception
                        virtual void getResponses() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void toStringTest() throws Exception
                        virtual void toStringTest() throw(std::runtime_error);
                    };

                }
            }
        }
    }
}
