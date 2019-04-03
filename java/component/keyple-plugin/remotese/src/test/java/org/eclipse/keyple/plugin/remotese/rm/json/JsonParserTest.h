#pragma once

#include <string>
#include <typeinfo>
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
            namespace plugin {
                namespace remotese {
                    namespace rm {
                        namespace json {

                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(MockitoJUnitRunner.class) public class JsonParserTest
                            class JsonParserTest : public std::enable_shared_from_this<JsonParserTest> {

                            private:
                                static const std::shared_ptr<Logger> logger;


                                /**
                                 * Test Serialization of Keyple Se Proxy Objects
                                 */

                            public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testHoplinkSeRequestSet()
                                virtual void testHoplinkSeRequestSet();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testCompleteSeRequestSet()
                                virtual void testCompleteSeRequestSet();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSeResponseSet()
                                virtual void testSeResponseSet();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSelectionByAidRequest()
                                virtual void testSelectionByAidRequest();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSelectionByAtrRequest()
                                virtual void testSelectionByAtrRequest();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testNotificationMode()
                                virtual void testNotificationMode();

                                /**
                                 * Test Serialization of Keyple Reader Exceptions
                                 */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSimpleKeypleException()
                                virtual void testSimpleKeypleException();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testStackedKeypleException()
                                virtual void testStackedKeypleException();

                                /*
                                 * Utility Method
                                 */

                                virtual std::shared_ptr<void> testSerializeDeserializeObj(std::shared_ptr<void> obj, std::type_info objectClass);

                            };

                        }
                    }
                }
            }
        }
    }
}
