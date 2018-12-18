#include "AndroidNfcReaderTest.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/android/nfc/AndroidNfcReader.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/android/nfc/TagProxy.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/android/nfc/AndroidNfcProtocolSettings.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocolSetting.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequestSet.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponseSet.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ReaderEvent.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../java/component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/PoClass.h"
#include "../../../../../../../../../../../../java/component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/builder/ReadRecordsCmdBuild.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequest.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace android {
                    namespace nfc {
//                        import static junit.framework.Assert.assertEquals;
//                        import static org.junit.Assert.assertArrayEquals;
//                        import static org.mockito.Mockito.doThrow;
//                        import static org.mockito.Mockito.when;
                        using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                        using ReadRecordsCmdBuild = org::eclipse::keyple::calypso::command::po::builder::ReadRecordsCmdBuild;
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using ReaderEvent = org::eclipse::keyple::seproxy::event_Renamed::ReaderEvent;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                        using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
                        using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                        using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::junit::Before;
                        using org::junit::Test;
                        using org::junit::runner::RunWith;
                        using org::mockito::Mockito;
                        using org::powermock::api::mockito::PowerMockito;
                        using org::powermock::core::classloader::annotations::PrepareForTest;
                        using org::powermock::modules::junit4::PowerMockRunner;
                        using android::app::Activity;
                        using android::content::Intent;
                        using android::nfc::NfcAdapter;
                        using android::nfc::Tag;
                        using junit::framework::Assert;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void SetUp() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        void AndroidNfcReaderTest::SetUp() throw(KeypleReaderException) {

                            // Mock others objects
                            tagProxy = Mockito::mock(TagProxy::typeid);
                            intent = Mockito::mock(Intent::typeid);
                            activity = Mockito::mock(Activity::typeid);
                            nfcAdapter = Mockito::mock(NfcAdapter::typeid);

                            // mock TagProxy Factory
                            PowerMockito::mockStatic(TagProxy::typeid);
                            when(TagProxy::getTagProxy(tag)).thenReturn(tagProxy);

                            // mock NfcAdapter Factory
                            PowerMockito::mockStatic(NfcAdapter::typeid);
                            when(NfcAdapter::getDefaultAdapter(activity)).thenReturn(nfcAdapter);

                            // instantiate a new Reader for each test
                            reader = std::make_shared<AndroidNfcReader>();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitSuccessfull() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                        void AndroidNfcReaderTest::transmitSuccessfull() throw(KeypleBaseException, IOException) {

                            // config
                            reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(AndroidNfcProtocolSettings::SETTING_PROTOCOL_ISO14443_4));

                            // input
                            std::shared_ptr<SeRequestSet> requests = getRequestIsoDepSetSample();

                            // init Mock
                            when(tagProxy->transceive(ByteArrayUtils::fromHex("00A404000AA000000291A00000019100"))).thenReturn(ByteArrayUtils::fromHex("6F25840BA000000291A00000019102A516BF0C13C70800000000C0E11FA653070A3C230C1410019000"));
                            when(tagProxy->transceive(ByteArrayUtils::fromHex("00B201A420"))).thenReturn(ByteArrayUtils::fromHex("00000000000000000000000000000000000000000000000000000000000000009000"));
                            when(tagProxy->getTech()).thenReturn(AndroidNfcProtocolSettings::SETTING_PROTOCOL_ISO14443_4.getValue());
                            when(tagProxy->isConnected()).thenReturn(true);

                            // test
                            insertSe();
                            std::shared_ptr<SeResponseSet> seResponse = reader->transmitSet(requests);

                            // assert
                            Assert::assertTrue(seResponse->getSingleResponse()->getSelectionStatus()->getFci()->isSuccessful());

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitWrongProtocols() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                        void AndroidNfcReaderTest::transmitWrongProtocols() throw(KeypleBaseException, IOException) {

                            // config reader with Isodep protocols
                            reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(AndroidNfcProtocolSettings::SETTING_PROTOCOL_ISO14443_4));

                            // input
                            std::shared_ptr<SeRequestSet> requests = getRequestIsoDepSetSample();

                            // init Mock with Mifare Classic Smart card
                            when(tagProxy->transceive(ByteArrayUtils::fromHex("00A404000AA000000291A00000019100"))).thenReturn(ByteArrayUtils::fromHex("6F25840BA000000291A00000019102A516BF0C13C70800000000C0E11FA653070A3C230C1410019000"));
                            when(tagProxy->transceive(ByteArrayUtils::fromHex("00B201A420"))).thenReturn(ByteArrayUtils::fromHex("00000000000000000000000000000000000000000000000000000000000000009000"));
                            when(tagProxy->getTech()).thenReturn(AndroidNfcProtocolSettings::SETTING_PROTOCOL_MIFARE_CLASSIC.getValue());
                            when(tagProxy->isConnected()).thenReturn(true);

                            // test
                            insertSe();
                            std::shared_ptr<SeResponseSet> seResponse = reader->transmitSet(requests);

                            // assert the only seRequest is null
                            Assert::assertTrue(seResponse->getSingleResponse() == nullptr);

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitWrongProtocol2() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                        void AndroidNfcReaderTest::transmitWrongProtocol2() throw(KeypleBaseException, IOException) {

                            // config reader with Mifare protocols
                            reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(AndroidNfcProtocolSettings::SETTING_PROTOCOL_MIFARE_CLASSIC));

                            // input
                            std::shared_ptr<SeRequestSet> requests = getRequestIsoDepSetSample();

                            // init Mock with Isodep Classic Smart card
                            when(tagProxy->transceive(ByteArrayUtils::fromHex("00A404000AA000000291A00000019100"))).thenReturn(ByteArrayUtils::fromHex("6F25840BA000000291A00000019102A516BF0C13C70800000000C0E11FA653070A3C230C1410019000"));
                            when(tagProxy->transceive(ByteArrayUtils::fromHex("00B201A420"))).thenReturn(ByteArrayUtils::fromHex("00000000000000000000000000000000000000000000000000000000000000009000"));
                            when(tagProxy->getTech()).thenReturn(AndroidNfcProtocolSettings::SETTING_PROTOCOL_ISO14443_4.getValue());
                            when(tagProxy->isConnected()).thenReturn(true);

                            // test
                            insertSe();
                            std::shared_ptr<SeResponseSet> seResponse = reader->transmitSet(requests);

                            // assert the only seRequest is null
                            Assert::assertTrue(seResponse->getSingleResponse() == nullptr);

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void transmitCardNotConnected() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                        void AndroidNfcReaderTest::transmitCardNotConnected() throw(KeypleBaseException, IOException) {

                            // config reader with Isodep protocols
                            reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(AndroidNfcProtocolSettings::SETTING_PROTOCOL_ISO14443_4));

                            // input
                            std::shared_ptr<SeRequestSet> requests = getRequestIsoDepSetSample();

                            // init Mock with Isodep Classic Smart card
                            when(tagProxy->transceive(ByteArrayUtils::fromHex("00A404000AA000000291A00000019100"))).thenReturn(ByteArrayUtils::fromHex("6F25840BA000000291A00000019102A516BF0C13C70800000000C0E11FA653070A3C230C1410019000"));
                            when(tagProxy->transceive(ByteArrayUtils::fromHex("00B201A420"))).thenReturn(ByteArrayUtils::fromHex("00000000000000000000000000000000000000000000000000000000000000009000"));
                            when(tagProxy->getTech()).thenReturn(AndroidNfcProtocolSettings::SETTING_PROTOCOL_ISO14443_4.getValue());


                            // card is not connected
                            when(tagProxy->isConnected()).thenReturn(false);

                            // test
                            insertSe();
                            std::shared_ptr<SeResponseSet> seResponse = reader->transmitSet(requests);

                            // wait for exception
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitUnkownCard() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                        void AndroidNfcReaderTest::transmitUnkownCard() throw(KeypleBaseException, IOException) {

                            // config reader with Isodep protocols
                            reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(AndroidNfcProtocolSettings::SETTING_PROTOCOL_ISO14443_4));

                            // input
                            std::shared_ptr<SeRequestSet> requests = getRequestIsoDepSetSample();

                            // init Mock with Isodep Classic Smart card
                            when(tagProxy->transceive(ByteArrayUtils::fromHex("00A404000AA000000291A00000019100"))).thenReturn(ByteArrayUtils::fromHex("6F25840BA000000291A00000019102A516BF0C13C70800000000C0E11FA653070A3C230C1410019000"));
                            when(tagProxy->transceive(ByteArrayUtils::fromHex("00B201A420"))).thenReturn(ByteArrayUtils::fromHex("00000000000000000000000000000000000000000000000000000000000000009000"));
                            when(tagProxy->isConnected()).thenReturn(false);

                            // unknown card
                            when(tagProxy->getTech()).thenReturn("Unknown card");

                            // test
                            insertSe();
                            std::shared_ptr<SeResponseSet> seResponse = reader->transmitSet(requests);

                            // assert the only seRequest is null
                            Assert::assertTrue(seResponse->getSingleResponse() == nullptr);
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitUnknownApplication() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                        void AndroidNfcReaderTest::transmitUnknownApplication() throw(KeypleBaseException, IOException) {

                            // config reader with Isodep protocols
                            reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(AndroidNfcProtocolSettings::SETTING_PROTOCOL_ISO14443_4));

                            // input
                            std::shared_ptr<SeRequestSet> requests = getRequestIsoDepSetSample();

                            // init Mock with Isodep Classic Smart card
                            when(tagProxy->transceive(ByteArrayUtils::fromHex("00B201A420"))).thenReturn(ByteArrayUtils::fromHex("00000000000000000000000000000000000000000000000000000000000000009000"));
                            when(tagProxy->isConnected()).thenReturn(false);
                            when(tagProxy->getTech()).thenReturn("Unknown card");

                            // unknown Application
                            when(tagProxy->transceive(ByteArrayUtils::fromHex("00A404000AA000000291A00000019100"))).thenReturn(ByteArrayUtils::fromHex("0000"));

                            // test
                            insertSe();
                            std::shared_ptr<SeResponseSet> seResponse = reader->transmitSet(requests);

                            // assert the only seRequest is null
                            Assert::assertTrue(seResponse->getSingleResponse() == nullptr);
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void setUnknownParameter() throws IllegalArgumentException
                        void AndroidNfcReaderTest::setUnknownParameter() throw(std::invalid_argument) {
                            reader->setParameter("dsfsdf", "sdfdsf");
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setCorrectParameter() throws IllegalArgumentException
                        void AndroidNfcReaderTest::setCorrectParameter() throw(std::invalid_argument) {
                            reader->setParameter(AndroidNfcReader::FLAG_READER_NO_PLATFORM_SOUNDS, "1");
                            Assert::assertEquals(NfcAdapter::FLAG_READER_NO_PLATFORM_SOUNDS, reader->getFlags());
                            reader->setParameter(AndroidNfcReader::FLAG_READER_NO_PLATFORM_SOUNDS, "0");
                            Assert::assertEquals(0, reader->getFlags());
                            reader->setParameter(AndroidNfcReader::FLAG_READER_SKIP_NDEF_CHECK, "1");
                            Assert::assertEquals(NfcAdapter::FLAG_READER_SKIP_NDEF_CHECK, reader->getFlags());
                            reader->setParameter(AndroidNfcReader::FLAG_READER_SKIP_NDEF_CHECK, "0");
                            Assert::assertEquals(0, reader->getFlags());
                            reader->setParameter(AndroidNfcReader::FLAG_READER_PRESENCE_CHECK_DELAY, "10");
                            /*
                             * Fail because android.os.Bundle is not present in the JVM, roboelectric is needed to play
                             * this test Assert.assertEquals(10,
                             * reader.getOptions().get(NfcAdapter.EXTRA_READER_PRESENCE_CHECK_DELAY));
                             * Assert.assertEquals(3, reader.getParameters().size());
                             */

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void setUnCorrectParameter() throws IllegalArgumentException
                        void AndroidNfcReaderTest::setUnCorrectParameter() throw(std::invalid_argument) {
                            reader->setParameter(AndroidNfcReader::FLAG_READER_NO_PLATFORM_SOUNDS, "A");
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void setUnCorrectParameter2() throws IllegalArgumentException
                        void AndroidNfcReaderTest::setUnCorrectParameter2() throw(std::invalid_argument) {
                            reader->setParameter(AndroidNfcReader::FLAG_READER_SKIP_NDEF_CHECK, "2");
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = IllegalArgumentException.class) public void setUnCorrectParameter3() throws IllegalArgumentException
                        void AndroidNfcReaderTest::setUnCorrectParameter3() throw(std::invalid_argument) {
                            reader->setParameter(AndroidNfcReader::FLAG_READER_PRESENCE_CHECK_DELAY, "-1");
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setIsoDepProtocol()
                        void AndroidNfcReaderTest::setIsoDepProtocol() {
                            reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(AndroidNfcProtocolSettings::SETTING_PROTOCOL_ISO14443_4));
                            assertEquals(NfcAdapter::FLAG_READER_NFC_B | NfcAdapter::FLAG_READER_NFC_A, reader->getFlags());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setMifareProtocol()
                        void AndroidNfcReaderTest::setMifareProtocol() {
                            reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(AndroidNfcProtocolSettings::SETTING_PROTOCOL_MIFARE_CLASSIC));
                            assertEquals(NfcAdapter::FLAG_READER_NFC_A, reader->getFlags());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setMifareULProtocol()
                        void AndroidNfcReaderTest::setMifareULProtocol() {
                            reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(AndroidNfcProtocolSettings::SETTING_PROTOCOL_MIFARE_UL));
                            assertEquals(NfcAdapter::FLAG_READER_NFC_A, reader->getFlags());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void insertEvent()
                        void AndroidNfcReaderTest::insertEvent() {

                            reader->addObserver(std::make_shared<ReaderObserverAnonymousInnerClass>(shared_from_this()));

                            insertSe();
                        }

                        AndroidNfcReaderTest::ReaderObserverAnonymousInnerClass::ReaderObserverAnonymousInnerClass(std::shared_ptr<AndroidNfcReaderTest> outerInstance) {
                            this->outerInstance = outerInstance;
                        }

                        void AndroidNfcReaderTest::ReaderObserverAnonymousInnerClass::update(std::shared_ptr<ReaderEvent> event_Renamed) {
                            assertEquals(ReaderEvent::EventType::SE_INSERTED, event_Renamed->getEventType());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isConnected()
                        void AndroidNfcReaderTest::isConnected() {
                            insertSe();
                            when(tagProxy->isConnected()).thenReturn(true);
                            assertEquals(true, reader->isSePresent());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getATR()
                        void AndroidNfcReaderTest::getATR() {
                            insertSe();
                            std::vector<char> atr = {static_cast<char>(0x90), 0x00};
                            when(tagProxy->getATR()).thenReturn(atr);
                            assertEquals(atr, reader->getATR());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isPhysicalChannelOpen()
                        void AndroidNfcReaderTest::isPhysicalChannelOpen() {
                            insertSe();
                            when(tagProxy->isConnected()).thenReturn(true);
                            assertEquals(true, reader->isPhysicalChannelOpen());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void openPhysicalChannelSuccess() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        void AndroidNfcReaderTest::openPhysicalChannelSuccess() throw(KeypleReaderException) {
                            insertSe();
                            when(tagProxy->isConnected()).thenReturn(false);
                            reader->openPhysicalChannel();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void openPhysicalChannelError() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                        void AndroidNfcReaderTest::openPhysicalChannelError() throw(KeypleBaseException, IOException) {
                            // init
                            insertSe();
                            when(tagProxy->isConnected()).thenReturn(false);
                            doThrow(std::make_shared<IOException>()).when(tagProxy).connect();

                            // test
                            reader->openPhysicalChannel();
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void closePhysicalChannelSuccess() throws org.eclipse.keyple.seproxy.exception.KeypleReaderException
                        void AndroidNfcReaderTest::closePhysicalChannelSuccess() throw(KeypleReaderException) {
                            // init
                            insertSe();
                            when(tagProxy->isConnected()).thenReturn(true);

                            // test
                            reader->closePhysicalChannel();

                            // no exception

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void closePhysicalChannelError() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                        void AndroidNfcReaderTest::closePhysicalChannelError() throw(KeypleBaseException, IOException) {
                            // init
                            insertSe();
                            when(tagProxy->isConnected()).thenReturn(true);
                            doThrow(std::make_shared<IOException>()).when(tagProxy)->close();

                            // test
                            reader->closePhysicalChannel();

                            // throw exception

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitAPDUSuccess() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                        void AndroidNfcReaderTest::transmitAPDUSuccess() throw(KeypleBaseException, IOException) {
                            // init
                            insertSe();
                            std::vector<char> in_Renamed = {static_cast<char>(0x90), 0x00};
                            std::vector<char> out = {static_cast<char>(0x90), 0x00};
                            when(tagProxy->transceive(in_Renamed)).thenReturn(out);

                            // test
                            std::vector<char> outBB = reader->transmitApdu(in_Renamed);

                            // assert
                            assertArrayEquals(out, outBB);

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void transmitAPDUError() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                        void AndroidNfcReaderTest::transmitAPDUError() throw(KeypleBaseException, IOException) {
                            // init
                            insertSe();
                            std::vector<char> in_Renamed = {static_cast<char>(0x90), 0x00};
                            std::vector<char> out = {static_cast<char>(0x90), 0x00};
                            when(tagProxy->transceive(in_Renamed)).thenThrow(std::make_shared<IOException>(""));

                            // test
                            std::vector<char> outBB = reader->transmitApdu(in_Renamed);

                            // throw exception
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void protocolFlagMatchesTrue() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                        void AndroidNfcReaderTest::protocolFlagMatchesTrue() throw(KeypleBaseException, IOException) {
                            // init
                            insertSe();
                            reader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(AndroidNfcProtocolSettings::SETTING_PROTOCOL_ISO14443_4));
                            when(tagProxy->getTech()).thenReturn(AndroidNfcProtocolSettings::SETTING_PROTOCOL_ISO14443_4.getValue());

                            // test
                            Assert::assertTrue(reader->protocolFlagMatches(ContactlessProtocols::PROTOCOL_ISO14443_4));
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void processIntent()
                        void AndroidNfcReaderTest::processIntent() {
                            reader->processIntent(intent);
                            Assert::assertTrue(true); // no test
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void printTagId()
                        void AndroidNfcReaderTest::printTagId() {
                            reader->printTagId();
                            Assert::assertTrue(true); // no test
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void enableReaderMode()
                        void AndroidNfcReaderTest::enableReaderMode() {
                            // init instumented test

                            // test
                            reader->enableNFCReaderMode(activity);

                            // nothing to assert
                            Assert::assertTrue(true);
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void disableReaderMode()
                        void AndroidNfcReaderTest::disableReaderMode() {
                            // init
                            reader->enableNFCReaderMode(activity);

                            // test
                            reader->disableNFCReaderMode(activity);

                            // nothing to assert
                            Assert::assertTrue(true);
                        }

                        void AndroidNfcReaderTest::insertSe() {
                            reader->onTagDiscovered(tag);
                        }

                        std::shared_ptr<SeRequestSet> AndroidNfcReaderTest::getRequestIsoDepSetSample() {
                            std::string poAid = "A000000291A000000191";

                            std::shared_ptr<ReadRecordsCmdBuild> poReadRecordCmd_T2Env = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, static_cast<char>(0x14), static_cast<char>(0x01), true, static_cast<char>(0x20), "Hoplink EF T2Environment");

                            std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;

                            poApduRequestList = Arrays::asList(poReadRecordCmd_T2Env->getApduRequest());

                            // TODO change this with the use of the selection API
                            std::shared_ptr<SeRequest> seRequest = std::make_shared<SeRequest>(std::make_shared<SeRequest::AidSelector>(ByteArrayUtils::fromHex(poAid)), poApduRequestList, ChannelState::CLOSE_AFTER, ContactlessProtocols::PROTOCOL_ISO14443_4, nullptr);

                            return std::make_shared<SeRequestSet>(seRequest);

                        }
                    }
                }
            }
        }
    }
}
