#include "Demo_CalypsoClassic_Stub.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ReaderPlugin.h"
#include "../../../../../../../../../../common/src/main/java/org/eclipse/keyple/example/calypso/common/transaction/CalypsoClassicTransactionEngine.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocolSetting.h"
#include "../../../../../../../../../../../../component/keyple-plugin/stub/src/main/java/org/eclipse/keyple/plugin/stub/StubProtocolSetting.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace calypso {
                    namespace pc {
                        using CalypsoClassicTransactionEngine = org::eclipse::keyple::example::calypso::common::transaction::CalypsoClassicTransactionEngine;
                        using namespace org::eclipse::keyple::example::calypso::pc::stub::se;
                        using namespace org::eclipse::keyple::plugin::stub;
                        using StubProtocolSetting = org::eclipse::keyple::plugin::stub::StubProtocolSetting;
                        using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;

                        void Demo_CalypsoClassic_Stub::main(std::vector<std::string> &args) throw(InterruptedException) {
                            std::shared_ptr<Logger> * const logger = LoggerFactory::getLogger(Demo_CalypsoClassic_Stub::typeid);

                            /* Get the instance of the SeProxyService (Singleton pattern) */
                            std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();

                            std::shared_ptr<SortedSet<std::shared_ptr<ReaderPlugin>>> pluginsSet = std::make_shared<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>>();

                            std::shared_ptr<StubPlugin> stubPlugin = StubPlugin::getInstance();

                            /* Get the instance of the PcscPlugin (Singleton pattern) */
                            pluginsSet->add(stubPlugin);

                            /* Assign StubPlugin to the SeProxyService */
                            seProxyService->setPlugins(pluginsSet);

                            /* Setting up the transaction engine (implements Observer) */
                            std::shared_ptr<CalypsoClassicTransactionEngine> transactionEngine = std::make_shared<CalypsoClassicTransactionEngine>();

                            /*
                             * Plug PO and SAM stub readers.
                             */
                            stubPlugin->plugStubReader("poReader");
                            stubPlugin->plugStubReader("samReader");

                            std::shared_ptr<StubReader> poReader = nullptr, samReader = nullptr;
                            try {
                                poReader = std::static_pointer_cast<StubReader>(stubPlugin->getReader("poReader"));
                                samReader = std::static_pointer_cast<StubReader>(stubPlugin->getReader("samReader"));
                            }
                            catch (const KeypleReaderNotFoundException &e) {
                                e->printStackTrace();
                            }

                            /* Both readers are expected not null */
                            if (poReader == samReader || poReader == nullptr || samReader == nullptr) {
                                throw std::make_shared<IllegalStateException>("Bad PO/SAM setup");
                            }

                            logger->info("PO Reader  NAME = {}", poReader->getName());
                            logger->info("SAM Reader  NAME = {}", samReader->getName());

                            /* Set the PO reader protocol flag */
                            poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4));
                            poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(StubProtocolSetting::SETTING_PROTOCOL_B_PRIME));

                            /* Assign readers to the Hoplink transaction engine */
                            transactionEngine->setReaders(poReader, samReader);

                            /* Create 'virtual' Hoplink and SAM SE */
                            std::shared_ptr<StubSecureElement> calypsoStubSe = std::make_shared<StubCalypsoClassic>();
                            std::shared_ptr<StubSecureElement> samSE = std::make_shared<StubSamCalypsoClassic>();

                            /* Insert the SAM into the SAM reader */
                            logger->info("Insert stub SAM SE.");
                            samReader->insertSe(samSE);

                            /* Set the default selection operation */
                            (std::static_pointer_cast<ObservableReader>(poReader))->setDefaultSelectionRequest(transactionEngine->preparePoSelection(), ObservableReader::NotificationMode::MATCHED_ONLY);

                            /* Set the transactionEngine as Observer of the PO reader */
                            (std::static_pointer_cast<ObservableReader>(poReader))->addObserver(transactionEngine);

                            logger->info("Insert stub PO SE.");
                            poReader->insertSe(calypsoStubSe);

                            delay(1000);

                            /* Remove SE */
                            logger->info("Remove stub SAM and PO SE.");

                            poReader->removeSe();
                            samReader->removeSe();

                            logger->info("END.");

                            exit(0);
                        }
                    }
                }
            }
        }
    }
}
