#include "Demo_CalypsoClassic_Pcsc.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.h"
#include "../../../../../../../../../../common/src/main/java/org/eclipse/keyple/example/calypso/common/transaction/CalypsoClassicTransactionEngine.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "PcscReadersSettings.h"
#include "../../../../../../../../../../../generic/pc/src/main/java/org/eclipse/keyple/example/generic/pc/ReaderUtilities.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderNotFoundException.h"
#include "../../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscReader.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocolSetting.h"
#include "../../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscProtocolSetting.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/event/ObservableReader.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace calypso {
                    namespace pc {
                        using CalypsoClassicTransactionEngine = org::eclipse::keyple::example::calypso::common::transaction::CalypsoClassicTransactionEngine;
                        using ReaderUtilities = org::eclipse::keyple::example::generic_Renamed::pc::ReaderUtilities;
                        using PcscPlugin = org::eclipse::keyple::plugin::pcsc::PcscPlugin;
                        using PcscProtocolSetting = org::eclipse::keyple::plugin::pcsc::PcscProtocolSetting;
                        using PcscReader = org::eclipse::keyple::plugin::pcsc::PcscReader;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using ObservableReader = org::eclipse::keyple::seproxy::event_Renamed::ObservableReader;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using KeypleReaderNotFoundException = org::eclipse::keyple::seproxy::exception::KeypleReaderNotFoundException;
                        using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<void> Demo_CalypsoClassic_Pcsc::waitForEnd = nullptr;

                        void Demo_CalypsoClassic_Pcsc::main(std::vector<std::string> &args) throw(KeypleBaseException, InterruptedException) {
                            std::shared_ptr<Logger> logger = LoggerFactory::getLogger(Demo_CalypsoClassic_Pcsc::typeid);

                            /* Get the instance of the SeProxyService (Singleton pattern) */
                            std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();

                            /* Get the instance of the PC/SC plugin */
                            std::shared_ptr<PcscPlugin> pcscPlugin = PcscPlugin::getInstance();

                            /* Assign PcscPlugin to the SeProxyService */
                            seProxyService->addPlugin(pcscPlugin);

                            /* Setting up the transaction engine (implements Observer) */
                            std::shared_ptr<CalypsoClassicTransactionEngine> transactionEngine = std::make_shared<CalypsoClassicTransactionEngine>();

                            /*
                             * Get PO and SAM readers. Apply regulars expressions to reader names to select PO / SAM
                             * readers. Use the getReader helper method from the transaction engine.
                             */
                            std::shared_ptr<SeReader> poReader = nullptr, samReader = nullptr;
                            try {
                                poReader = ReaderUtilities::getReaderByName(seProxyService, PcscReadersSettings::PO_READER_NAME_REGEX);
                                samReader = ReaderUtilities::getReaderByName(seProxyService, PcscReadersSettings::SAM_READER_NAME_REGEX);
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

                            /* Set PcSc settings per reader */
                            poReader->setParameter(PcscReader::SETTING_KEY_LOGGING, "true");
                            poReader->setParameter(PcscReader::SETTING_KEY_PROTOCOL, PcscReader::SETTING_PROTOCOL_T1);
                            samReader->setParameter(PcscReader::SETTING_KEY_LOGGING, "true");
                            samReader->setParameter(PcscReader::SETTING_KEY_PROTOCOL, PcscReader::SETTING_PROTOCOL_T0);

                            /*
                             * PC/SC card access mode:
                             *
                             * The SAM is left in the SHARED mode (by default) to avoid automatic resets due to the
                             * limited time between two consecutive exchanges granted by Windows.
                             *
                             * This point will be addressed in a coming release of the Keyple PcSc reader plugin.
                             *
                             * The PO reader is set to EXCLUSIVE mode to avoid side effects (on OS Windows 8+) during
                             * the selection step that may result in session failures.
                             *
                             * See KEYPLE-CORE.PC.md file to learn more about this point.
                             *
                             */
                            samReader->setParameter(PcscReader::SETTING_KEY_MODE, PcscReader::SETTING_MODE_SHARED);
                            poReader->setParameter(PcscReader::SETTING_KEY_MODE, PcscReader::SETTING_MODE_SHARED);

                            /* Set the PO reader protocol flag */
                            poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(PcscProtocolSetting::SETTING_PROTOCOL_ISO14443_4));
                            poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(PcscProtocolSetting::SETTING_PROTOCOL_B_PRIME));

                            /* Assign the readers to the Calypso transaction engine */
                            transactionEngine->setReaders(poReader, samReader);

                            /* Set the default selection operation */
                            (std::static_pointer_cast<ObservableReader>(poReader))->setDefaultSelectionRequest(transactionEngine->preparePoSelection(), ObservableReader::NotificationMode::MATCHED_ONLY);

                            /* Set terminal as Observer of the first reader */
                            (std::static_pointer_cast<ObservableReader>(poReader))->addObserver(transactionEngine);

                            /* Wait for ever (exit with CTRL-C) */
//JAVA TO C++ CONVERTER TODO TASK: Multithread locking is not converted to native C++ unless you choose one of the options on the 'Modern C++ Options' dialog:
                            synchronized(waitForEnd) {
                                waitForEnd->wait();
                            }
                        }
                    }
                }
            }
        }
    }
}
