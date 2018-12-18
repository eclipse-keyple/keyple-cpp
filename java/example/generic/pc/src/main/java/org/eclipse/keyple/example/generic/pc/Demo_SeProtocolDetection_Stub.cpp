#include "Demo_SeProtocolDetection_Stub.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ReaderPlugin.h"
#include "../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubPlugin.h"
#include "../../../../../../../../../../common/src/main/java/org/eclipse/keyple/example/generic/common/SeProtocolDetectionEngine.h"
#include "../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubReader.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocolSetting.h"
#include "../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubProtocolSetting.h"
#include "../../../../../../../../../../common/src/main/java/org/eclipse/keyple/example/generic/common/CustomProtocolSetting.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocol.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic_Renamed {
                    namespace pc {
                        using CustomProtocolSetting = org::eclipse::keyple::example::generic_Renamed::common::CustomProtocolSetting;
                        using SeProtocolDetectionEngine = org::eclipse::keyple::example::generic_Renamed::common::SeProtocolDetectionEngine;
                        using namespace org::eclipse::keyple::example::generic_Renamed::pc::stub::se;
                        using StubPlugin = org::eclipse::keyple::plugin::stub::StubPlugin;
                        using StubProtocolSetting = org::eclipse::keyple::plugin::stub::StubProtocolSetting;
                        using StubReader = org::eclipse::keyple::plugin::stub::StubReader;
                        using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                        using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;

//JAVA TO C++ CONVERTER TODO TASK: No base class can be determined:
                        Demo_SeProtocolDetection_Stub::Demo_SeProtocolDetection_Stub() {
                        }

                        void Demo_SeProtocolDetection_Stub::main(std::vector<std::string> &args) throw(std::invalid_argument, InterruptedException) {
                            /* get the SeProxyService instance */
                            std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();

                            /* add the PcscPlugin to the SeProxyService */
                            std::shared_ptr<SortedSet<std::shared_ptr<ReaderPlugin>>> pluginsSet = std::make_shared<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>>();

                            std::shared_ptr<StubPlugin> stubPlugin = StubPlugin::getInstance();

                            pluginsSet->add(stubPlugin);

                            seProxyService->setPlugins(pluginsSet);

                            /* create an observer class to handle the SE operations */
                            std::shared_ptr<SeProtocolDetectionEngine> observer = std::make_shared<SeProtocolDetectionEngine>();

                            /*
                             * Plug PO reader.
                             */
                            stubPlugin->plugStubReader("poReader");

                            delay(200);

                            std::shared_ptr<StubReader> poReader = nullptr, samReader = nullptr;
                            try {
                                poReader = std::static_pointer_cast<StubReader>(stubPlugin->getReader("poReader"));
                            }
                            catch (const KeypleReaderNotFoundException &e) {
                                e->printStackTrace();
                            }

                            observer->setReader(poReader);

                            // Protocol detection settings.
                            // add 8 expected protocols with three different methods:
                            // - using addSeProtocolSetting
                            // - using a custom enum
                            // - using a protocol map and addSeProtocolSetting
                            // A real application should use only one method.

                            // Method 1
                            // add protocols individually
                            poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_MEMORY_ST25));


                            poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4));


                            // Method 2
                            // add all settings at once with setting enum
                            poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(CustomProtocolSetting::values()));

                            // Method 3
                            // create and fill a protocol map
                            std::unordered_map<std::shared_ptr<SeProtocol>, std::string> protocolsMap;

                            protocolsMap.emplace(ContactlessProtocols::PROTOCOL_MIFARE_CLASSIC, StubProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_MIFARE_CLASSIC);

                            protocolsMap.emplace(ContactlessProtocols::PROTOCOL_MIFARE_UL, StubProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_MIFARE_UL);

                            // provide the reader with the map
                            poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(protocolsMap));

                            // Set terminal as Observer of the first reader
                            (std::static_pointer_cast<ObservableReader>(poReader))->addObserver(observer);

                            // poReader.insertSe(new StubCalypsoClassic());
                            //
                            // Thread.sleep(300);
                            //
                            // poReader.removeSe();

                            // Thread.sleep(100);
                            //
                            // poReader.insertSe(new StubCalypsoBPrime());

                            delay(300);

                            poReader->removeSe();

                            delay(100);

                            poReader->insertSe(std::make_shared<StubMifareClassic>());

                            delay(300);

                            poReader->removeSe();

                            delay(100);

                            /* insert Mifare UltraLight */
                            poReader->insertSe(std::make_shared<StubMifareUL>());

                            delay(300);

                            poReader->removeSe();

                            delay(100);

                            /* insert Mifare Desfire */
                            poReader->insertSe(std::make_shared<StubMifareDesfire>());

                            delay(300);

                            poReader->removeSe();

                            delay(100);



                            exit(0);
                        }
                    }
                }
            }
        }
    }
}
