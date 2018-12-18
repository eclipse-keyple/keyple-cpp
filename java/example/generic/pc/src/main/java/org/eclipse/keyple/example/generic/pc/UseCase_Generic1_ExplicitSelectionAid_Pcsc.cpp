#include "UseCase_Generic1_ExplicitSelectionAid_Pcsc.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/NoStackTraceThrowable.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "ReaderUtilities.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelection.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelector.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/MatchingSe.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic_Renamed {
                    namespace pc {
                        using PcscPlugin = org::eclipse::keyple::plugin::pcsc::PcscPlugin;
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using SeProxyService = org::eclipse::keyple::seproxy::SeProxyService;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using MatchingSe = org::eclipse::keyple::transaction::MatchingSe;
                        using SeSelection = org::eclipse::keyple::transaction::SeSelection;
                        using SeSelector = org::eclipse::keyple::transaction::SeSelector;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> UseCase_Generic1_ExplicitSelectionAid_Pcsc::logger = org::slf4j::LoggerFactory::getLogger(UseCase_Generic1_ExplicitSelectionAid_Pcsc::typeid);
std::string UseCase_Generic1_ExplicitSelectionAid_Pcsc::seAid = "A0000004040125090101";

                        void UseCase_Generic1_ExplicitSelectionAid_Pcsc::main(std::vector<std::string> &args) throw(KeypleBaseException, InterruptedException, IOException, NoStackTraceThrowable) {

                            /* Get the instance of the SeProxyService (Singleton pattern) */
                            std::shared_ptr<SeProxyService> seProxyService = SeProxyService::getInstance();

                            /* Get the instance of the PC/SC plugin */
                            std::shared_ptr<PcscPlugin> pcscPlugin = PcscPlugin::getInstance();

                            /* Assign PcscPlugin to the SeProxyService */
                            seProxyService->addPlugin(pcscPlugin);

                            /*
                             * Get a SE reader ready to work with generic SE. Use the getReader helper method from the
                             * ReaderUtilities class.
                             */
                            std::shared_ptr<SeReader> seReader = ReaderUtilities::getDefaultContactLessSeReader(seProxyService);

                            /* Check if the reader exists */
                            if (seReader == nullptr) {
                                throw std::make_shared<IllegalStateException>("Bad SE reader setup");
                            }

                            logger->info("=============== UseCase Generic #1: AID based explicit selection ==================");
                            logger->info("= SE Reader  NAME = {}", seReader->getName());

                            /* Check if a SE is present in the reader */
                            if (seReader->isSePresent()) {

                                logger->info("==================================================================================");
                                logger->info("= AID based selection.                                                           =");
                                logger->info("==================================================================================");

                                /*
                                 * Prepare the SE selection
                                 */
                                std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>(seReader);

                                /*
                                 * Setting of an AID based selection (in this example a Calypso REV3 PO)
                                 *
                                 * Select the first application matching the selection AID whatever the SE communication
                                 * protocol keep the logical channel open after the selection
                                 */

                                /*
                                 * Generic selection: configures a SeSelector with all the desired attributes to make
                                 * the selection and read additional information afterwards
                                 */
                                std::shared_ptr<SeSelector> seSelector = std::make_shared<SeSelector>(ByteArrayUtils::fromHex(seAid), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4, "AID: " + seAid);

                                /*
                                 * Add the selection case to the current selection (we could have added other cases
                                 * here)
                                 */
                                std::shared_ptr<MatchingSe> matchingSe = seSelection->prepareSelection(seSelector);

                                /*
                                 * Actual SE communication: operate through a single request the SE selection
                                 */
                                if (seSelection->processExplicitSelection()) {
                                    logger->info("The selection of the SE has succeeded.");
                                    logger->info("Application FCI = {}", matchingSe->getSelectionSeResponse()->getSelectionStatus()->getFci());

                                    logger->info("==================================================================================");
                                    logger->info("= End of the generic SE processing.                                              =");
                                    logger->info("==================================================================================");
                                }
                                else {
                                    logger->error("The selection of the SE has failed.");
                                }
                            }
                            else {
                                logger->error("No SE were detected.");
                            }
                            exit(0);
                        }
                    }
                }
            }
        }
    }
}
