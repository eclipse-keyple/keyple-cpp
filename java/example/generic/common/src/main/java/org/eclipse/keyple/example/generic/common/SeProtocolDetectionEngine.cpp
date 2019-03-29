#include "SeProtocolDetectionEngine.h"

/* Core */
#include "ChannelState.h"
#include "ContactlessProtocols_Import.h"
#include "SeReader.h"
#include "SeSelector.h"
#include "ByteArrayUtils.h"
#include "ApduRequest.h"

/* Calypso */
#include "PoSelector.h"
#include "ReadDataStructure.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic {
                    namespace common {
                        using ReadDataStructure = org::eclipse::keyple::calypso::command::po::parser::ReadDataStructure;
                        using PoSelectionRequest = org::eclipse::keyple::calypso::transaction::PoSelectionRequest;
                        using namespace org::eclipse::keyple::seproxy;
                        using DefaultSelectionRequest = org::eclipse::keyple::seproxy::event::DefaultSelectionRequest;
                        using SelectionResponse = org::eclipse::keyple::seproxy::event_Renamed::SelectionResponse;
                        using ApduRequest = org::eclipse::keyple::seproxy::message::ApduRequest;
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using namespace org::eclipse::keyple::transaction;
                        using ByteArrayUtils = org::eclipse::keyple::util::ByteArrayUtils;

                        SeProtocolDetectionEngine::SeProtocolDetectionEngine() : AbstractReaderObserverEngine() {
                        }

                        void SeProtocolDetectionEngine::setReader(std::shared_ptr<SeReader> poReader) {
                            this->poReader = poReader;
                        }

                        std::shared_ptr<DefaultSelectionRequest> SeProtocolDetectionEngine::prepareSeSelection() {

                            seSelection = std::make_shared<SeSelection>(poReader);

                            // process SDK defined protocols
                            for (ContactlessProtocols protocol : ContactlessProtocols::values()) {
                                switch (protocol.innerEnumValue) {
                                    case ContactlessProtocols::InnerEnum::PROTOCOL_ISO14443_4: {
                                        /* Add a Hoplink selector */
                                        std::string HoplinkAID = "A000000291A000000191";
                                        char SFI_T2Usage = static_cast<char>(0x1A);
                                        char SFI_T2Environment = static_cast<char>(0x14);
					
                                        std::shared_ptr<PoSelectionRequest> poSelectionRequest = std::make_shared<PoSelectionRequest>(std::make_shared<SeSelector>(std::make_shared<SeSelector::AidSelector>(ByteArrayUtils::fromHex(HoplinkAID), nullptr), nullptr, "Hoplink selector"), ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4);

                                        poSelectionRequest->preparePoCustomReadCmd("Standard Get Data", std::make_shared<ApduRequest>(ByteArrayUtils::fromHex("FFCA000000"), false));

                                        poSelectionRequest->prepareReadRecordsCmd(SFI_T2Environment, ReadDataStructure::SINGLE_RECORD_DATA, static_cast<char>(0x01), "Hoplink T2 Environment");

                                        seSelection->prepareSelection(poSelectionRequest);

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
                                        seSelection->prepareSelection(std::make_shared<SeSelectionRequest>(std::make_shared<SeSelector>(nullptr, std::make_shared<SeSelector::AtrFilter>(".*"), "Default selector"), ChannelState::KEEP_OPEN, ContactlessProtocols::PROTOCOL_ISO14443_4));
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
