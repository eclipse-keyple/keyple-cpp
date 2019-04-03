/* Example - Generic - Common */
#include "ObservableReaderNotificationEngine.h"

/* Core - Seproxy */
#include "SeProxyService.h"
#include "ReaderPlugin.h"

/* Plugin - Stub */
#include "StubPlugin.h"
#include "StubReader.h"
#include "StubSecureElement.h"
#include "StubSe1.h"
#include "StubSe2.h"


                        using ObservableReaderNotificationEngine = org::eclipse::keyple::example::generic::
                            common::ObservableReaderNotificationEngine;
                        using StubSe1 = org::eclipse::keyple::example::generic::pc::stub::se::StubSe1;
                        using StubSe2 = org::eclipse::keyple::example::generic::pc::stub::se::StubSe2;
                        using StubPlugin        = org::eclipse::keyple::plugin::stub::StubPlugin;
                        using StubReader        = org::eclipse::keyple::plugin::stub::StubReader;
                        using StubSecureElement = org::eclipse::keyple::plugin::stub::StubSecureElement;
                        using ReaderPlugin      = org::eclipse::keyple::seproxy::ReaderPlugin;
                        using SeProxyService    = org::eclipse::keyple::seproxy::SeProxyService;
                        using ObservablePlugin  = org::eclipse::keyple::seproxy::event::ObservablePlugin;

                        static const std::shared_ptr<void> waitBeforeEnd = nullptr;

                        int main(int argc, char **argv) throw(std::runtime_error)
                        {
                            std::shared_ptr<ObservableReaderNotificationEngine> demoEngine =
                                std::make_shared<ObservableReaderNotificationEngine>();

                            /*
                             * Alex: diamond issue, casting PcscPlugin into ReaderPlugin can take two
                             * routes:
                             * - PcscPlugin -> AbstractThreadedObservablePlugin ->
                             *   AbstractObservablePlugin -> ReaderPlugin
                             * or
                             * - PcscPlugin -> AbstractThreadedObservablePlugin -> ObservablePlugin ->
                             *   ReaderPlugin
                             *
                             * Forcing conversion to ObservablePlugin for now but should be fixed or at
                             * least validated.
                             */
                            SeProxyService seProxyService = SeProxyService::getInstance();
                            std::set<std::shared_ptr<ReaderPlugin>> pluginsSet = std::set<std::shared_ptr<ReaderPlugin>>();

                            
                            StubPlugin stubPlugin = StubPlugin::getInstance();
                            stubPlugin.initReaders();
                            pluginsSet.insert(std::dynamic_pointer_cast<ReaderPlugin>(
                                std::dynamic_pointer_cast<ObservablePlugin>(std::make_shared<StubPlugin>(stubPlugin))));
                            seProxyService.setPlugins(pluginsSet);

                            // Set observers
                            std::cout << "Set plugin observer." << std::endl;
                            demoEngine->setPluginObserver();

                            std::cout << "Wait a little to see the \"no reader available message\"."
                                      << std::endl;
                            Thread::sleep(200);

                            std::cout << "Plug reader 1." << std::endl;
                            stubPlugin.plugStubReader("Reader1", true);

                            Thread::sleep(100);

                            std::cout << "Plug reader 2." << std::endl;
                            stubPlugin.plugStubReader("Reader2", true);

                            Thread::sleep(1000);

                            std::shared_ptr<StubReader> reader1 =
                                std::dynamic_pointer_cast<StubReader>(stubPlugin.getReader("Reader1"));

                            std::shared_ptr<StubReader> reader2 =
                                std::dynamic_pointer_cast<StubReader>(stubPlugin.getReader("Reader2"));

                            /* Create 'virtual' Hoplink and SAM SE */
                            std::shared_ptr<StubSecureElement> se1 = std::make_shared<StubSe1>();
                            std::shared_ptr<StubSecureElement> se2 = std::make_shared<StubSe2>();

                            std::cout << "Insert SE into reader 1." << std::endl;
                            reader1->insertSe(se1);

                            Thread::sleep(100);

                            std::cout << "Insert SE into reader 2." << std::endl;
                            reader2->insertSe(se2);

                            Thread::sleep(100);

                            std::cout << "Remove SE from reader 1." << std::endl;
                            reader1->removeSe();

                            Thread::sleep(100);

                            std::cout << "Remove SE from reader 2." << std::endl;
                            reader2->removeSe();

                            Thread::sleep(100);

                            std::cout << "Plug reader 1 again (twice)." << std::endl;
                            stubPlugin.plugStubReader("Reader1", true);

                            std::cout << "Unplug reader 1." << std::endl;
                            stubPlugin.unplugStubReader("Reader1", true);

                            Thread::sleep(100);

                            std::cout << "Plug reader 1 again." << std::endl;
                            stubPlugin.plugStubReader("Reader1", true);

                            Thread::sleep(100);

                            std::cout << "Unplug reader 1." << std::endl;
                            stubPlugin.unplugStubReader("Reader1", true);

                            Thread::sleep(100);

                            std::cout << "Unplug reader 2." << std::endl;
                            stubPlugin.unplugStubReader("Reader2", true);

                            std::cout << "END." << std::endl;

                            return 0;
                        }

