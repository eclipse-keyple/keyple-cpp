#include "AndroidOmapiReaderTest.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ProxyReader.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/android/omapi/AndroidOmapiReader.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/NoStackTraceThrowable.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeResponseSet.h"
#include "../../../../../../../../../../../../java/component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/PoClass.h"
#include "../../../../../../../../../../../../java/component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/builder/ReadRecordsCmdBuild.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequest.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/SeRequestSet.h"
#include "../../../../../../../../../../../../java/component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactsProtocols.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace android {
                    namespace omapi {
//                        import static org.powermock.api.mockito.PowerMockito.when;
                        using PoClass = org::eclipse::keyple::calypso::command::PoClass;
                        using ReadRecordsCmdBuild = org::eclipse::keyple::calypso::command::po::builder::ReadRecordsCmdBuild;
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                        using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                        using ProxyReader = org::eclipse::keyple::seproxy::message::ProxyReader;
                        using SeRequest = org::eclipse::keyple::seproxy::message::SeRequest;
                        using SeRequestSet = org::eclipse::keyple::seproxy::message::SeRequestSet;
                        using SeResponseSet = org::eclipse::keyple::seproxy::message::SeResponseSet;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using ContactsProtocols = org::eclipse::keyple::seproxy::protocol::ContactsProtocols;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::junit::Assert;
                        using org::junit::Before;
                        using org::junit::Test;
                        using org::junit::runner::RunWith;
                        using org::mockito::Mockito;
                        using org::powermock::modules::junit4::PowerMockRunner;
                        using org::simalliance::openmobileapi::Channel;
                        using org::simalliance::openmobileapi::Reader;
                        using org::simalliance::openmobileapi::Session;

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void SetUp() throws Exception
                        void AndroidOmapiReaderTest::SetUp() throw(std::runtime_error) {
                            // default reader connected with secure element with poAid
                            omapiReader = mockReader();

                            // instanciate proxyReader with omapiReader
                            proxyReader = std::make_shared<AndroidOmapiReader>(PLUGIN_NAME, omapiReader, omapiReader->getName());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getInstance() throws Exception
                        void AndroidOmapiReaderTest::getInstance() throw(std::runtime_error) {
                            Assert::assertNotNull(proxyReader);
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getName() throws Exception
                        void AndroidOmapiReaderTest::getName() throw(std::runtime_error) {
                            Assert::assertEquals(omapiReader->getName(), proxyReader->getName());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void isSEPresent() throws org.eclipse.keyple.seproxy.exception.NoStackTraceThrowable
                        void AndroidOmapiReaderTest::isSEPresent() throw(NoStackTraceThrowable) {
                            Assert::assertEquals(true, proxyReader->isSePresent());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void getParameters() throws org.eclipse.keyple.seproxy.exception.NoStackTraceThrowable
                        void AndroidOmapiReaderTest::getParameters() throw(NoStackTraceThrowable) {
                            Assert::assertNotNull(proxyReader->getParameters());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setParameters() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException
                        void AndroidOmapiReaderTest::setParameters() throw(KeypleBaseException) {
                            std::unordered_map<std::string, std::string> parameters;
                            parameters.emplace("key1", "value1");
                            proxyReader->setParameters(parameters);
                            Assert::assertTrue(proxyReader->getParameters().size() == 1);
                            Assert::assertTrue(proxyReader->getParameters()["key1"] == "value1");
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void setParameter() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException
                        void AndroidOmapiReaderTest::setParameter() throw(KeypleBaseException) {
                            proxyReader->setParameter("key2", "value2");
                            Assert::assertTrue(proxyReader->getParameters().size() == 1);
                            Assert::assertTrue(proxyReader->getParameters()["key2"] == "value2");
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitHoplinkSuccessfull() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException
                        void AndroidOmapiReaderTest::transmitHoplinkSuccessfull() throw(KeypleBaseException) {
                            // default init

                            // test
                            std::shared_ptr<SeResponseSet> seResponse = proxyReader->transmitSet(getCalypsoRequestSample());

                            // assert
                            Assert::assertTrue(seResponse->getResponses()[0]->getApduResponses()[0].isSuccessful());

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitNoAid() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                        void AndroidOmapiReaderTest::transmitNoAid() throw(KeypleBaseException, IOException) {

                            // init
                            omapiReader = mockReaderWithNoAid();
                            proxyReader = std::make_shared<AndroidOmapiReader>(PLUGIN_NAME, omapiReader, omapiReader->getName());

                            // test
                            std::shared_ptr<SeResponseSet> seResponse = proxyReader->transmitSet(getCalypsoRequestSample());

                            // assert
                            Assert::assertNull(seResponse->getResponses()[0]);

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void transmitWrongProtocol() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException
                        void AndroidOmapiReaderTest::transmitWrongProtocol() throw(KeypleBaseException) {
                            // init
                            std::string poAid = "A000000291A000000191";
                            std::shared_ptr<ReadRecordsCmdBuild> poReadRecordCmd_T2Env = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, static_cast<char>(0x14), static_cast<char>(0x01), true, static_cast<char>(0x20), "Hoplink EF T2Environment");
                            std::vector<std::shared_ptr<ApduRequest>> poApduRequestList = Arrays::asList(poReadRecordCmd_T2Env->getApduRequest());

                            // wrong protocol
                            std::shared_ptr<SeRequest> seRequest = std::make_shared<SeRequest>(std::make_shared<SeRequest::AidSelector>(ByteArrayUtils::fromHex(poAid)), poApduRequestList, ChannelState::CLOSE_AFTER, ContactlessProtocols::PROTOCOL_MIFARE_UL, nullptr);

                            // test
                            std::shared_ptr<SeResponseSet> seResponse = proxyReader->transmitSet(std::make_shared<SeRequestSet>(seRequest));

                            // assert
                            Assert::assertNull(seResponse->getResponses()[0]);

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void transmitNotConnected() throws org.eclipse.keyple.seproxy.exception.KeypleBaseException, java.io.IOException
                        void AndroidOmapiReaderTest::transmitNotConnected() throw(KeypleBaseException, IOException) {

                            // init
                            std::shared_ptr<Reader> omapiReader = Mockito::mock(Reader::typeid);
                            when(omapiReader->getName()).thenReturn("SIM1");
                            when(omapiReader->isSecureElementPresent()).thenReturn(false);
                            when(omapiReader->openSession()).thenThrow(std::make_shared<IOException>());
                            proxyReader = std::make_shared<AndroidOmapiReader>(PLUGIN_NAME, omapiReader, omapiReader->getName());

                            // test
                            std::shared_ptr<SeResponseSet> seResponse = proxyReader->transmitSet(getCalypsoRequestSample());

                            // expected = KeypleReaderException.class
                        }

                        std::shared_ptr<Reader> AndroidOmapiReaderTest::mockReader() throw(IOException) {

                            std::shared_ptr<Reader> omapiReader = Mockito::mock(Reader::typeid);
                            std::shared_ptr<Session> session = Mockito::mock(Session::typeid);
                            std::shared_ptr<Channel> channel = Mockito::mock(Channel::typeid);

                            when(omapiReader->getName()).thenReturn("SIM1");
                            when(omapiReader->isSecureElementPresent()).thenReturn(true);
                            when(session->openLogicalChannel(ByteArrayUtils::fromHex(poAid))).thenReturn(channel);
                            when(omapiReader->openSession()).thenReturn(session);
                            when(session->getATR()).thenReturn(nullptr);
                            when(channel->getSelectResponse()).thenReturn(ByteArrayUtils::fromHex(poAidResponse));
                            when(channel->getSession()).thenReturn(session);

                            when(channel->transmit(ByteArrayUtils::fromHex("00B201A420"))).thenReturn(ByteArrayUtils::fromHex("00000000000000000000000000000000000000000000000000000000000000009000"));

                            return omapiReader;

                        }

                        std::shared_ptr<Reader> AndroidOmapiReaderTest::mockReaderWithNoAid() throw(KeypleBaseException, IOException) {

                            std::shared_ptr<Reader> omapiReader = Mockito::mock(Reader::typeid);
                            std::shared_ptr<Session> session = Mockito::mock(Session::typeid);
                            std::shared_ptr<Channel> channel = Mockito::mock(Channel::typeid);

                            when(omapiReader->getName()).thenReturn("SIM1");
                            when(omapiReader->isSecureElementPresent()).thenReturn(true);
                            when(omapiReader->openSession()).thenReturn(session);
                            when(session->openLogicalChannel(ByteArrayUtils::fromHex(poAid))).thenThrow(std::make_shared<NoSuchElementException>(""));

                            return omapiReader;

                        }

                        std::shared_ptr<SeRequestSet> AndroidOmapiReaderTest::getCalypsoRequestSample() {
                            std::string poAid = "A000000291A000000191";

                            std::shared_ptr<ReadRecordsCmdBuild> poReadRecordCmd_T2Env = std::make_shared<ReadRecordsCmdBuild>(PoClass::ISO, static_cast<char>(0x14), static_cast<char>(0x01), true, static_cast<char>(0x20), "Hoplink EF T2Environment");

                            std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;

                            poApduRequestList = Arrays::asList(poReadRecordCmd_T2Env->getApduRequest());

                            std::shared_ptr<SeRequest> seRequest = std::make_shared<SeRequest>(std::make_shared<SeRequest::AidSelector>(ByteArrayUtils::fromHex(poAid)), poApduRequestList, ChannelState::CLOSE_AFTER, ContactsProtocols::PROTOCOL_ISO7816_3, nullptr);

                            return std::make_shared<SeRequestSet>(seRequest);

                        }
                    }
                }
            }
        }
    }
}
