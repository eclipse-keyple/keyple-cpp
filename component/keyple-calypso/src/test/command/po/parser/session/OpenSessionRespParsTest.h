#pragma once

#include <vector>
#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace keyple { namespace calypso { namespace command { namespace po { namespace parser { namespace security { class AbstractOpenSessionRespPars; } } } } } }

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
                            namespace security {


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(MockitoJUnitRunner.class) public class OpenSessionRespParsTest
                                class OpenSessionRespParsTest : public std::enable_shared_from_this<OpenSessionRespParsTest> {

                                private:
                                    void check(std::shared_ptr<AbstractOpenSessionRespPars> resp);

                                public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testgetResponse_rev2_4()
                                    virtual void testgetResponse_rev2_4();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testgetResponse_rev2_4_no_data()
                                    virtual void testgetResponse_rev2_4_no_data();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testgetResponse_rev2_4_non_ratified()
                                    virtual void testgetResponse_rev2_4_non_ratified();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testgetResponse_rev2_4_no_data_non_ratified()
                                    virtual void testgetResponse_rev2_4_no_data_non_ratified();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalStateException.class) public void testgetResponse_rev2_4_bad_length_inf()
                                    virtual void testgetResponse_rev2_4_bad_length_inf();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalStateException.class) public void testgetResponse_rev2_4_bad_length_sup()
                                    virtual void testgetResponse_rev2_4_bad_length_sup();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testgetResponse_rev3_1()
                                    virtual void testgetResponse_rev3_1();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testgetResponse_rev3_2()
                                    virtual void testgetResponse_rev3_2();

                                };

                            }
                        }
                    }
                }
            }
        }
