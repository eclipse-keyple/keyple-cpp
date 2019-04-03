#include "UseCase_Generic4_SequentialMultiSelection_Pcsc.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleReaderException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/KeypleBaseException.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/exception/NoStackTraceThrowable.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeProxyService.h"
#include "../../../../../../../../../../../../component/keyple-plugin/pcsc/src/main/java/org/eclipse/keyple/plugin/pcsc/PcscPlugin.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "ReaderUtilities.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeSelector.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"

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
                        using KeypleReaderException = org::eclipse::keyple::seproxy::exception::KeypleReaderException;
                        using NoStackTraceThrowable = org::eclipse::keyple::seproxy::exception::NoStackTraceThrowable;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using namespace org::eclipse::keyple::transaction;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;
                        using org::slf4j::Logger;
                        using org::slf4j::LoggerFactory;
const std::shared_ptr<org::slf4j::Logger> UseCase_Generic4_SequentialMultiSelection_Pcsc::logger = org::slf4j::LoggerFactory::getLogger(UseCase_Generic1_ExplicitSelectionAid_Pcsc::typeid);

                        void UseCase_Generic4_SequentialMultiSelection_Pcsc::doAndAnalyseSelection(std::shared_ptr<SeSelection> seSelection, std::shared_ptr<MatchingSe> matchingSe, int index) throw(KeypleReaderException) {
                            if (seSelection->processExplicitSelection()) {
                                logger->info("The SE matched the selection {}.", index);

                                if (matchingSe->getSelectionSeResponse() != nullptr) {
                                    logger->info("Selection status for case {}: \n\t\tATR: {}\n\t\tFCI: {}", index, ByteArrayUtils::toHex(matchingSe->getSelectionSeResponse()->getSelectionStatus()->getAtr()->getBytes()), ByteArrayUtils::toHex(matchingSe->getSelectionSeResponse()->getSelectionStatus()->getFci()->getDataOut()));
                                }
                            }
                            else {
                                logger->info("The selection 2 process did not return any selected SE.");
                            }
                        }

                        void UseCase_Generic4_SequentialMultiSelection_Pcsc::main(std::vector<std::string> &args) throw(KeypleBaseException, InterruptedException, IOException, NoStackTraceThrowable) {

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

                            logger->info(std::string("=============== UseCase Generic #4: AID based sequential explicit multiple selection ") + "==================");
                            logger->info("= SE Reader  NAME = {}", seReader->getName());

                            std::shared_ptr<MatchingSe> matchingSe;

                            /* Check if a SE is present in the reader */
                            if (seReader->isSePresent()) {

                                std::shared_ptr<SeSelection> seSelection;

                                seSelection = std::make_shared<SeSelection>(seReader);

                                /* operate SE selection (change the AID here to adapt it to the SE used for the test) */
                                std::string seAidPrefix = "A000000404012509";

                                /* AID based selection */
                                matchingSe = seSelection->prepareSelection(std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(seAidPrefix), nullptr, SeSelector::AidSelector::FileOccurrence::FIRST, SeSelector::AidSelector::FileControlInformation::FCI), nullptr, "Initial selection #1"), ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4));

                                seSelection = std::make_shared<SeSelection>(seReader);

                                doAndAnalyseSelection(seSelection, matchingSe, 1);

                                /* next selection */
                                matchingSe = seSelection->prepareSelection(std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(seAidPrefix), nullptr, SeSelector::AidSelector::FileOccurrence::NEXT, SeSelector::AidSelector::FileControlInformation::FCI), nullptr, "Next selection #2"), ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4));

                                seSelection = std::make_shared<SeSelection>(seReader);

                                doAndAnalyseSelection(seSelection, matchingSe, 2);

                                /* next selection */
                                matchingSe = seSelection->prepareSelection(std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(seAidPrefix), nullptr, SeSelector::AidSelector::FileOccurrence::NEXT, SeSelector::AidSelector::FileControlInformation::FCI), nullptr, "Next selection #3"), ChannelState::CLOSE_AFTER, ContactlessProtocols::PROTOCOL_ISO14443_4));

                                doAndAnalyseSelection(seSelection, matchingSe, 3);

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
