#include "Demo_ObservableReaderNotification_Pcsc.h"
#include "../../../../../../../../../../common/src/main/java/org/eclipse/keyple/example/generic/common/ObservableReaderNotificationEngine.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic_Renamed {
                    namespace pc {
                        using ObservableReaderNotificationEngine = org::eclipse::keyple::example::generic_Renamed::common::ObservableReaderNotificationEngine;
                        using PcscPlugin = org::eclipse::keyple::plugin::pcsc::PcscPlugin;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
const std::shared_ptr<void> Demo_ObservableReaderNotification_Pcsc::waitBeforeEnd = nullptr;

                        void Demo_ObservableReaderNotification_Pcsc::main(std::vector<std::string> &args) throw(std::runtime_error) {
                            std::shared_ptr<ObservableReaderNotificationEngine> demoEngine = std::make_shared<ObservableReaderNotificationEngine>();

                            /* Instantiate SeProxyService and add PC/SC plugin */
                            std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();

                            seProxyService->addPlugin(PcscPlugin::getInstance());

                            /* Set observers */
                            demoEngine->setPluginObserver();

                            std::cout << "Wait for reader or SE insertion/removal" << std::endl;

                            /* Wait indefinitely. CTRL-C to exit. */
//JAVA TO C++ CONVERTER TODO TASK: Multithread locking is not converted to native C++ unless you choose one of the options on the 'Modern C++ Options' dialog:
                            synchronized(waitBeforeEnd) {
                                waitBeforeEnd->wait();
                            }
                        }
                    }
                }
            }
        }
    }
}
