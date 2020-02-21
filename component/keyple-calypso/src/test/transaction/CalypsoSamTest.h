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

                    //using namespace keyple::calypso::command;
                    //using namespace keyple::calypso::command::sam;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                    import static org.eclipse.keyple.calypso.command.sam.SamRevision.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                    import static org.junit.Assert.*;



                    class CalypsoSamTest : public std::enable_shared_from_this<CalypsoSamTest> {
                        /** basic CalypsoSam test: nominal ATR parsing */
                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_CalypsoSam_1()
                        virtual void test_CalypsoSam_1();

                        /* S1D D1 */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_CalypsoSam_2()
                        virtual void test_CalypsoSam_2();

                        /* S1D D2 */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_CalypsoSam_3()
                        virtual void test_CalypsoSam_3();

                        /* C1 */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_CalypsoSam_4()
                        virtual void test_CalypsoSam_4();

                        /* E1 */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void test_CalypsoSam_5()
                        virtual void test_CalypsoSam_5();

                        /* Unrecognized E2 */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalStateException.class) public void test_CalypsoSam_6()
                        virtual void test_CalypsoSam_6();

                        /* Bad Calypso SAM ATR (0000 instead of 9000) */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalStateException.class) public void test_CalypsoSam_7()
                        virtual void test_CalypsoSam_7();

                        /* Bad Calypso SAM ATR (empty array) */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalStateException.class) public void test_CalypsoSam_8()
                        virtual void test_CalypsoSam_8();
                    };

                }
            }
        }
