#include "UseCase_Generic3_GroupedMultiSelection_Pcsc.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/NoStackTraceThrowable.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "ReaderUtilities.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeSelector.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
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
                        using SeSelector = org::eclipse::keyple::seproxy::SeSelector;
                        using KeypleBaseException = org::eclipse::keyple::seproxy::exception::KeypleBaseException;
                        using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using namespace org::eclipse::keyple::transaction;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> UseCase_Generic3_GroupedMultiSelection_Pcsc::logger = org::slf4j::LoggerFactory::getLogger(UseCase_Generic1_ExplicitSelectionAid_Pcsc::typeid);

                        void UseCase_Generic3_GroupedMultiSelection_Pcsc::main(std::vector<std::string> &args) throw(KeypleBaseException, InterruptedException, IOException, NoStackTraceThrowable) {

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

                            logger->info("=============== UseCase Generic #3: AID based grouped explicit multiple selection ==================");
                            logger->info("= SE Reader  NAME = {}", seReader->getName());

                            std::vector<std::shared_ptr<MatchingSe>> matchingSeTable(3);

                            /* Check if a SE is present in the reader */
                            if (seReader->isSePresent()) {

                                /* CLOSE_AFTER pour assurer la sélection de toutes les applications */
                                std::shared_ptr<SeSelection> seSelection = std::make_shared<SeSelection>(seReader);

                                /* operate SE selection (change the AID here to adapt it to the SE used for the test) */
                                std::string seAidPrefix = "A000000404012509";

                                /* AID based selection */
                                matchingSeTable[0] = seSelection->prepareSelection(std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(seAidPrefix), nullptr), nullptr, "Initial selection #1"), ChannelState::CLOSE_AFTER, ContactlessProtocols::PROTOCOL_ISO14443_4));
                                /* next selection */
                                matchingSeTable[1] = seSelection->prepareSelection(std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(seAidPrefix), nullptr), nullptr, "Next selection #2"), ChannelState::CLOSE_AFTER, ContactlessProtocols::PROTOCOL_ISO14443_4));
                                /* next selection */
                                matchingSeTable[2] = seSelection->prepareSelection(std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(seAidPrefix), nullptr), nullptr, "Next selection #3"), ChannelState::CLOSE_AFTER, ContactlessProtocols::PROTOCOL_ISO14443_4));
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
