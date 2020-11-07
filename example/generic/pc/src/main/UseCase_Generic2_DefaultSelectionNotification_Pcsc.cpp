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
#include "ReaderUtilities.h"
#include "SeCommonProtocols.h"
#include "SeProxyService.h"
#include "SeSelection.h"
#include "StubReader.h"

/* Common */
#include "IllegalStateException.h"

using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;
using namespace keyple::plugin::pcsc;
using namespace keyple::example::common;
using namespace keyple::example::generic::common;

using AidSelector = SeSelector::AidSelector;

/**
 * <h1>Use Case ‘generic 2’ – Default Selection Notification (PC/SC)</h1>
 * <ul>
 * <li>
 * <h2>Scenario:</h2>
 * <ul>
 * <li>Define a default selection of ISO 14443-4 (here a Calypso PO) and set it
 * to an observable reader, on SE detection in case the selection is successful,
 * notify the terminal application with the SE information.</li>
 * <li><code>
 * Default Selection Notification
 * </code> means that the SE processing is automatically started when detected.
 * </li>
 * <li>PO messages:
 * <ul>
 * <li>A single SE message handled at SeReader level</li>
 * </ul>
 * </li>
 * </ul>
 * </li>
 * </ul>
 */
class UseCase_Generic2_DefaultSelectionNotification_Pcsc final
: public std::enable_shared_from_this<
      UseCase_Generic2_DefaultSelectionNotification_Pcsc>,
  public ObservableReader::ReaderObserver {
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
public:
    /**
     *
     */
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(
        typeid(UseCase_Generic2_DefaultSelectionNotification_Pcsc));

    UseCase_Generic2_DefaultSelectionNotification_Pcsc()
    {
        /* Get the instance of the SeProxyService (Singleton pattern) */
        SeProxyService& seProxyService = SeProxyService::getInstance();

        /* Assign PcscPlugin to the SeProxyService */
        auto pcscFactory = std::make_shared<PcscPluginFactory>();
        auto factory = std::dynamic_pointer_cast<PluginFactory>(pcscFactory);
        seProxyService.registerPlugin(factory);

        /*
         * Get a SE reader ready to work with contactless SE. Use the getReader helper method from
         * the ReaderUtilities class.
         */
        seReader = ReaderUtilities::getDefaultContactLessSeReader();

        /* Check if the reader exists */
        if (seReader == nullptr)
            throw IllegalStateException("Bad SE reader setup");

        logger->info("=============== " \
                     "UseCase Generic #2: AID based default selection " \
                     "===================\n");
        logger->info("= SE Reader  NAME = %\n", seReader->getName());

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
        auto aidSelector = AidSelector::builder()->aidToSelect(seAid).build();
        auto selector = SeSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                              .aidSelector(aidSelector)
                                              .build();
        auto seSelector = std::make_shared<GenericSeSelectionRequest>(selector);

        /*
         * Add the selection case to the current selection (we could have added
         * other cases here)
         */
        seSelection->prepareSelection(seSelector);

        /*
         * Provide the SeReader with the selection operation to be processed when a SE is inserted
         */
        (std::dynamic_pointer_cast<ObservableReader>(seReader))->setDefaultSelectionRequest(
                seSelection->getSelectionOperation(),
                ObservableReader::NotificationMode::MATCHED_ONLY,
                ObservableReader::PollingMode::REPEATING);

        logger->debug("end of constructor\n");
    }

    void doSomething()
    {
        /* Set the current class as Observer of the first reader */
        (std::dynamic_pointer_cast<ObservableReader>(seReader))
            ->addObserver(shared_from_this());

        logger->info("= #### Wait for a SE. The default AID based selection to be processed as " \
                     "soon as the SE is detected\n");

        /* Wait for ever (exit with CTRL-C) */
        while (1);
    }

    void update(const std::shared_ptr<ReaderEvent> event)
    {
        if (event->getEventType() == ReaderEvent::EventType::SE_MATCHED) {
            /* The selection has one target, get the result at index 0 */
            std::shared_ptr<AbstractMatchingSe> selectedSe = nullptr;
            try {
                selectedSe =
                    seSelection->processDefaultSelection(event->getDefaultSelectionsResponse())
                               ->getActiveMatchingSe();
            } catch (const KeypleException& e) {
                logger->error("Exception: %\n", e.getMessage());
                try {
                    std::shared_ptr<SeReader> reader = event->getReader();
                    auto observable = std::dynamic_pointer_cast<ObservableReader>(reader);
                    observable->finalizeSeProcessing();
                } catch (const KeypleReaderNotFoundException& ex) {
                    logger->error("Reader not found exception: {}", ex.getMessage());
                } catch (const KeyplePluginNotFoundException& ex) {
                    logger->error("Plugin not found exception: {}", ex.getMessage());
                }
            }

            if (selectedSe != nullptr) {
                logger->info("Observer notification: the selection of the SE has succeeded\n");
                logger->info("= #### End of the SE processing\n");
            } else {
                logger->error("The selection of the SE has failed. Should not have occurred due " \
                              "to the MATCHED_ONLY selection mode.");
            }
        } else if (event->getEventType() == ReaderEvent::EventType::SE_INSERTED) {
            logger->error("SE_INSERTED event: should not have occurred due to the MATCHED_ONLY " \
                          "selection mode\n");
        } else if (event->getEventType() == ReaderEvent::EventType::SE_REMOVED) {
            logger->info("The SE has been removed\n");
        } else {
            logger->debug("Unhandled case\n");
        }

        if (event->getEventType() == ReaderEvent::EventType::SE_INSERTED ||
            event->getEventType() == ReaderEvent::EventType::SE_MATCHED) {
            /*
             * Informs the underlying layer of the end of the SE processing, in order to manage the
             * removal sequence.
             */
            try {
                std::shared_ptr<SeReader> reader = event->getReader();
                auto observable = std::dynamic_pointer_cast<ObservableReader>(reader);
                observable->finalizeSeProcessing();
            } catch (const KeypleReaderNotFoundException& e) {
                logger->error("Reader not found exception: %\n", e.getMessage());
            } catch (const KeyplePluginNotFoundException& e) {
                logger->error("Plugin not found exception: %\n", e.getMessage());
            }
        }
    }
};

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    /* Create the observable object to handle the SE processing */
    auto m = std::make_shared<UseCase_Generic2_DefaultSelectionNotification_Pcsc>();
    m->doSomething();
}
