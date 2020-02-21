#pragma once

#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

/********************************************************************************
 * Copyright (c) 2019 Calypso Networks Association https://www.calypsonet-asso.org/
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
                namespace transaction {

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                    import static org.junit.Assert.*;

                    class SamSelectorTest : public std::enable_shared_from_this<SamSelectorTest> {

                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_AidSelectorNull()
                        virtual void test_AidSelectorNull();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_SamRevision_S1D()
                        virtual void test_SamRevision_S1D();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_SamRevision_S1E()
                        virtual void test_SamRevision_S1E();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_SamRevision_C1()
                        virtual void test_SamRevision_C1();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_SamRevision_ANY()
                        virtual void test_SamRevision_ANY();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_SamSerialNumber()
                        virtual void test_SamSerialNumber();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_ExtraInfo()
                        virtual void test_ExtraInfo();
                    };

                }
            }
        }
