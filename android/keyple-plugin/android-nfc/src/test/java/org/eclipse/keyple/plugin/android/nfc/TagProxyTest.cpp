#include "TagProxyTest.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/android/nfc/TagProxy.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace android {
                    namespace nfc {
//                        import static junit.framework.Assert.assertEquals;
//                        import static org.mockito.Mockito.when;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::junit::Before;
                        using org::junit::Rule;
                        using org::junit::Test;
                        using org::junit::rules::ExpectedException;
                        using org::junit::runner::RunWith;
                        using org::mockito::Mockito;
                        using org::powermock::api::mockito::PowerMockito;
                        using org::powermock::core::classloader::annotations::PrepareForTest;
                        using org::powermock::modules::junit4::PowerMockRunner;
                        using android::nfc::Tag;
                        using android::nfc::tech::IsoDep;
                        using android::nfc::tech::MifareClassic;
                        using android::nfc::tech::MifareUltralight;
                        using junit::framework::Assert;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void SetUp() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        void TagProxyTest::SetUp() throw(KeypleReaderException) {
                            initIsoDep();
                            initMifare();
                            initMifareUL();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getTagProxyIsoDep() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        void TagProxyTest::getTagProxyIsoDep() throw(KeypleReaderException) {
                            std::shared_ptr<TagProxy> tagProxy = TagProxy::getTagProxy(tagIso);
                            assertEquals("android.nfc.tech.IsoDep", tagProxy->getTech());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getTagProxyMifareClassic() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        void TagProxyTest::getTagProxyMifareClassic() throw(KeypleReaderException) {
                            std::shared_ptr<TagProxy> tagProxy = TagProxy::getTagProxy(tagMifare);
                            assertEquals("android.nfc.tech.MifareClassic", tagProxy->getTech());

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getTagProxyMifareUltralight() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        void TagProxyTest::getTagProxyMifareUltralight() throw(KeypleReaderException) {
                            std::shared_ptr<TagProxy> tagProxy = TagProxy::getTagProxy(tagMifareUL);
                            assertEquals("android.nfc.tech.MifareUltralight", tagProxy->getTech());

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void getTagProxyNull() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        void TagProxyTest::getTagProxyNull() throw(KeypleReaderException) {
                            std::shared_ptr<Tag> tag = Mockito::mock(Tag::typeid);
                            when(tag->getTechList()).thenReturn(std::vector<std::string> {"unknown tag"});
                            std::shared_ptr<TagProxy> tagProxy = TagProxy::getTagProxy(tag);
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void getTag() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                        void TagProxyTest::getTag() throw(KeypleReaderException, IOException) {

                            // test
                            std::shared_ptr<TagProxy> tagProxy = TagProxy::getTagProxy(tagIso);

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void connect() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                        void TagProxyTest::connect() throw(KeypleReaderException, IOException) {
                            // test
                            std::shared_ptr<TagProxy> tagProxy = TagProxy::getTagProxy(tagIso);

                            tagProxy->connect();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void close() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                        TagProxyTest::~TagProxyTest() {

                            // test
                            std::shared_ptr<TagProxy> tagProxy = TagProxy::getTagProxy(tagIso);

                            tagProxy->close();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void isConnected() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                        void TagProxyTest::isConnected() throw(KeypleReaderException, IOException) {

                            // test
                            std::shared_ptr<TagProxy> tagProxy = TagProxy::getTagProxy(tagIso);

                            tagProxy->isConnected();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void transceive() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                        void TagProxyTest::transceive() throw(KeypleReaderException, IOException) {

                            std::shared_ptr<TagProxy> tagProxy = TagProxy::getTagProxy(tagIso);

                            tagProxy->transceive((std::string("0000")).getBytes());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void getATRMifare() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                        void TagProxyTest::getATRMifare() throw(KeypleReaderException, IOException) {

                            std::shared_ptr<TagProxy> tagProxy = TagProxy::getTagProxy(tagMifare);

                            Assert::assertTrue(Arrays::equals(tagProxy->getATR(), ByteArrayUtils::fromHex("3B8F8001804F0CA000000306030001000000006A")));
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void getATRIsodep() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                        void TagProxyTest::getATRIsodep() throw(KeypleReaderException, IOException) {
                            // test
                            std::shared_ptr<TagProxy> tagProxy = TagProxy::getTagProxy(tagIso);

                            Assert::assertNull(tagProxy->getATR());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.junit.Test.None.class) public void getATRMifareUL() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException, java.io.IOException
                        void TagProxyTest::getATRMifareUL() throw(KeypleReaderException, IOException) {

                            std::shared_ptr<TagProxy> tagProxy = TagProxy::getTagProxy(tagMifareUL);
                            Assert::assertTrue(Arrays::equals(tagProxy->getATR(), ByteArrayUtils::fromHex("3B8F8001804F0CA0000003060300030000000068")));
                        }

                        void TagProxyTest::initIsoDep() {
                            PowerMockito::mockStatic(IsoDep::typeid);
                            isoDep = Mockito::mock(IsoDep::typeid);
                            tagIso = Mockito::mock(Tag::typeid);

                            when(tagIso->getTechList()).thenReturn(std::vector<std::string> {"android.nfc.tech.IsoDep", "android.nfc.tech.NfcB"});
                            when(IsoDep->get(tagIso)).thenReturn(isoDep);


                        }

                        void TagProxyTest::initMifare() {
                            PowerMockito::mockStatic(MifareClassic::typeid);
                            mifare = Mockito::mock(MifareClassic::typeid);
                            tagMifare = Mockito::mock(Tag::typeid);


                            when(MifareClassic->get(tagMifare)).thenReturn(mifare);
                            when(tagMifare->getTechList()).thenReturn(std::vector<std::string> {"android.nfc.tech.MifareClassic", "android.nfc.tech.NfcA"});


                        }

                        void TagProxyTest::initMifareUL() {
                            PowerMockito::mockStatic(MifareUltralight::typeid);
                            tagMifareUL = Mockito::mock(Tag::typeid);
                            mifareUL = Mockito::mock(MifareUltralight::typeid);

                            when(tagMifareUL->getTechList()).thenReturn(std::vector<std::string> {"android.nfc.tech.MifareUltralight", "android.nfc.tech.NfcA"});
                            when(MifareUltralight->get(tagMifareUL)).thenReturn(mifareUL);


                        }
                    }
                }
            }
        }
    }
}
