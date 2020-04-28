/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include "AbstractMatchingSe.h"
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "ChannelControl.h"
#include "GenericSeSelectionRequest.h"
#include "LoggerFactory.h"
#include "KeypleBaseException.h"
#include "KeypleReaderException.h"
#include "Matcher.h"
#include "MatchingSelection.h"
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
using namespace keyple::example::generic::common;
using namespace keyple::example::generic::pc;

class UseCase_Generic4_SequentialMultiSelection_Pcsc {
};

const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(
    typeid(UseCase_Generic4_SequentialMultiSelection_Pcsc));

void doAndAnalyseSelection(std::shared_ptr<SeReader> seReader,
                           std::shared_ptr<SeSelection> seSelection, int index)
{
    std::shared_ptr<SelectionsResult> selectionsResult =
        seSelection->processExplicitSelection(seReader);

    if (selectionsResult->hasActiveSelection()) {
        std::shared_ptr<AbstractMatchingSe> matchingSe =
            selectionsResult->getMatchingSelection(0)->getMatchingSe();

        logger->info("The SE matched the selection %: %\n", index, matchingSe);
    } else {
        logger->info(
            "The selection 2 process did not return any selected SE\n");
    }
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    /* Get the instance of the SeProxyService (Singleton pattern) */
    SeProxyService& seProxyService = SeProxyService::getInstance();

    /* Assign PcscPlugin to the SeProxyService */
    seProxyService.registerPlugin(new PcscPluginFactory());

    /*
     * Get a SE reader ready to work with contactless SE. Use the getReader
     * helper method from the ReaderUtilities class.
     */
    std::shared_ptr<SeReader> seReader =
        ReaderUtilities::getDefaultContactLessSeReader();

    /* Check if the reader exists */
    if (seReader == nullptr) {
        throw std::make_shared<IllegalStateException>("Bad SE reader setup");
    }

    logger->info("=============== UseCase Generic #4: AID based sequential "
                 "explicit multiple selection ==================\n");
    logger->info("= SE Reader  NAME = %\n", seReader->getName());

    std::shared_ptr<AbstractMatchingSe> matchingSe;

    /* Check if a SE is present in the reader */
    if (seReader->isSePresent()) {

        std::shared_ptr<SeSelection> seSelection;

        /*
         * Operate SE selection (change the AID here to adapt it to the SE used
         * for the test)
         */
        std::string seAidPrefix = "315449432E494341";
        std::vector<uint8_t> aid = ByteArrayUtil::fromHex(seAidPrefix);

        /* First selection case */
        seSelection =
            std::make_shared<SeSelection>(MultiSeRequestProcessing::FIRST_MATCH,
                                          ChannelControl::KEEP_OPEN);
        /*
         * AID based selection: get the first application occurrence matching
         * the AID, keep the physical channel open
         */
        seSelection->prepareSelection(
            std::make_shared<GenericSeSelectionRequest>(
                std::make_shared<SeSelector>(
                    SeCommonProtocols::PROTOCOL_ISO14443_4, nullptr,
                    std::make_shared<SeSelector::AidSelector>(
                        std::make_shared<SeSelector::AidSelector::IsoAid>(
                            aid),
                        nullptr,
                        SeSelector::AidSelector::FileOccurrence::FIRST,
                        SeSelector::AidSelector::FileControlInformation::FCI),
                    "Initial selection #1")));

        /* Do the selection and display the result */
        doAndAnalyseSelection(seReader, seSelection, 1);

        /*
         * New selection: get the next application occurrence matching the same
         * AID, close the physical channel after
         */
        seSelection =
            std::make_shared<SeSelection>(MultiSeRequestProcessing::FIRST_MATCH,
                                          ChannelControl::CLOSE_AFTER);

        seSelection->prepareSelection(
            std::make_shared<GenericSeSelectionRequest>(
                std::make_shared<SeSelector>(
                    SeCommonProtocols::PROTOCOL_ISO14443_4, nullptr,
                    std::make_shared<SeSelector::AidSelector>(
                        std::make_shared<SeSelector::AidSelector::IsoAid>(
                            aid),
                        nullptr,
                        SeSelector::AidSelector::FileOccurrence::NEXT,
                        SeSelector::AidSelector::FileControlInformation::FCI),
                    "Initial selection #2")));

        /* Do the selection and display the result */
        doAndAnalyseSelection(seReader, seSelection, 2);

    } else {
        logger->error("No SE were detected\n");
    }

    return 0;
}
