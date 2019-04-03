#pragma once

#include "VirtualReaderBaseTest.h"
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
            namespace plugin {
                namespace remotese {
                    namespace integration {

                        using namespace org::eclipse::keyple::seproxy::message;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        /**
                         * Test transmit scenarii extends configuration from VirtualReaderTest
                         */
                        class VirtualReaderTransmitTest : public VirtualReaderBaseTest {

                        private:
                            static const std::shared_ptr<Logger> logger;


                        public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws Exception
                            virtual void setUp() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws Exception
                            virtual void tearDown() throw(std::runtime_error);

                            /*
                             * TRANSMITS
                             */

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testKOTransmitSet_NoSE()
                            virtual void testKOTransmitSet_NoSE();

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testKOTransmit_NoSE()
                            virtual void testKOTransmit_NoSE();

                            /**
                             * Successful Transmit with a Calypso command to a Calypso SE
                             *
                             * @throws Exception
                             */
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void rse_transmit_Hoplink_Sucessfull() throws Exception
                            virtual void rse_transmit_Hoplink_Sucessfull() throw(std::runtime_error);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void rse_transmit_no_response() throws Exception
                            virtual void rse_transmit_no_response() throw(std::runtime_error);


//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_0() throws InterruptedException
                            virtual void transmit_partial_response_set_0() throw(InterruptedException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_1() throws InterruptedException
                            virtual void transmit_partial_response_set_1() throw(InterruptedException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_2() throws InterruptedException
                            virtual void transmit_partial_response_set_2() throw(InterruptedException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_set_3() throws InterruptedException
                            virtual void transmit_partial_response_set_3() throw(InterruptedException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_0() throws InterruptedException
                            virtual void transmit_partial_response_0() throw(InterruptedException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_1() throws InterruptedException
                            virtual void transmit_partial_response_1() throw(InterruptedException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_2() throws InterruptedException
                            virtual void transmit_partial_response_2() throw(InterruptedException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmit_partial_response_3() throws InterruptedException
                            virtual void transmit_partial_response_3() throw(InterruptedException);

protected:
                            std::shared_ptr<VirtualReaderTransmitTest> shared_from_this() {
                                return std::static_pointer_cast<VirtualReaderTransmitTest>(VirtualReaderBaseTest::shared_from_this());
                            }
                        };

                    }
                }
            }
        }
    }
}
