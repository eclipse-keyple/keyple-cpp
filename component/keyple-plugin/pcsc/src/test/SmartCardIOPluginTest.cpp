#include "SmartCardIOPluginTest.h"
#include "../../../../../../../main/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace pcsc {
//                    import static org.junit.Assert.assertEquals;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                    using org::junit::Before;
                    using org::junit::Test;
                    using org::junit::runner::RunWith;
                    using org::mockito::InjectMocks;
                    using org::mockito::Mock;
                    using org::mockito::Spy;
                    using org::mockito::junit::MockitoJUnitRunner;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setUp() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, javax.smartcardio.CardException
                    void SmartCardIOPluginTest::setUp() throw(KeypleReaderException, CardException) {
                        // smartCardPluginSpyied = spy(plugin);
                        /*
                         * when(plugin.getReaders()).thenReturn(cardTerminals); List<CardTerminal> terms = new
                         * ArrayList<CardTerminal>(); terms.add(cardTerminal);
                         * when(cardTerminals.list()).thenReturn(terms);
                         * when(cardTerminal.getName()).thenReturn("PcscPlugin");
                         */
                    }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testGetReaders() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                    void SmartCardIOPluginTest::testGetReaders() throw(KeypleReaderException) {
                        assertEquals(plugin->getReaders()->size(), 1);
                        assertEquals("PcscPlugin", plugin->getName());
                    }
                }
            }
        }
    }
}
