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
            namespace calypso {
                namespace command {
                    namespace sam {
                        namespace builder {

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                            import static org.junit.Assert.assertArrayEquals;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(MockitoJUnitRunner.class) public class DigestInitCmdBuildTest
                            class DigestInitCmdBuildTest : public std::enable_shared_from_this<DigestInitCmdBuildTest> {
                            public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void digestInitCmd_inconsistent() throws IllegalArgumentException
                                virtual void digestInitCmd_inconsistent() throw(std::invalid_argument);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void digestInitCmd_inconsistent_digestNull() throws IllegalArgumentException
                                virtual void digestInitCmd_inconsistent_digestNull() throw(std::invalid_argument);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void digestInitCmd() throws IllegalArgumentException
                                virtual void digestInitCmd() throw(std::invalid_argument);
                            };

                        }
                    }
                }
            }
        }
    }
}
