#include "AndroidNfcPluginTest.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/android/nfc/AndroidNfcPlugin.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/android/nfc/AndroidNfcReader.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace android {
                    namespace nfc {
//                        import static org.hamcrest.CoreMatchers.instanceOf;
//                        import static org.junit.Assert.assertThat;
//                        import static org.powermock.api.mockito.PowerMockito.when;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using org::junit::Before;
                        using org::junit::Test;
                        using org::junit::runner::RunWith;
                        using org::powermock::api::mockito::PowerMockito;
                        using org::powermock::core::classloader::annotations::PrepareForTest;
                        using org::powermock::modules::junit4::PowerMockRunner;
                        using junit::framework::Assert;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void SetUp() throws java.io.IOException
                        void AndroidNfcPluginTest::SetUp() throw(IOException) {
                            PowerMockito::mockStatic(AndroidNfcReader::typeid);
                            when(AndroidNfcReader::getInstance()).thenReturn(PowerMockito::mock(AndroidNfcReader::typeid));

                            // get unique instance
                            plugin = AndroidNfcPlugin::getInstance();

                            // reset parameters

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getInstance() throws java.io.IOException
                        void AndroidNfcPluginTest::getInstance() throw(IOException) {
                            Assert::assertTrue(plugin != nullptr);
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setParameters() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException
                        void AndroidNfcPluginTest::setParameters() throw(KeypleBaseException) {

                            std::unordered_map<std::string, std::string> parameters;
                            parameters.emplace("key1", "value1");
                            plugin->setParameters(parameters);
                            Assert::assertTrue(plugin->getParameters().size() > 0);
                            Assert::assertTrue(plugin->getParameters()["key1"] == "value1");

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getParameters() throws java.io.IOException
                        void AndroidNfcPluginTest::getParameters() throw(IOException) {
                            Assert::assertTrue(plugin->getParameters().size() > 0);
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setParameter() throws java.io.IOException
                        void AndroidNfcPluginTest::setParameter() throw(IOException) {
                            plugin->setParameter("key2", "value2");
                            Assert::assertTrue(plugin->getParameters().size() > 0);
                            Assert::assertTrue(plugin->getParameters()["key2"] == "value2");
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getReaders() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        void AndroidNfcPluginTest::getReaders() throw(KeypleReaderException) {
                            Assert::assertTrue(plugin->getReaders()->size() == 1);
                            assertThat(plugin->getReaders()->first(), instanceOf(AndroidNfcReader::typeid));
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getName() throws java.io.IOException
                        void AndroidNfcPluginTest::getName() throw(IOException) {
                            Assert::assertTrue(plugin->getName().equals(AndroidNfcPlugin::PLUGIN_NAME));
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getNativeReader() throws java.io.IOException
                        void AndroidNfcPluginTest::getNativeReader() throw(IOException) {
                            assertThat(plugin->getNativeReader(AndroidNfcReader::READER_NAME), instanceOf(AndroidNfcReader::typeid));
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getNativeReaders() throws java.io.IOException
                        void AndroidNfcPluginTest::getNativeReaders() throw(IOException) {
                            Assert::assertTrue(plugin->getNativeReaders()->size() == 1);
                            assertThat(plugin->getNativeReaders()->first(), instanceOf(AndroidNfcReader::typeid));
                        }
                    }
                }
            }
        }
    }
}
