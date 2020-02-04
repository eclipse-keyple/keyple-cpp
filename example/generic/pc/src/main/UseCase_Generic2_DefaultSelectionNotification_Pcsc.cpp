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
#include "ChannelState.h"
#include "GenericSeSelectionRequest.h"
#include "KeypleBaseException.h"
#include "MatchingSelection.h"
#include "ObservableReader_Import.h"
#include "PcscPlugin.h"
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
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;
using namespace keyple::plugin::pcsc;
using namespace keyple::example::generic::common;
using namespace keyple::example::generic::pc;

class UseCase_Generic2_DefaultSelectionNotification_Pcsc
: public std::enable_shared_from_this<
      UseCase_Generic2_DefaultSelectionNotification_Pcsc>,
  public ObservableReader::ReaderObserver  {
private:
    /**
     *
     */
    const std::shared_ptr<void> waitForEnd = nullptr;

    /**
     *
     */
    //std::string seAid = "A0000004040125090101";
    std::string seAid = "304554502E494341";

    /**
     *
     */
    std::shared_ptr<SeSelection> seSelection;

    /**
     *
     */
    std::shared_ptr<SeReader> seReader;

    /**
     *
     */
    PcscPlugin pcscplugin;

public:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(
            typeid(UseCase_Generic2_DefaultSelectionNotification_Pcsc));

    UseCase_Generic2_DefaultSelectionNotification_Pcsc() : pcscplugin(PcscPlugin::getInstance())
    {
        /* Get the instance of the PC/SC plugin */
        //pcscplugin = PcscPlugin::getInstance();
        pcscplugin.initReaders();

        /* Assign PcscPlugin to the SeProxyService */
        SeProxyService& seProxyService = SeProxyService::getInstance();
        seProxyService.addPlugin(std::make_shared<PcscPlugin>(pcscplugin));

        /*
         * Get a SE reader ready to work with contactless SE. Use the getReader
         * helper method from the ReaderUtilities class.
         */
        seReader = ReaderUtilities::getDefaultContactLessSeReader();

        /* Check if the reader exists */
        if (seReader == nullptr) {
            throw IllegalStateException("Bad SE reader setup");
        }

        logger->info("=============== UseCase Generic #2: AID based default " \
                     "selection ===================\n");
        logger->info("= SE Reader  NAME = %s\n", seReader->getName().c_str());

        /*
         * Prepare a SE selection
         */
        seSelection = std::make_shared<SeSelection>();

        /*
         * Setting of an AID based selection
         *
         * Select the first application matching the selection AID whatever the
         * SE communication protocol keep the logical channel open after the
         * selection
         */

        /*
         * Generic selection: configures a SeSelector with all the desired
         * attributes to make the selection
         */
        std::vector<uint8_t> aid = ByteArrayUtil::fromHex(seAid);
        std::shared_ptr<SeSelector::AidSelector::IsoAid> isoAid =
            std::make_shared<SeSelector::AidSelector::IsoAid>(aid);
        std::shared_ptr<SeSelector::AidSelector> aidSelector  =
            std::make_shared<SeSelector::AidSelector>(isoAid, nullptr);
        std::shared_ptr<SeSelector> seSelector =
            std::make_shared<SeSelector>(
                SeCommonProtocols::PROTOCOL_ISO14443_4, nullptr, aidSelector,
                "AID:" + seAid);
        std::shared_ptr<GenericSeSelectionRequest> genericSeSelectionRequest =
            std::make_shared<GenericSeSelectionRequest>(
                seSelector, ChannelState::KEEP_OPEN);
        /*
         * Add the selection case to the current selection (we could have added
         * other cases here)
         */
        seSelection->prepareSelection(genericSeSelectionRequest);

        /*
         * Provide the SeReader with the selection operation to be processed
         * when a SE is inserted.
         */
        std::shared_ptr<ObservableReader> observable =
            std::dynamic_pointer_cast<ObservableReader>(seReader);
        observable->setDefaultSelectionRequest(
            seSelection->getSelectionOperation(),
            ObservableReader::NotificationMode::MATCHED_ONLY);

        logger->debug("end of constructor\n");
    }

    virtual ~UseCase_Generic2_DefaultSelectionNotification_Pcsc()
    {
    }

    void doSomething()
    {
        /* Set the current class as Observer of the first reader */
        (std::dynamic_pointer_cast<ObservableReader>(seReader))
            ->addObserver(shared_from_this());

        logger->info("=======================================================" \
                     "===========================\n");
        logger->info("= Wait for a SE. The default AID based selection to be " \
                     "processed as soon as the  =\n");
        logger->info("= SE is detected.                                      " \
                     "                          =\n");
        logger->info("=======================================================" \
                     "===========================\n");

        /* Wait for ever (exit with CTRL-C) */
        while(1);
    }

    void update(std::shared_ptr<ReaderEvent> event)
    {
        if (event->getEventType() == ReaderEvent::EventType::SE_MATCHED) {
            /* the selection has one target, get the result at index 0 */
            if (seSelection->processDefaultSelection(
                    event->getDefaultSelectionsResponse())->getActiveSelection()
                        ->getMatchingSe()) {

                //std::shared_ptr<MatchingSe> selectedSe = seSelection->getSelectedSe(); Alex: unused?

                logger->info("Observer notification: the selection of the SE " \
                             "has succeeded\n");

                logger->info("===============================================" \
                             "===================================\n");
                logger->info("= End of the SE processing.                    " \
                             "                                  =\n");
                logger->info("===============================================" \
                             "===================================\n");
            } else {
                logger->error("The selection of the SE has failed. Should not" \
                              " have occurred due to the MATCHED_ONLY " \
                              "selection mode\n");
            }
        } else if (event->getEventType() ==
                       ReaderEvent::EventType::SE_INSERTED) {
            logger->error("SE_INSERTED event: should not have occurred due to" \
                          " the MATCHED_ONLY selection mode\n");
        } else if (event->getEventType() == ReaderEvent::EventType::SE_REMOVAL)
        {
            logger->info("The SE has been removed.");
        }
    }

};

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    /* Create the observable object to handle the SE processing */
    std::shared_ptr<UseCase_Generic2_DefaultSelectionNotification_Pcsc> m =
        std::make_shared<UseCase_Generic2_DefaultSelectionNotification_Pcsc>();
    m->doSomething();
}
