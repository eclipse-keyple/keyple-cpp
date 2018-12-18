#include "TestEngine.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "PoFileStructureInfo.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelection.h"
#include "../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoSelector.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/Protocol.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelector.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ReaderPlugin.h"
#include "../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.h"
#include "../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscReader.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/SeProtocolSetting.h"
#include "../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscProtocolSetting.h"
#include "../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/NoStackTraceThrowable.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace integration {
                namespace calypso {
                    using PoSelector = org::eclipse::keyple::calypso::transaction::PoSelector;
                    using PcscPlugin = org::eclipse::keyple::plugin::pcsc::PcscPlugin;
                    using PcscProtocolSetting = org::eclipse::keyple::plugin::pcsc::PcscProtocolSetting;
                    using PcscReader = org::eclipse::keyple::plugin::pcsc::PcscReader;
                    using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                    using ReaderPlugin = org::eclipse::keyple::seproxy::ReaderPlugin;
                    using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                    using SeReader = org::eclipse::keyple::seproxy::SeReader;
                    using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                    using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                    using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                    using Protocol = org::eclipse::keyple::seproxy::protocol::Protocol;
                    using SeProtocolSetting = org::eclipse::keyple::seproxy::protocol::SeProtocolSetting;
                    using SeSelection = org::eclipse::keyple::transaction::SeSelection;
                    using SeSelector = org::eclipse::keyple::transaction::SeSelector;
                    using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
std::shared_ptr<org::eclipse::keyple::seproxy::SeReader> poReader, TestEngine::samReader;

                    std::shared_ptr<PoFileStructureInfo> TestEngine::selectPO() throw(std::invalid_argument, KeypleReaderException) {

                        std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>(poReader);

                        // Add Audit C0 AID to the list
                        seSelection->prepareSelection(std::make_shared<PoSelector>(ByteArrayUtils::fromHex(PoFileStructureInfo::poAuditC0Aid), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, Protocol::ANY, "Audit C0"));

                        // Add CLAP AID to the list
                        seSelection->prepareSelection(std::make_shared<PoSelector>(ByteArrayUtils::fromHex(PoFileStructureInfo::clapAid), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, Protocol::ANY, "CLAP"));

                        // Add cdLight AID to the list
                        seSelection->prepareSelection(std::make_shared<PoSelector>(ByteArrayUtils::fromHex(PoFileStructureInfo::cdLightAid), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, Protocol::ANY, "CDLight"));

                        if (seSelection->processExplicitSelection()) {
                            return std::make_shared<PoFileStructureInfo>(seSelection->getSelectedSe());
                        }

                        throw std::invalid_argument("No recognizable PO detected.");
                    }

                    std::shared_ptr<SeReader> TestEngine::getReader(std::shared_ptr<SeProxyService> seProxyService, const std::string &pattern) throw(KeypleReaderException) {

                        std::shared_ptr<Pattern> p = Pattern::compile(pattern);
                        for (auto plugin : seProxyService->getPlugins()) {
                            for (auto reader : plugin->getReaders()) {
                                if (p->matcher(reader->getName()).matches()) {
                                    return reader;
                                }
                            }
                        }
                        return nullptr;
                    }

                    void TestEngine::configureReaders() throw(IOException, InterruptedException, KeypleBaseException) {

                        std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();
                        std::shared_ptr<SortedSet<std::shared_ptr<ReaderPlugin>>> pluginsSet = std::make_shared<ConcurrentSkipListSet<std::shared_ptr<ReaderPlugin>>>();
                        pluginsSet->add(PcscPlugin::getInstance());
                        seProxyService->setPlugins(pluginsSet);

                        std::string PO_READER_NAME_REGEX = ".*(ASK|ACS).*";
                        std::string SAM_READER_NAME_REGEX = ".*(Cherry TC|SCM Microsystems|Identive|HID).*";

                        poReader = getReader(seProxyService, PO_READER_NAME_REGEX);
                        samReader = getReader(seProxyService, SAM_READER_NAME_REGEX);


                        if (poReader == samReader || poReader == nullptr || samReader == nullptr) {
                            throw std::make_shared<IllegalStateException>("Bad PO/SAM setup");
                        }

                        std::cout << "\n==================================================================================" << std::endl;
                        std::cout << "PO Reader  : " << poReader->getName() << std::endl;
                        std::cout << "SAM Reader : " << samReader->getName() << std::endl;
                        std::cout << "==================================================================================" << std::endl;

                        poReader->setParameter(PcscReader::SETTING_KEY_PROTOCOL, PcscReader::SETTING_PROTOCOL_T1);
                        samReader->setParameter(PcscReader::SETTING_KEY_PROTOCOL, PcscReader::SETTING_PROTOCOL_T0);

                        // provide the reader with the map
                        poReader->addSeProtocolSetting(std::make_shared<SeProtocolSetting>(PcscProtocolSetting::SETTING_PROTOCOL_ISO14443_4));

                        try {
                            if (!samReader->isSePresent()) {
                                throw std::make_shared<IllegalStateException>("No SAM present in the reader.");
                            }
                        }
                        catch (const NoStackTraceThrowable &noStackTraceThrowable) {
                            throw std::make_shared<KeypleReaderException>("Exception raised while checking SE presence.");
                        }

                        // operate PO multiselection
                        std::string SAM_ATR_REGEX = "3B3F9600805A[0-9a-fA-F]{2}80[0-9a-fA-F]{16}829000";

                        // check the availability of the SAM, open its physical and logical channels and keep it
                        // open
                        std::shared_ptr<SeSelection> samSelection = std::make_shared<SeSelection>(samReader);

                        std::shared_ptr<SeSelector> samSelector = std::make_shared<SeSelector>(SAM_ATR_REGEX, ChannelState::KEEP_OPEN, Protocol::ANY, "SAM Selection");

                        /* Prepare selector, ignore MatchingSe here */
                        samSelection->prepareSelection(samSelector);

                        try {
                            if (!samSelection->processExplicitSelection()) {
                                std::cout << "Unable to open a logical channel for SAM!" << std::endl;
                                throw std::make_shared<IllegalStateException>("SAM channel opening failure");
                            }
                            else {
                            }
                        }
                        catch (const KeypleReaderException &e) {
                            throw std::make_shared<IllegalStateException>("Reader exception: " + e->what());

                        }


                    }
                }
            }
        }
    }
}
