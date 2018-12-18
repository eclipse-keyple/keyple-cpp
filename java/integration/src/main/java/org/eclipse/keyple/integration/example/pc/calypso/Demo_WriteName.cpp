#include "Demo_WriteName.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/NoStackTraceThrowable.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "DemoUtilities.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelection.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/Protocol.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelector.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/CalypsoPo.h"
#include "../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoSelector.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../calypso/PoFileStructureInfo.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/MatchingSe.h"
#include "../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoTransaction.h"
#include "../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/TransmissionMode.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace integration {
                namespace example {
                    namespace pc {
                        namespace calypso {
                            using CalypsoPo = org::eclipse::keyple::calypso::transaction::CalypsoPo;
                            using PoSelector = org::eclipse::keyple::calypso::transaction::PoSelector;
                            using PoTransaction = org::eclipse::keyple::calypso::transaction::PoTransaction;
                            using PoFileStructureInfo = org::eclipse::keyple::integration::calypso::PoFileStructureInfo;
                            using PcscPlugin = org::eclipse::keyple::plugin::pcsc::PcscPlugin;
                            using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                            using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                            using SeReader = org::eclipse::keyple::seproxy::SeReader;
                            using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                            using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                            using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                            using Protocol = org::eclipse::keyple::seproxy::protocol::Protocol;
                            using TransmissionMode = org::eclipse::keyple::seproxy::protocol::TransmissionMode;
                            using MatchingSe = org::eclipse::keyple::transaction::MatchingSe;
                            using SeSelection = org::eclipse::keyple::transaction::SeSelection;
                            using SeSelector = org::eclipse::keyple::transaction::SeSelector;
                            using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                            using org::slf4j::Logger;
                            using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> Demo_WriteName::logger = org::slf4j::LoggerFactory::getLogger(Demo_WriteName::typeid);

                            void Demo_WriteName::main(std::vector<std::string> &args) throw(KeypleBaseException, NoStackTraceThrowable) {

                                /* Get the instance of the SeProxyService (Singleton pattern) */
                                std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();

                                /* Get the instance of the PC/SC plugin */
                                std::shared_ptr<PcscPlugin> pcscPlugin = PcscPlugin::getInstance();

                                /* Assign PcscPlugin to the SeProxyService */
                                seProxyService->addPlugin(pcscPlugin);

                                std::shared_ptr<SeReader> poReader = DemoUtilities::getReader(seProxyService, DemoUtilities::PO_READER_NAME_REGEX);
                                std::shared_ptr<SeReader> samReader = DemoUtilities::getReader(seProxyService, DemoUtilities::SAM_READER_NAME_REGEX);

                                /* Check if the readers exists */
                                if (poReader == nullptr || samReader == nullptr) {
                                    throw std::make_shared<IllegalStateException>("Bad PO or SAM reader setup");
                                }

                                logger->info("= PO Reader  NAME = {}", poReader->getName());
                                logger->info("= SAM Reader  NAME = {}", samReader->getName());

                                std::string SAM_ATR_REGEX = "3B3F9600805A[0-9a-fA-F]{2}80[0-9a-fA-F]{16}829000";

                                std::shared_ptr<SeSelection> samSelection = std::make_shared<SeSelection>(samReader);

                                std::shared_ptr<SeSelector> samSelector = std::make_shared<SeSelector>(SAM_ATR_REGEX, ChannelState::KEEP_OPEN, Protocol::ANY, "SAM Selection");

                                /* Prepare selector, ignore MatchingSe here */
                                samSelection->prepareSelection(samSelector);

                                try {
                                    if (!samSelection->processExplicitSelection()) {
                                        std::cout << "Unable to open a logical channel for SAM!" << std::endl;
                                        throw std::make_shared<IllegalStateException>("SAM channel opening failure");
                                    }
                                }
                                catch (const KeypleReaderException &e) {
                                    throw std::make_shared<IllegalStateException>("Reader exception: " + e->what());

                                }

                                /* Check if a PO is present in the reader */
                                if (poReader->isSePresent()) {

                                    /*
                                     * Prepare a Calypso PO selection
                                     */
                                    std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>(poReader);

                                    /*
                                     * Setting of an AID based selection of a Calypso REV3 PO
                                     *
                                     * Select the first application matching the selection AID whatever the SE communication
                                     * protocol keep the logical channel open after the selection
                                     */

                                    /*
                                     * Calypso selection: configures a PoSelector with all the desired attributes to make
                                     * the selection and read additional information afterwards
                                     */
                                    /* Calypso AID */
                                    std::string poAuditC0Aid = "315449432E4943414C54"; // AID of the PO with Audit C0 profile
                                    std::string clapAid = "315449432E494341D62010029101"; // AID of the CLAP product being tested
                                    std::string cdLightAid = "315449432E494341"; // AID of the Rev2.4 PO emulating CDLight

                                    // Add Audit C0 AID to the list
                                    std::shared_ptr<CalypsoPo> auditC0Se = std::static_pointer_cast<CalypsoPo>(seSelection->prepareSelection(std::make_shared<PoSelector>(ByteArrayUtils::fromHex(PoFileStructureInfo::poAuditC0Aid), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, Protocol::ANY, "Audit C0")));

                                    // Add CLAP AID to the list
                                    std::shared_ptr<CalypsoPo> clapSe = std::static_pointer_cast<CalypsoPo>(seSelection->prepareSelection(std::make_shared<PoSelector>(ByteArrayUtils::fromHex(PoFileStructureInfo::clapAid), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, Protocol::ANY, "CLAP")));

                                    // Add cdLight AID to the list
                                    std::shared_ptr<CalypsoPo> cdLightSe = std::static_pointer_cast<CalypsoPo>(seSelection->prepareSelection(std::make_shared<PoSelector>(ByteArrayUtils::fromHex(PoFileStructureInfo::cdLightAid), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, Protocol::ANY, "CDLight")));

                                    if (!seSelection->processExplicitSelection()) {
                                        throw std::invalid_argument("No recognizable PO detected.");
                                    }

                                    char environmentSid = static_cast<char>(0x00);

                                    if (auditC0Se->isSelected()) {
                                        environmentSid = static_cast<char>(0x07);
                                    }
                                    else if (clapSe->isSelected()) {
                                        environmentSid = static_cast<char>(0x14);

                                    }
                                    else if (cdLightSe->isSelected()) {
                                        environmentSid = static_cast<char>(0x07);
                                    }
                                    else {
                                        throw std::invalid_argument("No recognizable PO detected.");
                                    }

                                    /*
                                     * Actual PO communication: operate through a single request the Calypso PO selection
                                     * and the file read
                                     */
                                    logger->info("The selection of the PO has succeeded.");

                                    std::shared_ptr<MatchingSe> selectedSe = seSelection->getSelectedSe();

                                    std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(poReader, std::static_pointer_cast<CalypsoPo>(selectedSe), samReader, nullptr);

                                    std::string name = "CNA Keyple Demo";

                                    bool poProcessStatus = poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC, PoTransaction::SessionAccessLevel::SESSION_LVL_PERSO, static_cast<char>(0), static_cast<char>(0));

                                    if (!poProcessStatus) {
                                        throw std::make_shared<IllegalStateException>("processingOpening failure.");
                                    }


                                    poTransaction->prepareUpdateRecordCmd(environmentSid, static_cast<char>(0x01), name.getBytes(), "Environment");

                                    poProcessStatus = poTransaction->processClosing(TransmissionMode::CONTACTLESS, ChannelState::KEEP_OPEN);

                                    if (!poProcessStatus) {
                                        throw std::make_shared<IllegalStateException>("processClosing failure.");
                                    }

                                    logger->info("==================================================================================");
                                    logger->info("= End of the Calypso PO processing.                                              =");
                                    logger->info("==================================================================================");
                                }
                                else {
                                    logger->error("No PO were detected.");
                                }
                                exit(0);
                            }
                        }
                    }
                }
            }
        }
    }
}
