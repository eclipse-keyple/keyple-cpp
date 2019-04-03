#include "NativeReaderServiceTest.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/transport/factory/TransportFactory.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/pluginse/VirtualReaderService.h"
#include "../../../../../../../../../../stub/src/main/java/org/eclipse/keyple/plugin/stub/StubReader.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/pluginse/VirtualReader.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/nativese/NativeReaderServiceImpl.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/transport/impl/java/LocalTransportFactory.h"
#include "Integration.h"
#include "../../../../../../../../../../stub/src/main/java/org/eclipse/keyple/plugin/stub/StubPlugin.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/transport/impl/java/LocalClient.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace integration {
                        using NativeReaderServiceImpl = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderServiceImpl;
                        using VirtualReader = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader;
                        using VirtualReaderService = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReaderService;
                        using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::factory::TransportFactory;
                        using LocalClient = org::eclipse::keyple::plugin::remotese::transport::impl::java::LocalClient;
                        using LocalTransportFactory = org::eclipse::keyple::plugin::remotese::transport::impl::java::LocalTransportFactory;
                        using StubPlugin = org::eclipse::keyple::plugin::stub::StubPlugin;
                        using StubReader = org::eclipse::keyple::plugin::stub::StubReader;
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using namespace org::junit;
                        using org::junit::rules::TestName;
                        using org::junit::runner::RunWith;
                        using org::mockito::junit::MockitoJUnitRunner;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> NativeReaderServiceTest::logger = org::slf4j::LoggerFactory::getLogger(NativeReaderServiceTest::typeid);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setTup() throws Exception
                        void NativeReaderServiceTest::setTup() throw(std::runtime_error) {
                            logger->info("------------------------------");
                            logger->info("Test {}", name->getMethodName());
                            logger->info("------------------------------");

                            logger->info("*** Init LocalTransportFactory");
                            // use a local transport factory for testing purposes (only java calls between client and
                            // server)
                            // only one client and one server
                            factory = std::make_shared<LocalTransportFactory>();

                            logger->info("*** Bind Master Services");
                            // bind Master services to server
                            virtualReaderService = Integration::bindMaster(factory->getServer());

                            logger->info("*** Bind Slave Services");
                            // bind Slave services to client
                            nativeReaderSpy = Integration::bindSlaveSpy(factory->getClient());

                            nativeReader = Integration::createStubReader(NATIVE_READER_NAME);

                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws Exception
                        void NativeReaderServiceTest::tearDown() throw(std::runtime_error) {

                            logger->info("TearDown Test");

                            std::shared_ptr<StubPlugin> stubPlugin = StubPlugin::getInstance();

                            // delete stubReader
                            stubPlugin->unplugStubReader(nativeReader->getName(), true);

                            // Thread.sleep(500);

                            // delete observer and monitor thread
                            // stubPlugin.removeObserver(stubPluginObserver);

                            nativeReader->clearObservers();

                            // Thread.sleep(500);
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOKConnect() throws Exception
                        void NativeReaderServiceTest::testOKConnect() throw(std::runtime_error) {

                            std::string sessionId = nativeReaderSpy->connectReader(nativeReader, CLIENT_NODE_ID);

                            // assert that a virtual reader has been created
                            std::shared_ptr<VirtualReader> virtualReader = std::static_pointer_cast<VirtualReader>(virtualReaderService->getPlugin()->getReaderByRemoteName(NATIVE_READER_NAME));

                            Assert::assertEquals(NATIVE_READER_NAME, virtualReader->getNativeReaderName());
                            Assert::assertEquals(1, nativeReader->countObservers());
                            Assert::assertEquals(0, virtualReader->countObservers());
                            Assert::assertNotNull(sessionId);


                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void testKOConnectError() throws Exception
                        void NativeReaderServiceTest::testKOConnectError() throw(std::runtime_error) {

                            // first connectReader is successful
                            std::string sessionId = nativeReaderSpy->connectReader(nativeReader, CLIENT_NODE_ID);

                            // assert an exception will be contained into keypleDto response
                            // doAnswer(Integration.assertContainsException()).when(nativeReaderSpy)
                            // .onDTO(ArgumentMatchers.<TransportDto>any());

                            // should throw a DTO with an exception in master side KeypleReaderException
                            nativeReaderSpy->connectReader(nativeReader, CLIENT_NODE_ID);


                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void testKOConnectServerError() throws Exception
                        void NativeReaderServiceTest::testKOConnectServerError() throw(std::runtime_error) {

                            // bind Slave to faulty client
                            nativeReaderSpy = Integration::bindSlaveSpy(std::make_shared<LocalClient>(nullptr));

                            nativeReaderSpy->connectReader(nativeReader, CLIENT_NODE_ID);
                            // should throw a KeypleRemoteException in slave side
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test public void testOKConnectDisconnect() throws Exception
                        void NativeReaderServiceTest::testOKConnectDisconnect() throw(std::runtime_error) {

                            // connect
                            std::string sessionId = nativeReaderSpy->connectReader(nativeReader, CLIENT_NODE_ID);

                            std::shared_ptr<VirtualReader> virtualReader = std::static_pointer_cast<VirtualReader>(virtualReaderService->getPlugin()->getReaderByRemoteName(NATIVE_READER_NAME));

                            Assert::assertEquals(NATIVE_READER_NAME, virtualReader->getNativeReaderName());

                            // disconnect
                            nativeReaderSpy->disconnectReader(sessionId, nativeReader->getName(), CLIENT_NODE_ID);

                            // assert that the virtual reader has been destroyed
                            Assert::assertEquals(0, virtualReaderService->getPlugin()->getReaders()->size());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Test(expected = org.eclipse.keyple.seproxy.exception.KeypleReaderException.class) public void testKODisconnectServerError() throws Exception
                        void NativeReaderServiceTest::testKODisconnectServerError() throw(std::runtime_error) {

                            // bind Slave to faulty client
                            nativeReaderSpy = Integration::bindSlaveSpy(std::make_shared<LocalClient>(nullptr));

                            nativeReaderSpy->disconnectReader("null", nativeReader->getName(), CLIENT_NODE_ID);
                            // should throw a KeypleRemoteException in slave side
                        }
                    }
                }
            }
        }
    }
}
