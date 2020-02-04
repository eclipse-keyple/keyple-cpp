/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

/* Example */
#include "GenericSeSelectionRequest.h"
#include "SeProtocolDetectionEngine.h"

/* Core */
#include "AbstractDefaultSelectionsRequest.h"
#include "ApduRequest.h"
#include "ByteArrayUtil.h"
#include "ChannelState.h"
#include "MatchingSelection.h"
#include "SeCommonProtocols_Import.h"
#include "SeReader.h"
#include "SeSelector_Import.h"

/* Calypso */
#include "PoSelector.h"
#include "ReadDataStructure.h"
#include "PoSelectionRequest.h"

namespace keyple {
namespace example {
namespace generic {
namespace common {

using namespace keyple::calypso::command::po::parser;
using namespace keyple::calypso::transaction;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

SeProtocolDetectionEngine::SeProtocolDetectionEngine() : AbstractReaderObserverEngine() {
}

void SeProtocolDetectionEngine::setReader(std::shared_ptr<SeReader> poReader) {
    this->poReader = poReader;
}

std::shared_ptr<AbstractDefaultSelectionsRequest> SeProtocolDetectionEngine::prepareSeSelection() {

    seSelection = std::make_shared<SeSelection>();

    // process SDK defined protocols
    for (SeCommonProtocols protocol : SeCommonProtocols::values()) {
        switch (protocol.innerEnumValue) {
        case SeCommonProtocols::InnerEnum::PROTOCOL_ISO14443_4: {
            /* Add a Hoplink selector */
            std::string HoplinkAID = "A000000291A000000191";
            //char SFI_T2Usage = static_cast<char>(0x1A);
            char SFI_T2Environment = static_cast<char>(0x14);

            std::vector<uint8_t> aid = ByteArrayUtil::fromHex(HoplinkAID);
            std::shared_ptr<PoSelectionRequest> poSelectionRequest =
                std::make_shared<PoSelectionRequest>(
                    std::make_shared<PoSelector>(SeCommonProtocols::PROTOCOL_ISO14443_3A, nullptr,
                                                 std::make_shared<PoSelector::PoAidSelector>(
                                                     std::make_shared<SeSelector::AidSelector::IsoAid>(aid),
                                                     PoSelector::InvalidatedPo::REJECT),
                                                 "Hoplink selector"),
                    ChannelState::KEEP_OPEN);

                std::vector<uint8_t> apdu = ByteArrayUtil::fromHex("FFCA000000");
                poSelectionRequest->preparePoCustomReadCmd("Standard Get Data", apdu);
                poSelectionRequest->prepareReadRecordsCmd(SFI_T2Environment, ReadDataStructure::SINGLE_RECORD_DATA, static_cast<char>(0x01), "Hoplink T2 Environment");
                seSelection->prepareSelection(poSelectionRequest);
                break;
            }
            case SeCommonProtocols::InnerEnum::PROTOCOL_ISO14443_3A:
            case SeCommonProtocols::InnerEnum::PROTOCOL_ISO14443_3B:
                // not handled in this demo code
                break;
            case SeCommonProtocols::InnerEnum::PROTOCOL_MIFARE_DESFIRE:
            case SeCommonProtocols::InnerEnum::PROTOCOL_B_PRIME:
                // intentionally ignored for demo purpose
                break;
            default:
                /* Add a generic selector */
                seSelection->prepareSelection(
                    std::make_shared<GenericSeSelectionRequest>(
                        std::make_shared<SeSelector>(SeCommonProtocols::PROTOCOL_ISO14443_3A,
                                                     std::make_shared<SeSelector::AtrFilter>(".*"),
                                                     nullptr,
                                                     "Default selector"),
                        ChannelState::KEEP_OPEN));
                break;
        }
    }

    return seSelection->getSelectionOperation();
}

void SeProtocolDetectionEngine::processSeMatch(std::shared_ptr<AbstractDefaultSelectionsResponse> defaultSelectionsResponse)
{
    std::shared_ptr<SelectionsResult> selectionsResult = seSelection->processDefaultSelection(defaultSelectionsResponse);
    /* get the SE that matches one of the two selection targets */
    std::shared_ptr<AbstractMatchingSe> selectedSe = selectionsResult->getActiveSelection()->getMatchingSe();
    if (selectedSe != nullptr) {
        std::cout << "Selector: " << selectedSe->getSelectionExtraInfo() << ", selection status = " << selectedSe->isSelected() << std::endl;
    } else {
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
