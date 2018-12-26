#include "Integration.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/pluginse/VirtualReaderService.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/transport/TransportNode.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/ReaderPlugin.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/nativese/NativeReaderServiceImpl.h"
#include "../../../../../../../../../../../keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"
#include "../../../../../../../../../../stub/src/main/java/org/eclipse/keyple/plugin/stub/StubReader.h"
#include "../../../../../../../../../../stub/src/main/java/org/eclipse/keyple/plugin/stub/StubPlugin.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/transport/TransportDto.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/transport/KeypleDtoHelper.h"
#include "../../../../../../../../main/java/org/eclipse/keyple/plugin/remotese/transport/java/LocalTransportDto.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace remotese {
                    namespace integration {
                        using NativeReaderServiceImpl = org::eclipse::keyple::plugin::remotese::nativese::NativeReaderServiceImpl;
                        using VirtualReaderService = org::eclipse::keyple::plugin::remotese::pluginse::VirtualReaderService;
                        using KeypleDtoHelper = org::eclipse::keyple::plugin::remotese::transport::KeypleDtoHelper;
                        using TransportDto = org::eclipse::keyple::plugin::remotese::transport::TransportDto;
                        using TransportNode = org::eclipse::keyple::plugin::remotese::transport::TransportNode;
                        using LocalTransportDto = org::eclipse::keyple::plugin::remotese::transport::java::LocalTransportDto;
                        using StubPlugin = org::eclipse::keyple::plugin::stub::StubPlugin;
                        using StubReader = org::eclipse::keyple::plugin::stub::StubReader;
                        using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using org::junit::Assert;
                        using org::mockito::Mockito;
                        using org::mockito::invocation::InvocationOnMock;
                        using org::mockito::stubbing::Answer;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> Integration::logger = org::slf4j::LoggerFactory::getLogger(Integration::typeid);

                        std::shared_ptr<VirtualReaderService> Integration::bindMaster(std::shared_ptr<TransportNode> node) {
                            // Create Master services : virtualReaderService
                            std::shared_ptr<VirtualReaderService> virtualReaderService = std::make_shared<VirtualReaderService>(SeProxyService::getInstance(), node);

                            // observe remote se plugin for events
                            std::shared_ptr<ReaderPlugin> rsePlugin = virtualReaderService->getPlugin();

                            // Binds virtualReaderService to a
                            virtualReaderService->bindDtoEndpoint(node);

                            return virtualReaderService;
                        }

                        std::shared_ptr<NativeReaderServiceImpl> Integration::bindSlave(std::shared_ptr<TransportNode> node) {
                            // Binds node for outgoing KeypleDto
                            std::shared_ptr<NativeReaderServiceImpl> nativeReaderService = std::make_shared<NativeReaderServiceImpl>(node);

                            // Binds node for incoming KeypleDTo
                            nativeReaderService->bindDtoEndpoint(node);

                            return nativeReaderService;
                        }

                        std::shared_ptr<NativeReaderServiceImpl> Integration::bindSlaveSpy(std::shared_ptr<TransportNode> node) {
                            // Binds node for outgoing KeypleDto
                            std::shared_ptr<NativeReaderServiceImpl> nativeReaderService = std::make_shared<NativeReaderServiceImpl>(node);

                            std::shared_ptr<NativeReaderServiceImpl> spy = Mockito::spy(nativeReaderService);

                            // Binds node for incoming KeypleDTo
                            spy->bindDtoEndpoint(node);

                            return spy;
                        }

                        std::shared_ptr<StubReader> Integration::createStubReader(const std::string &stubReaderName) throw(InterruptedException, KeypleReaderNotFoundException) {
                            std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();

                            std::shared_ptr<StubPlugin> stubPlugin = StubPlugin::getInstance();
                            seProxyService->addPlugin(stubPlugin);

                            // add an stubPluginObserver to start the plugin monitoring thread
                            // stubPlugin.addObserver(observer); //do not observe so the monitoring thread is not
                            // created

                            logger->debug("Stub plugin count observers : {}", stubPlugin->countObservers());

                            logger->debug("Create a new StubReader : {}", stubReaderName);
                            stubPlugin->plugStubReader(stubReaderName);

                            delay(100);

                            // get the created proxy reader
                            return std::static_pointer_cast<StubReader>(stubPlugin->getReader(stubReaderName));
                        }

                        std::shared_ptr<Answer<std::shared_ptr<TransportDto>>> Integration::assertContainsException() {
                            return std::make_shared<AnswerAnonymousInnerClass>();
                        }

                        std::shared_ptr<TransportDto> Integration::AnswerAnonymousInnerClass::answer(std::shared_ptr<InvocationOnMock> invocationOnMock) throw(std::runtime_error) {
                            std::shared_ptr<TransportDto> transportDto = invocationOnMock->getArgument(0);

                            // assert that returning dto DOES contain an exception
                            Assert::assertTrue(KeypleDtoHelper::containsException(transportDto->getKeypleDTO()));
                            return std::make_shared<LocalTransportDto>(KeypleDtoHelper::NoResponse(), nullptr);
                        }
                    }
                }
            }
        }
    }
}
