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
#include "PcscPlugin.h"
#include "PcscPluginFactory.h"
#include "SeCommonProtocols.h"
#include "SelectionStatus.h"
#include "SeProtocol.h"
#include "SeProxyService.h"
#include "SeReader.h"
#include "SeSelection.h"
#include "SeSelector.h"

/* Example */
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
    seProxyService.registerPlugin(std::make_shared<PcscPluginFactory>());

    /*
     * Get a SE reader ready to work with generic SE. Use the getReader helper method from the
     * ReaderUtilities class.
     */
    std::shared_ptr<SeReader> seReader = ReaderUtilities::getDefaultContactLessSeReader();

    logger->info("=============== UseCase Generic #1: AID based explicit selection ==============" \
                 "====\n");
    logger->info("= SE Reader  NAME = %\n", seReader->getName());

    /* Check if a SE is present in the reader */
    if (seReader->isSePresent()) {

        logger->info("= #### AID based selection\n");

        /* Prepare the SE selection */
        SeSelection seSelection;

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
        auto aidSelector = AidSelector::builder()->aidToSelect(seAid).build();
        auto seSelector = SeSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                                .aidSelector(aidSelector)
                                                .build();
        auto genericSeSelectionRequest = std::make_shared<GenericSeSelectionRequest>(seSelector);

        /*
         * Add the selection case to the current selection (we could have added
         * other cases here)
         */
        seSelection.prepareSelection(genericSeSelectionRequest);

        /*
         * Actual SE communication: operate through a single request the SE
         * selection
         */
        std::shared_ptr<AbstractMatchingSe> matchingSe =
            seSelection.processExplicitSelection(seReader)->getActiveMatchingSe();
        logger->info("The selection of the SE has succeeded\n");
        if (matchingSe->hasFci()) {
            const std::string fci = ByteArrayUtil::toHex(matchingSe->getFciBytes());
            logger->info("Application FCI = %\n", fci);
        }
        if (matchingSe->hasAtr()) {
            const std::string atr = ByteArrayUtil::toHex(matchingSe->getAtrBytes());
            logger->info("Secure Element ATR = %\n", atr);
        }

        logger->info("= #### End of the generic SE processing\n");
    } else {
        logger->info("The selection of the SE has failed\n");
    }

    return 0;
}
