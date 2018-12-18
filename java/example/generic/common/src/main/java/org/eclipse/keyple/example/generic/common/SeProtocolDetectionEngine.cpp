#include "SeProtocolDetectionEngine.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/SeReader.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/protocol/ContactlessProtocols.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/transaction/PoSelector.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/ChannelState.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/transaction/SeSelector.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/util/ByteArrayUtils.h"
#include "../../../../../../../../../../../../component/keyple-core/src/main/java/org/eclipse/keyple/seproxy/message/ApduRequest.h"
#include "../../../../../../../../../../../../component/keyple-calypso/src/main/java/org/eclipse/keyple/calypso/command/po/parser/ReadDataStructure.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic_Renamed {
                    namespace common {
                        using ReadDataStructure = org::eclipse::keyple::calypso::command::po::parser::ReadDataStructure;
                        using PoSelector = org::eclipse::keyple::calypso::transaction::PoSelector;
                        using ChannelState = org::eclipse::keyple::seproxy::ChannelState;
                        using SeReader = org::eclipse::keyple::seproxy::SeReader;
                        using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using namespace org::eclipse::keyple::transaction;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                        SeProtocolDetectionEngine::SeProtocolDetectionEngine() : AbstractReaderObserverEngine() {
                        }

                        void SeProtocolDetectionEngine::setReader(std::shared_ptr<SeReader> poReader) {
                            this->poReader = poReader;
                        }

                        std::shared_ptr<SelectionRequest> SeProtocolDetectionEngine::prepareSeSelection() {

                            seSelection = std::make_shared<SeSelection>(poReader);

                            // process SDK defined protocols
                            for (ContactlessProtocols protocol : ContactlessProtocols::values()) {
                                switch (protocol.innerEnumValue) {
                                    case ContactlessProtocols::InnerEnum::PROTOCOL_ISO14443_4: {
                                        /* Add a Hoplink selector */
                                        std::string HoplinkAID = "A000000291A000000191";
                                        char SFI_T2Usage = static_cast<char>(0x1A);
                                        char SFI_T2Environment = static_cast<char>(0x14);

                                        std::shared_ptr<PoSelector> poSelector = std::make_shared<PoSelector>(ByteArrayUtils::fromHex(HoplinkAID), SeSelector::SelectMode::FIRST, ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4, "Hoplink selector");

                                        poSelector->preparePoCustomReadCmd("Standard Get Data", std::make_shared<ApduRequest>(ByteArrayUtils::fromHex("FFCA000000"), false));

                                        poSelector->prepareReadRecordsCmd(SFI_T2Environment, ReadDataStructure::SINGLE_RECORD_DATA, static_cast<char>(0x01), "Hoplink T2 Environment");

                                        seSelection->prepareSelection(poSelector);

                                        break;
                                    }
                                    case ContactlessProtocols::InnerEnum::PROTOCOL_ISO14443_3A:
                                    case ContactlessProtocols::InnerEnum::PROTOCOL_ISO14443_3B:
                                        // not handled in this demo code
                                        break;
                                    case ContactlessProtocols::InnerEnum::PROTOCOL_MIFARE_DESFIRE:
                                    case ContactlessProtocols::InnerEnum::PROTOCOL_B_PRIME:
                                        // intentionally ignored for demo purpose
                                        break;
                                    default:
                                        /* Add a generic selector */
                                        seSelection->prepareSelection(std::make_shared<SeSelector>(".*", ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4, "Default selector"));
                                        break;
                                }
                            }
                            return seSelection->getSelectionOperation();
                        }

                        void SeProtocolDetectionEngine::processSeMatch(std::shared_ptr<SelectionResponse> selectionResponse) {
                            if (seSelection->processDefaultSelection(selectionResponse)) {
                                std::shared_ptr<MatchingSe> selectedSe = seSelection->getSelectedSe();
                                std::cout << "Selector: " << selectedSe->getExtraInfo() << ", selection status = " << selectedSe->isSelected() << std::endl;
                            }
                            else {
                                std::cout << "No selection matched!" << std::endl;
                            }
                        }

                        void SeProtocolDetectionEngine::processSeInsertion() {
                            std::cout << "Unexpected SE insertion event" << std::endl;
                        }

                        void SeProtocolDetectionEngine::processSeRemoval() {
                            std::cout << "SE removal event" << std::endl;
                        }

                        void SeProtocolDetectionEngine::processUnexpectedSeRemoval() {
                            std::cout << "Unexpected SE removal event" << std::endl;
                        }
                    }
                }
            }
        }
    }
}
