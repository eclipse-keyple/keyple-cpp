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
#include "ByteArrayUtil.h"
#include "GenericSeSelectionRequest.h"
#include "KeypleBaseException.h"
#include "MatchingSelection.h"
#include "PcscPlugin.h"
#include "PcscPluginFactory.h"
#include "ReaderUtilities.h"
#include "SeCommonProtocols_Import.h"
#include "SelectionStatus.h"
#include "SeProtocol_Import.h"
#include "SeProxyService.h"
#include "SeReader.h"
#include "SeSelection.h"
#include "SeSelector_Import.h"

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

//static std::string seAid = "A0000004040125090101";
static std::string seAid = "A000000291";
//static std::string seAid = "A00000019102";

/**
 * <h1>Use Case ‘generic 1’ – Explicit Selection Aid (PC/SC)</h1>
 * <ul>
 * <li>
 * <h2>Scenario:</h2>
 * <ul>
 * <li>Check if a ISO 14443-4 SE is in the reader, select a SE (here a Calypso
 * PO).</li>
 * <li><code>
 * Explicit Selection
 * </code> means that it is the terminal application which start the SE
 * processing.</li>
 * <li>SE messages:
 * <ul>
 * <li>A single SE message to select the application in the reader</li>
 * </ul>
 * </li>
 * </ul>
 * </li>
 * </ul>
 */
class ExplicitSelectionAid_Pcsc {
};

const std::shared_ptr<Logger> logger =
    LoggerFactory::getLogger(typeid(ExplicitSelectionAid_Pcsc));

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    /* Get the instance of the SeProxyService (Singleton pattern) */
    SeProxyService& seProxyService = SeProxyService::getInstance();

    /* Assign PcscPlugin to the SeProxyService */
    seProxyService.registerPlugin(new PcscPluginFactory());

    /*
     * Get a SE reader ready to work with generic SE. Use the getReader helper
     * method from the ReaderUtilities class.
     */
    std::shared_ptr<SeReader> seReader =
        ReaderUtilities::getDefaultContactLessSeReader();

    /* Check if the reader exists */
    if (seReader == nullptr) {
        throw IllegalStateException("Bad SE reader setup");
    }

    logger->info("=============== UseCase Generic #1: AID based explicit "
                 "selection ==================\n");
    logger->info("= SE Reader  NAME = %\n", seReader->getName());

    /* Check if a SE is present in the reader */
    if (seReader->isSePresent()) {

        logger->info("======================================================="
                     "===========================\n");
        logger->info("= AID based selection.                                 "
                     "                          =\n");
        logger->info("======================================================="
                     "===========================\n");

        /* Prepare the SE selection */
        std::shared_ptr<SeSelection> seSelection =
            std::make_shared<SeSelection>();

        /*
         * Setting of an AID based selection (in this example a Calypso REV3 PO)
         *
         * Select the first application matching the selection AID whatever the
         * SE communication protocol keep the logical channel open after the
         * selection
         */

        /*
         * Generic selection: configures a SeSelector with all the desired
         * attributes to make the selection and read additional information
         * afterwards
         */
        std::vector<uint8_t> aid = ByteArrayUtil::fromHex(seAid);
        std::shared_ptr<SeSelector::AidSelector::IsoAid> isoAid =
            std::make_shared<SeSelector::AidSelector::IsoAid>(aid);
        std::shared_ptr<SeSelector::AidSelector> aidSelector =
            std::make_shared<SeSelector::AidSelector>(isoAid, nullptr);
        std::shared_ptr<SeSelector> seSelector =
            std::make_shared<SeSelector>(SeCommonProtocols::PROTOCOL_ISO14443_4,
                                         nullptr, aidSelector, "AID:" + seAid);
        std::shared_ptr<GenericSeSelectionRequest> genericSeSelectionRequest =
            std::make_shared<GenericSeSelectionRequest>(seSelector);

        /*
         * Add the selection case to the current selection (we could have added
         * other cases here)
         */
        seSelection->prepareSelection(genericSeSelectionRequest);

        /*
         * Actual SE communication: operate through a single request the SE
         * selection
         */
        std::shared_ptr<SelectionsResult> selectionsResult =
            seSelection->processExplicitSelection(seReader);

        if (selectionsResult->hasActiveSelection()) {
            std::shared_ptr<AbstractMatchingSe> matchedSe =
                selectionsResult->getActiveSelection()->getMatchingSe();
            logger->info("The selection of the SE has succeeded\n");
            logger->info("Application FCI = %\n",
                         matchedSe->getSelectionStatus()->getFci());

            logger->info("==================================================="
                         "===============================\n");
            logger->info("= End of the generic SE processing.                "
                         "                              =\n");
            logger->info("==================================================="
                         "===============================\n");
        } else {
            logger->info("The selection of the SE has failed\n");
        }
    } else {
        logger->info("No SE were detected\n");
    }

    return 0;
}
