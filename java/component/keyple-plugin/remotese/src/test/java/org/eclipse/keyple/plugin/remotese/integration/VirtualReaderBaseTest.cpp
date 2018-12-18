#include "VirtualReaderBaseTest.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/transport/TransportFactory.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/PluginEvent.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/nativese/NativeReaderServiceImpl.h"
#include "../../../../../../../../../../stub/src/main/java/org/eclipse/keyple/plugin/stub/StubReader.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/pluginse/VirtualReader.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/pluginse/VirtualReaderService.h"
#include "../../../../../../../../../../stub/src/main/java/org/eclipse/keyple/plugin/stub/StubPlugin.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/transport/java/LocalTransportFactory.h"
#include "Integration.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocolSetting.h"
#include "../../../../../../../../../../stub/src/main/java/org/eclipse/keyple/plugin/stub/StubProtocolSetting.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace integration {
                        using NativeReaderServiceImpl = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderServiceImpl;
                        using VirtualReader = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReader;
                        using VirtualReaderService = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReaderService;
                        using TransportFactory = org::eclipse::keyple::plugin::remotese::transport::TransportFactory;
                        using LocalTransportFactory = org::eclipse::keyple::plugin::remotese::transport::java::LocalTransportFactory;
                        using StubPlugin = org::eclipse::keyple::plugin::stub::StubPlugin;
                        using StubProtocolSetting = org::eclipse::keyple::plugin::stub::StubProtocolSetting;
                        using StubReader = org::eclipse::keyple::plugin::stub::StubReader;
                        using ObservablePlugin = org::eclipse::keyple::seproxy::event_Renamed::ObservablePlugin;
                        using PluginEvent = org::eclipse::keyple::seproxy::event_Renamed::PluginEvent;
                        using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                        using namespace org::junit;
                        using org::junit::rules::TestName;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> VirtualReaderBaseTest::logger = org::slf4j::LoggerFactory::getLogger(VirtualReaderBaseTest::typeid);

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @Before public void setTup() throws Exception
                        void VirtualReaderBaseTest::setTup() throw(std::runtime_error) {
                            logger->info("------------------------------");
                            logger->info("Test {}", name->getMethodName());
                            logger->info("------------------------------");

                            // assert that there is no stub readers plugged already
                            Assert::assertEquals(0, StubPlugin::getInstance()->getReaders()->size());

                            logger->info("*** Init LocalTransportFactory");
                            // use a local transport factory for testing purposes (only java calls between client and
                            // server). Only one client and one server bound together.
                            factory = std::make_shared<LocalTransportFactory>();

                            stubPluginObserver = std::make_shared<PluginObserverAnonymousInnerClass>(shared_from_this());

                            logger->info("*** Bind Master Services");
                            // bind Master services to server
                            virtualReaderService = Integration::bindMaster(factory->getServer());

                            logger->info("*** Bind Slave Services");
                            // bind Slave services to client
                            nativeReaderService = Integration::bindSlave(factory->getClient());

                            // configure and connect a Stub Native reader
                            nativeReader = connectStubReader(NATIVE_READER_NAME, CLIENT_NODE_ID, stubPluginObserver);

                            // test virtual reader
                            virtualReader = getVirtualReader();


                        }

                        VirtualReaderBaseTest::PluginObserverAnonymousInnerClass::PluginObserverAnonymousInnerClass(std::shared_ptr<VirtualReaderBaseTest> outerInstance) {
                            this->outerInstance = outerInstance;
                        }

                        void VirtualReaderBaseTest::PluginObserverAnonymousInnerClass::update(std::shared_ptr<PluginEvent> pluginEvent) {
                            logger->debug("Default Stub Plugin Observer : {}", pluginEvent->getEventType());
                        }

//JAVA TO C++ CONVERTER TODO TASK: Most Java annotations will not have direct C++ equivalents:
//ORIGINAL LINE: @After public void tearDown() throws Exception
                        void VirtualReaderBaseTest::tearDown() throw(std::runtime_error) {

                            logger->info("TearDown Test");

                            std::shared_ptr<StubPlugin> stubPlugin = StubPlugin::getInstance();

                            stubPlugin->unplugReader(nativeReader->getName());

                            delay(500);

                            nativeReader->clearObservers();

                            stubPlugin->removeObserver(stubPluginObserver);

                            delay(500);

                            logger->info("End of TearDown Test");
                        }

                        std::shared_ptr<StubReader> VirtualReaderBaseTest::connectStubReader(const std::string &readerName, const std::string &nodeId, std::shared_ptr<ObservablePlugin::PluginObserver> observer) throw(std::runtime_error) {
                            // configure native reader
                            std::shared_ptr<StubReader> nativeReader = std::static_pointer_cast<StubReader>(Integration::createStubReader(readerName, observer));
                            nativeReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4));
                            this->nativeReaderService->connectReader(nativeReader, nodeId);
                            return nativeReader;
                        }

                        void VirtualReaderBaseTest::disconnectStubReader(std::shared_ptr<StubReader> nativeReader, const std::string &nodeId) throw(std::runtime_error) {
                            this->nativeReaderService->disconnectReader(nativeReader, nodeId);
                        }

                        std::shared_ptr<VirtualReader> VirtualReaderBaseTest::getVirtualReader() throw(std::runtime_error) {
                            Assert::assertEquals(1, this->virtualReaderService->getPlugin()->getReaders()->size());
                            return std::static_pointer_cast<VirtualReader>(this->virtualReaderService->getPlugin()->getReaders()->first());
                        }
                    }
                }
            }
        }
    }
}
