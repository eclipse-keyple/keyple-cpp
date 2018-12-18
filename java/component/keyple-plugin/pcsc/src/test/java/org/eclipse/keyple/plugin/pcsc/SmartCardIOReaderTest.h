#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace pcsc { class PcscReader; } } } } }

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
                namespace pcsc {

//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                    import static org.junit.Assert.*;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                    import static org.mockito.Matchers.any;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                    import static org.mockito.Mockito.*;
                    using namespace org::eclipse::keyple::seproxy::exception;
                    using namespace org::eclipse::keyple::seproxy::message;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(MockitoJUnitRunner.class) public class SmartCardIOReaderTest
                    class SmartCardIOReaderTest : public std::enable_shared_from_this<SmartCardIOReaderTest> {

                    private:
                        std::shared_ptr<PcscReader> reader;

                        std::string readerName;

                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Mock CardTerminal terminal;
                        std::shared_ptr<CardTerminal> terminal;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Mock Card card;
                        std::shared_ptr<Card> card;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Mock CardChannel channel;
                        std::shared_ptr<CardChannel> channel;

                        std::shared_ptr<ATR> atr;

                        std::shared_ptr<ResponseAPDU> res;

                    private:
                        std::vector<char> responseApduByte;

                    public:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws CardException, IllegalArgumentException, KeypleBaseException
                        virtual void setUp() throw(CardException, std::invalid_argument, KeypleBaseException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testSmartCardIOReader()
                        virtual void testSmartCardIOReader();

                        // TODO redesign @Test
                        virtual void testGettersSetters() throw(std::invalid_argument, KeypleBaseException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testIsSEPresent() throws CardException, NoStackTraceThrowable
                        virtual void testIsSEPresent() throw(CardException, NoStackTraceThrowable);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = KeypleReaderException.class) public void testIsSEPresentWithException() throws CardException, NoStackTraceThrowable
                        virtual void testIsSEPresentWithException() throw(CardException, NoStackTraceThrowable);

                        // TODO redesign @Test
                        virtual void testTransmitCardNotPresent() throw(CardException, KeypleReaderException, KeypleReaderException);

                        // TODO redesign @Test
                        virtual void testTransmitToCardWithoutAidToSelect() throw(CardException, KeypleReaderException, KeypleReaderException);

                        // TODO redesign @Test
                        virtual void testTransmitToCardWithAidToSelect() throw(CardException, KeypleReaderException, KeypleReaderException);

                        // TODO redesign @Test
                        virtual void testTransmitToCardAndDisconnect() throw(CardException, KeypleReaderException, KeypleReaderException);
                    };

                }
            }
        }
    }
}
