#pragma once

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
            namespace plugin {
                namespace remotese {
                    namespace rm {
                        namespace json {


                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(MockitoJUnitRunner.class) public class KeypleDtoHelperTest
                            class KeypleDtoHelperTest : public std::enable_shared_from_this<KeypleDtoHelperTest> {

                            private:
                                static const std::shared_ptr<Logger> logger;

                            public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testContainsException()
                                virtual void testContainsException();

                            };

                        }
                    }
                }
            }
        }
    }
}
