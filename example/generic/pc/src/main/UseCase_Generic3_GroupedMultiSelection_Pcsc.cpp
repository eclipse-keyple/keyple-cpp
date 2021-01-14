/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
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


#include "ByteArrayUtil.h"
#include "GenericSeSelectionRequest.h"
#include "PcscPlugin.h"
#include "PcscPluginFactory.h"
#include "ReaderUtilities.h"
#include "SeCommonProtocols.h"
#include "SeProtocol.h"
#include "SeProxyService.h"
#include "SeReader.h"
#include "SeSelection.h"

/* Examples */
#include "ReaderUtilities.h"

using namespace keyple::common;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;
using namespace keyple::plugin::pcsc;
using namespace keyple::example::common;
using namespace keyple::example::generic::common;

using AidSelector = SeSelector::AidSelector;
using FileOccurrence = SeSelector::AidSelector::FileOccurrence;
using FileControlInformation = SeSelector::AidSelector::FileControlInformation;

class UseCase_Generic3_GroupedMultiSelection_Pcsc {
};

const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(
    typeid(UseCase_Generic3_GroupedMultiSelection_Pcsc));

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    /* Get the instance of the SeProxyService (Singleton pattern) */
    SeProxyService& seProxyService = SeProxyService::getInstance();

    /* Assign PcscPlugin to the SeProxyService */
    auto pcscFactory = std::make_shared<PcscPluginFactory>();
    auto factory = std::dynamic_pointer_cast<PluginFactory>(pcscFactory);
    seProxyService.registerPlugin(factory);

    /*
     * Get a SE reader ready to work with contactless SE. Use the getReader helper method from the
     * ReaderUtilities class.
     */
    std::shared_ptr<SeReader> seReader = ReaderUtilities::getDefaultContactLessSeReader();

    logger->info("=============== UseCase Generic #3: AID based grouped "
                 "explicit multiple selection ==================\n");
    logger->info("= SE Reader  NAME = %\n", seReader->getName());

    /* Check if a SE is present in the reader */
    if (seReader->isSePresent()) {

        /* CLOSE_AFTER to force selection of all applications */
        auto seSelection = std::make_shared<SeSelection>(MultiSeRequestProcessing::PROCESS_ALL);

        /*
         * Operate SE selection (change the AID here to adapt it to the SE used
         * for the test)
         */
        //std::string seAidPrefix = "A000000404012509";
        std::string seAidPrefix = "315449432E494341";
        std::vector<uint8_t> aid = ByteArrayUtil::fromHex(seAidPrefix);

        /* AID based selection (1st selection, later indexed 0 */
        auto aidSelector = AidSelector::builder()
                               ->aidToSelect(seAidPrefix)
                                .fileOccurrence(FileOccurrence::FIRST)
                                .fileControlInformation(FileControlInformation::FCI)
                                .build();
        auto seSelector = SeSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                                .aidSelector(aidSelector)
                                                .build();
        auto generic = std::make_shared<GenericSeSelectionRequest>(seSelector);
        seSelection->prepareSelection(generic);

        /* next selection (2nd selection, later indexed 1) */
        aidSelector = AidSelector::builder()->aidToSelect(seAidPrefix)
                                             .fileOccurrence(FileOccurrence::NEXT)
                                             .fileControlInformation(FileControlInformation::FCI)
                                             .build();
        seSelector = SeSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                           .aidSelector(aidSelector)
                                           .build();
        generic = std::make_shared<GenericSeSelectionRequest>(seSelector);
        seSelection->prepareSelection(generic);

        /* next selection (3rd selection, later indexed 2) */
        aidSelector = AidSelector::builder()->aidToSelect(seAidPrefix)
                                             .fileOccurrence(FileOccurrence::NEXT)
                                             .fileControlInformation(FileControlInformation::FCI)
                                             .build();
        seSelector = SeSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                           .aidSelector(aidSelector)
                                           .build();
        generic = std::make_shared<GenericSeSelectionRequest>(seSelector);
        seSelection->prepareSelection(generic);

        /* Close the channel after the selection to force the selection of all applications */
        seSelection->prepareReleaseSeChannel();

        /* Actual SE communication: operate through a single request the SE selection */
        std::shared_ptr<SelectionsResult> selectionsResult =
            seSelection->processExplicitSelection(seReader);

        if (selectionsResult->getMatchingSelections().size() > 0) {
            for (const auto& entry : selectionsResult->getMatchingSelections()) {
                std::shared_ptr<AbstractMatchingSe> matchingSe = entry.second;
                const std::string atr = matchingSe->hasAtr() ?
                                        ByteArrayUtil::toHex(matchingSe->getAtrBytes()) : "no ATR";
                const std::string fci = matchingSe->hasFci() ?
                                        ByteArrayUtil::toHex(matchingSe->getFciBytes()) : "no FCI";
                logger->info("Selection status for selection (indexed %): \n\t\tATR: %\n\t\t" \
                             "FCI: %\n",
                             entry.first, atr, fci);
            }
        } else {
            logger->info("No SE matched the selection\n");
        }
    } else {
        logger->error("No SE were detected\n");
    }

    return 0;
}
