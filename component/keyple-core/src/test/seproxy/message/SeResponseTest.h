#pragma once

#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class SeResponse; } } } } }

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


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @SuppressWarnings({"PMD.TooManyMethods", "PMD.SignatureDeclareThrowsException"}) @RunWith(MockitoJUnitRunner.class) public class SeResponseTest
                    class SeResponseTest : public std::enable_shared_from_this<SeResponseTest> {
                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorSuccessfullResponseMatch() throws IllegalArgumentException
                        virtual void constructorSuccessfullResponseMatch() throw(std::invalid_argument);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorSuccessfullResponseNoMatch() throws IllegalArgumentException
                        virtual void constructorSuccessfullResponseNoMatch() throw(std::invalid_argument);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorATRNull() throws IllegalArgumentException
                        virtual void constructorATRNull() throw(std::invalid_argument);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorFCINull() throws IllegalArgumentException
                        virtual void constructorFCINull() throw(std::invalid_argument);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void constructorFCIAndATRNull() throws IllegalArgumentException
                        virtual void constructorFCIAndATRNull() throw(std::invalid_argument);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test() public void testEquals() throws Exception
                        virtual void testEquals() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test() public void testThisEquals() throws Exception
                        virtual void testThisEquals() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test() public void testNotEquals() throws Exception
                        virtual void testNotEquals() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test() public void testNotEqualsNull() throws Exception
                        virtual void testNotEqualsNull() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test() public void hashcode() throws Exception
                        virtual void hashcode() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test() public void hashcodeNull() throws Exception
                        virtual void hashcodeNull() throw(std::runtime_error);


                        /*
                         * HELPERS
                         */

                        static std::shared_ptr<SeResponse> getASeResponse() throw(std::invalid_argument);
                    };

                }
            }
        }
    }
}
