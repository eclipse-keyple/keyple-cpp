#include "UseCase_Generic3_MultiSelection_Pcsc.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/NoStackTraceThrowable.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "ReaderUtilities.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelector.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"

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
                        using namespace org::eclipse::keyple::transaction;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> UseCase_Generic3_MultiSelection_Pcsc::logger = org::slf4j::LoggerFactory::getLogger(UseCase_Generic1_ExplicitSelectionAid_Pcsc::typeid);

                        void UseCase_Generic3_MultiSelection_Pcsc::main(std::vector<std::string> &args) throw(KeypleBaseException, InterruptedException, IOException, NoStackTraceThrowable) {

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

                            logger->info("=============== UseCase Generic #3: AID based explicit selection ==================");
                            logger->info("= SE Reader  NAME = {}", seReader->getName());

                            std::vector<std::shared_ptr<MatchingSe>> matchingSeTable(3);

                            /* Check if a SE is present in the reader */
                            if (seReader->isSePresent()) {

                                std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>(seReader);

                                /* operate SE selection (change the AID here to adapt it to the SE used for the test) */
                                std::string seAidPrefix = "A000000404012509";

                                /* AID based selection */
                                matchingSeTable[0] = seSelection->prepareSelection(std::make_shared<SeSelector>(ByteArrayUtils::fromHex(seAidPrefix), SeSelector::SelectMode::FIRST, ChannelState::CLOSE_AFTER, ContactlessProtocols::PROTOCOL_ISO14443_4, "Initial selection #1"));
                                /* next selection */
                                matchingSeTable[1] = seSelection->prepareSelection(std::make_shared<SeSelector>(ByteArrayUtils::fromHex(seAidPrefix), SeSelector::SelectMode::NEXT, ChannelState::CLOSE_AFTER, ContactlessProtocols::PROTOCOL_ISO14443_4, "Next selection #2"));
                                /* next selection */
                                matchingSeTable[2] = seSelection->prepareSelection(std::make_shared<SeSelector>(ByteArrayUtils::fromHex(seAidPrefix), SeSelector::SelectMode::NEXT, ChannelState::CLOSE_AFTER, ContactlessProtocols::PROTOCOL_ISO14443_4, "Next selection #3"));
                                /*
                                 * Actual SE communication: operate through a single request the SE selection
                                 */
                                if (seSelection->processExplicitSelection()) {

                                    int matchedSelection = 0;
                                    /* Count the number of SE that matched the selection */
                                    for (int i = 0; i < matchingSeTable.size(); i++) {
                                        if (matchingSeTable[i]->getSelectionSeResponse() != nullptr) {
                                            matchedSelection++;
                                        }
                                    }
                                    logger->info("The SE matched {} time(s) the selection.", matchedSelection);

                                    for (int i = 0; i < matchingSeTable.size(); i++) {

                                        if (matchingSeTable[i]->getSelectionSeResponse() != nullptr) {
                                            logger->info("Selection status for case {}: \n\t\tATR: {}\n\t\tFCI: {}", i + 1, ByteArrayUtils::toHex(matchingSeTable[i]->getSelectionSeResponse().getSelectionStatus().getAtr().getBytes()), ByteArrayUtils::toHex(matchingSeTable[i]->getSelectionSeResponse().getSelectionStatus().getFci().getDataOut()));
                                        }
                                    }
                                }
                                else {
                                    logger->info("The selection process did not return any selected SE.");
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
