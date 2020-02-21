#pragma once

#include <unordered_set>
#include <vector>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class AnswerToReset; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ApduResponse; } } } } }

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
//ORIGINAL LINE: @SuppressWarnings("PMD.AvoidDuplicateLiterals") @RunWith(MockitoJUnitRunner.class) public class ApduResponseTest
                    class ApduResponseTest : public std::enable_shared_from_this<ApduResponseTest> {
                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp()
                        virtual void setUp();
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorSuccessFullResponse()
                        virtual void constructorSuccessFullResponse();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorSuccessFullResponseWithCustomCode()
                        virtual void constructorSuccessFullResponseWithCustomCode();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorFailResponse()
                        virtual void constructorFailResponse();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void constructorFailResponseWithCustomCode()
                        virtual void constructorFailResponseWithCustomCode();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isEqualsTest()
                        virtual void isEqualsTest();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isThisEquals()
                        virtual void isThisEquals();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isNotEquals()
                        virtual void isNotEquals();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isNotEqualsNull()
                        virtual void isNotEqualsNull();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hashcodeTest()
                        virtual void hashcodeTest();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void hashcodeNull()
                        virtual void hashcodeNull();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testToStringNull()
                        virtual void testToStringNull();

                        /*
                         * HELPERS
                         */


                        static std::shared_ptr<std::set<Integer>> getA9005CustomCode();

                        static std::shared_ptr<AnswerToReset> getAAtr();

                        static std::shared_ptr<ApduResponse> getAFCI();

                        static std::shared_ptr<ApduResponse> getSuccessfullResponse();

                        static std::vector<std::shared_ptr<ApduResponse>> getAListOfAPDUs();

                    };

                }
            }
        }
    }
}
