#include "SmartCardIOPluginTest.h"
#include "PcscPlugin.h"
#include "ProxyReader.h"
#include "KeypleReaderException.h"


        namespace keyple {
        namespace core {
            namespace plugin {
                namespace pcsc {
//                    import static org.junit.Assert.assertEquals;
                    using KeypleReaderException = keyple::core::seproxy::exception::KeypleReaderException;
                    using ProxyReader = keyple::core::seproxy::message::ProxyReader;


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
