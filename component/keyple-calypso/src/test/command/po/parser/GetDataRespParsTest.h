#pragma once

#include <string>
#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

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
        namespace keyple {
            namespace calypso {
                namespace command {
                    namespace po {
                        namespace parser {



//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(MockitoJUnitRunner.class) public class GetDataRespParsTest
                            class GetDataRespParsTest : public std::enable_shared_from_this<GetDataRespParsTest> {
                            public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFCIparser_FCI_OK()
                                virtual void testFCIparser_FCI_OK();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFCIparser_FCI_Invalidated()
                                virtual void testFCIparser_FCI_Invalidated();


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testFCIparser_FCI_BadTags()
                                virtual void testFCIparser_FCI_BadTags();
                            };

                        }
                    }
                }
            }
        }
