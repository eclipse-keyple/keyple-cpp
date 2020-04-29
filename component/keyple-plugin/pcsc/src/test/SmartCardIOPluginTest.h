#pragma once

#include <vector>
#include <memory>

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace plugin { namespace pcsc { class PcscPlugin; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace message { class ProxyReader; } } } } }
namespace org { namespace eclipse { namespace keyple { namespace seproxy { namespace exception { class KeypleReaderException; } } } } }

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
//                    import static org.junit.Assert.assertEquals;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @RunWith(MockitoJUnitRunner.class) public class SmartCardIOPluginTest
                    class SmartCardIOPluginTest : public std::enable_shared_from_this<SmartCardIOPluginTest> {
                    private:
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @InjectMocks @Spy private PcscPlugin plugin;
                        std::shared_ptr<PcscPlugin> plugin;

                    public:
                        std::shared_ptr<PcscPlugin> smartCardPluginSpyied;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Mock java.util.List<org.eclipse.keyple.seproxy.message.ProxyReader> cardTerminals;
                        std::vector<std::shared_ptr<ProxyReader>> cardTerminals;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Mock javax.smartcardio.CardTerminal cardTerminal;
                        std::shared_ptr<CardTerminal> cardTerminal;

                        // fclairamb (2018-03-01): This test made no sense, we were using a CardTerminals which is
                        // PCSC-specific when we
                        // should be returning ProxyReaders
//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, javax.smartcardio.CardException
                        virtual void setUp() throw(KeypleReaderException, CardException);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetReaders() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        virtual void testGetReaders() throw(KeypleReaderException);

                    };

                }
            }
        }
    }
}
