/**************************************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "SeProtocolDetectionEngine.h"

/* Example */
#include "GenericSeSelectionRequest.h"

/* Core */
#include "AbstractDefaultSelectionsRequest.h"
#include "ApduRequest.h"
#include "ByteArrayUtil.h"
#include "ChannelControl.h"
#include "MatchingSelection.h"
#include "SeCommonProtocols.h"
#include "SeReader.h"
#include "SeSelector.h"

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

using SeCommonProtocol = SeCommonProtocols::SeCommonProtocol;

SeProtocolDetectionEngine::SeProtocolDetectionEngine() : AbstractReaderObserverEngine() {}

void SeProtocolDetectionEngine::setReader(std::shared_ptr<SeReader> poReader)
{
    mPoReader = poReader;
}

std::shared_ptr<AbstractDefaultSelectionsRequest> SeProtocolDetectionEngine::prepareSeSelection()
{
    seSelection = std::make_shared<SeSelection>();

    /* Process SDK defined protocols */
    for (const auto& protocol : SeCommonProtocols::values) {
        if (protocol == SeCommonProtocols::PROTOCOL_ISO14443_4) {
            /* Add a Hoplink selector */
            const std::string HoplinkAID = "A000000291A000000191";
            //char SFI_T2Usage = static_cast<char>(0x1A);
            const uint8_t SFI_T2Environment = 0x14;

            auto aidSelector = AidSelector.builder().aidToSelect(HoplinkAID).build();
            auto seSelector =
                PoSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                      .aidSelector(aidSelector)
                                      .invalidatedPo(InvalidatedPo::REJECT)
                                      .build());
            auto poSelector = std::dynamic_pointer_cast<PoSelector>(seSelector);
            auto poSelectionRequest = std::make_shared<PoSelectionRequest>(poSelector);

            poSelectionRequest.prepareReadRecordFile(SFI_T2Environment, 1);

            seSelection->prepareSelection(poSelectionRequest);

        } else if(protocol == SeCommonProtocols::PROTOCOL_ISO14443_3A ||
                  protocol == SeCommonProtocols::PROTOCOL_ISO14443_3B) {
            /*
             * Not handled in this demo code
             */
        } else if (protocol == SeCommonProtocols::PROTOCOL_MIFARE_DESFIRE ||
                   protocol == SeCommonProtocols::PROTOCOL_B_PRIME) {
            /*
             * Intentionally ignored for demo purpose
             */
        } else {
            /* Add a generic selector */
            auto atrFilter = std::make_shared<AtrFilter>(".*");
            auto seSelector =
                SeSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                      .atrFilter(atrFilter)
                                      .build())
            auto selection = std::make_shared<GenericSeSelectionRequest>(seSelector);
            seSelection->prepareSelection(selection);
        }
    }

    return seSelection->getSelectionOperation();
}

void SeProtocolDetectionEngine::processSeMatch(
    const std::shared_ptr<AbstractDefaultSelectionsResponse> defaultSelectionsResponse)
{
    /* Get the SE that matches one of the two selection targets */
    if (seSelection->processDefaultSelection(defaultSelectionsResponse)->hasActiveSelection()) {
        std::shared_ptr<AbstractMatchingSe> selectedSe =
            seSelection->processDefaultSelection(defaultSelectionsResponse).getActiveMatchingSe();
    } else {
        /* TODO check this. Shouldn't an exception have been raised before? */
        std::cout << "No selection matched!" << std::endl;
    }
}

void SeProtocolDetectionEngine::processSeInserted()
{
    std::cout << "Unexpected SE insertion event" << std::endl;
}

void SeProtocolDetectionEngine::processSeRemoval()
{
    std::cout << "SE removal event" << std::endl;
}

void SeProtocolDetectionEngine::processUnexpectedSeRemoval()
{
    std::cout << "Unexpected SE removal event" << std::endl;
}
}
}
}
}
