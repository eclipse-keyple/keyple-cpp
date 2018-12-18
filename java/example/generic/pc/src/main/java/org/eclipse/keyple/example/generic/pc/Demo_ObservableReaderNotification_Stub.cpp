#include "Demo_ObservableReaderNotification_Stub.h"
#include "../../../../../../../../../../common/src/main/java/org/eclipse/keyple/example/generic/common/ObservableReaderNotificationEngine.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ReaderPlugin.h"
#include "../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubPlugin.h"
#include "../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubReader.h"
#include "../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubSecureElement.h"
#include "stub/se/StubSe1.h"
#include "stub/se/StubSe2.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic_Renamed {
                    namespace pc {
                        using ObservableReaderNotificationEngine = org::eclipse::keyple::example::generic_Renamed::common::ObservableReaderNotificationEngine;
                        using StubSe1 = org::eclipse::keyple::example::generic_Renamed::pc::stub::se::StubSe1;
                        using StubSe2 = org::eclipse::keyple::example::generic_Renamed::pc::stub::se::StubSe2;
                        using StubPlugin = org::eclipse::keyple::plugin::stub::StubPlugin;
                        using StubReader = org::eclipse::keyple::plugin::stub::StubReader;
                        using StubSecureElement = org::eclipse::keyple::plugin::stub::StubSecureElement;
                        using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
const std::shared_ptr<void> Demo_ObservableReaderNotification_Stub::waitBeforeEnd = nullptr;

                        void Demo_ObservableReaderNotification_Stub::main(std::vector<std::string> &args) throw(std::runtime_error) {
                            std::shared_ptr<ObservableReaderNotificationEngine> demoEngine = std::make_shared<ObservableReaderNotificationEngine>();

                            // Set Stub plugin
                            std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();
                            std::shared_ptr<SortedSet<std::shared_ptr<ReaderPlugin>>> pluginsSet = std::make_shared<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>>();

                            std::shared_ptr<StubPlugin> stubPlugin = StubPlugin::getInstance();
                            pluginsSet->add(stubPlugin);
                            seProxyService->setPlugins(pluginsSet);

                            // Set observers
                            std::cout << "Set plugin observer." << std::endl;
                            demoEngine->setPluginObserver();

                            std::cout << "Wait a little to see the \"no reader available message\"." << std::endl;
                            delay(200);

                            std::cout << "Plug reader 1." << std::endl;
                            stubPlugin->plugStubReader("Reader1");

                            delay(100);

                            std::cout << "Plug reader 2." << std::endl;
                            stubPlugin->plugStubReader("Reader2");

                            delay(1000);

                            std::shared_ptr<StubReader> reader1 = std::static_pointer_cast<StubReader>(stubPlugin->getReader("Reader1"));

                            std::shared_ptr<StubReader> reader2 = std::static_pointer_cast<StubReader>(stubPlugin->getReader("Reader2"));

                            /* Create 'virtual' Hoplink and SAM SE */
                            std::shared_ptr<StubSecureElement> se1 = std::make_shared<StubSe1>();
                            std::shared_ptr<StubSecureElement> se2 = std::make_shared<StubSe2>();

                            std::cout << "Insert SE into reader 1." << std::endl;
                            reader1->insertSe(se1);

                            delay(100);

                            std::cout << "Insert SE into reader 2." << std::endl;
                            reader2->insertSe(se2);

                            delay(100);

                            std::cout << "Remove SE from reader 1." << std::endl;
                            reader1->removeSe();

                            delay(100);

                            std::cout << "Remove SE from reader 2." << std::endl;
                            reader2->removeSe();

                            delay(100);

                            std::cout << "Plug reader 1 again (twice)." << std::endl;
                            stubPlugin->plugStubReader("Reader1");

                            std::cout << "Unplug reader 1." << std::endl;
                            stubPlugin->unplugReader("Reader1");

                            delay(100);

                            std::cout << "Plug reader 1 again." << std::endl;
                            stubPlugin->plugStubReader("Reader1");

                            delay(100);

                            std::cout << "Unplug reader 2." << std::endl;
                            stubPlugin->unplugReader("Reader2");

                            delay(100);

                            std::cout << "Unplug reader 2." << std::endl;
                            stubPlugin->unplugReader("Reader1");

                            std::cout << "END." << std::endl;

                            exit(0);
                        }
                    }
                }
            }
        }
    }
}
