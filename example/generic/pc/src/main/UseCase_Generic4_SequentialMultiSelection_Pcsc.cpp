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

#include "AbstractMatchingSe.h"
#include "ByteArrayUtil.h"
#include "GenericSeSelectionRequest.h"
#include "LoggerFactory.h"
#include "PcscPlugin.h"
#include "PcscPluginFactory.h"
#include "ReaderUtilities.h"
#include "SeCommonProtocols.h"
#include "SelectionStatus.h"
#include "SeProtocol.h"
#include "SeProxyService.h"
#include "SeReader.h"
#include "SeResponse.h"
#include "SeSelector.h"
#include "SeSelection.h"

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

class UseCase_Generic4_SequentialMultiSelection_Pcsc {
};

const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(
    typeid(UseCase_Generic4_SequentialMultiSelection_Pcsc));

void doAndAnalyseSelection(std::shared_ptr<SeReader> seReader,
                           std::shared_ptr<SeSelection> seSelection,
                           int index)
{
    std::shared_ptr<SelectionsResult> selectionsResult =
        seSelection->processExplicitSelection(seReader);

    if (selectionsResult->hasActiveSelection()) {
        std::shared_ptr<AbstractMatchingSe> matchingSe = selectionsResult->getActiveMatchingSe();
        logger->info("The SE matched the selection %\n", index);
        const std::string atr = matchingSe->hasAtr() ?
                                ByteArrayUtil::toHex(matchingSe->getAtrBytes()) : "no ATR";
        const std::string fci = matchingSe->hasFci() ?
                                ByteArrayUtil::toHex(matchingSe->getFciBytes()) : "no FCI";
        logger->info("Selection status for case %: \n\t\tATR: %\n\t\tFCI: %\n", index, atr, fci);
    } else {
        logger->info("The selection 2 process did not return any selected SE\n");
    }
}

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


    logger->info("=============== " \
                 "UseCase Generic #4: AID based sequential explicit multiple selection " \
                 "==================\n");
    logger->info("= SE Reader  NAME = %\n", seReader->getName());

    std::shared_ptr<AbstractMatchingSe> matchingSe;

    /* Check if a SE is present in the reader */
    if (seReader->isSePresent()) {

        /* Operate SE selection (change the AID here to adapt it to the SE used for the test) */
        const std::string seAidPrefix = "315449432E494341";
        const std::vector<uint8_t> aid = ByteArrayUtil::fromHex(seAidPrefix);

        /* First selection case */
        auto seSelection = std::make_shared<SeSelection>();
        /*
         * AID based selection: get the first application occurrence matching the AID, keep the
         * physical channel open
         */
        auto aidSelector = AidSelector::builder()
                               ->aidToSelect(seAidPrefix)
                                .fileOccurrence(FileOccurrence::FIRST)
                                .fileControlInformation(FileControlInformation::FCI)
                                .build();
        auto selector = SeSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                              .aidSelector(aidSelector)
                                              .build();
        auto seSelector = std::make_shared<GenericSeSelectionRequest>(selector);
        seSelection->prepareSelection(seSelector);

        /* Do the selection and display the result */
        doAndAnalyseSelection(seReader, seSelection, 1);

        /*
         * New selection: get the next application occurrence matching the same
         * AID, close the physical channel after
         */
        seSelection = std::make_shared<SeSelection>();

        aidSelector = AidSelector::builder()
                               ->aidToSelect(seAidPrefix)
                                .fileOccurrence(FileOccurrence::NEXT)
                                .fileControlInformation(FileControlInformation::FCI)
                                .build();
        selector = SeSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                        .aidSelector(aidSelector)
                                        .build();
        seSelector = std::make_shared<GenericSeSelectionRequest>(selector);

        /* Close the channel after the selection */
        seSelection->prepareReleaseSeChannel();

        /* Do the selection and display the result */
        doAndAnalyseSelection(seReader, seSelection, 2);

    } else {
        logger->error("No SE were detected\n");
    }

    return 0;
}
