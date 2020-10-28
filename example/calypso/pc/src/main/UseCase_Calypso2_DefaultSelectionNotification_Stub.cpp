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

/* Core */
#include "ByteArrayUtil.h"

/* Example */
#include "CalypsoClassicInfo.h"
#include "CalypsoClassicTransactionEngine.h"

/* Common */
#include "IllegalStateException.h"
#include "LoggerFactory.h"

/* Core */
#include "PoSelectionRequest.h"
#include "ReaderPlugin.h"
#include "SeCommonProtocols.h"
#include "SeProxyService.h"

/* Calypso */
#include "CalypsoPo.h"
#include "CalypsoPoCommandException.h"
#include "CalypsoPoTransactionException.h"
#include "ElementaryFile.h"
#include "ReadRecordsRespPars.h"

/* Plugin */
#include "StubCalypsoClassic.h"
#include "StubSamCalypsoClassic.h"
#include "StubProtocolSetting.h"
#include "StubPlugin.h"
#include "StubPluginFactory.h"
#include "StubReader.h"

using namespace keyple::calypso::command::po::exception;
using namespace keyple::calypso::transaction;
using namespace keyple::calypso::transaction::exception;
using namespace keyple::common::exception;
using namespace keyple::example::calypso::common::transaction;
using namespace keyple::example::calypso::common::postructure;
using namespace keyple::example::calypso::pc::stub::se;
using namespace keyple::plugin::stub;
using namespace keyple::core::util;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;

using AidSelector = SeSelector::AidSelector;
using InvalidatedPo = PoSelector::InvalidatedPo;

class UseCase_Calypso2_DefaultSelectionNotification_Stub final
: public std::enable_shared_from_this<UseCase_Calypso2_DefaultSelectionNotification_Stub>,
  public ObservableReader::ReaderObserver {
private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(
        typeid(UseCase_Calypso2_DefaultSelectionNotification_Stub));

    /**
     *
     */
    SeSelection seSelection;

public:
    /**
     *
     */
    UseCase_Calypso2_DefaultSelectionNotification_Stub() {}

    void init()
    {
        /* Get the instance of the SeProxyService (Singleton pattern) */
        SeProxyService& seProxyService = SeProxyService::getInstance();

        const std::string STUB_PLUGIN_NAME = "stub1";

        /* Register Stub plugin in the platform */
        auto factory = std::make_shared<StubPluginFactory>(STUB_PLUGIN_NAME);
        std::shared_ptr<ReaderPlugin> stubPlugin = seProxyService.registerPlugin(factory);


        /* Plug the PO stub reader */
        std::dynamic_pointer_cast<StubPlugin>(stubPlugin)->plugStubReader("poReader", true);

        /* Get a PO reader ready to work with Calypso PO */
        std::shared_ptr<SeReader> poReader = stubPlugin->getReader("poReader");

        /* Check if the reader exists */
        if (poReader == nullptr) {
            throw IllegalStateException(
                "Bad PO reader setup");
        }

        poReader->addSeProtocolSetting(
            SeCommonProtocols::PROTOCOL_ISO14443_4,
            StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

        logger->info("=============== UseCase Calypso #2: AID based explicit selection ==========" \
                     "=========\n");
        logger->info("= PO Reader  NAME = %\n", poReader->getName());

        /*
         * Prepare a Calypso PO selection
         */
        // Class member

        /*
         * Setting of an AID based selection of a Calypso REV3 PO
         *
         * Select the first application matching the selection AID whatever the SE communication
         * protocol keep the logical channel open after the selection
         */

        /*
         * Calypso selection: configures a PoSelector with all the desired attributes to make the
         * selection and read additional information afterwards
         */
        auto aidSelector = AidSelector::builder()->aidToSelect(CalypsoClassicInfo::AID).build();
        auto seSelector = PoSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                                .aidSelector(aidSelector)
                                                .invalidatedPo(InvalidatedPo::REJECT)
                                                .build();
        auto poSelector = std::dynamic_pointer_cast<PoSelector>(seSelector);
        auto poSelectionRequest = std::make_shared<PoSelectionRequest>(poSelector);

        /* Prepare the reading order */
        poSelectionRequest->prepareReadRecordFile(CalypsoClassicInfo::SFI_EnvironmentAndHolder,
                                                  CalypsoClassicInfo::RECORD_NUMBER_1);

        /* Add the selection case to the current selection (we could have added other cases here) */
        auto abstract = std::reinterpret_pointer_cast
                            <AbstractSeSelectionRequest<AbstractApduCommandBuilder>>(
                                poSelectionRequest);
        seSelection.prepareSelection(abstract);

        /*
         * Provide the SeReader with the selection operation to be processed
         * when a PO is inserted
         */

        std::dynamic_pointer_cast<ObservableReader>(poReader)
            ->setDefaultSelectionRequest(
                seSelection.getSelectionOperation(),
                ObservableReader::NotificationMode::MATCHED_ONLY);

        /* Set the current class as Observer of the first reader */
        std::dynamic_pointer_cast<ObservableReader>(poReader)->addObserver(
            shared_from_this());

        logger->info("= #### Wait for a PO. The default AID based selection with reading of " \
                     "Environment");
        logger->info("= #### file is ready to be processed as soon as the PO is detected.");

        /* Create a 'virtual' Calypso PO */
        std::shared_ptr<StubSecureElement> calypsoStubSe = std::make_shared<StubCalypsoClassic>();

        /* Wait a while */
        Thread::sleep(100);

        logger->info("Insert stub PO.\n");
        std::dynamic_pointer_cast<StubReader>(poReader)->insertSe(calypsoStubSe);

        /* Wait a while */
        Thread::sleep(1000);

        logger->info("Remove stub PO.\n");
        std::dynamic_pointer_cast<StubReader>(poReader)->removeSe();

        exit(0);
    }

    /**
     *
     */
    void update(const std::shared_ptr<ReaderEvent> event)
    {
        ReaderEvent::EventType type = event->getEventType();

        if (type == ReaderEvent::EventType::SE_MATCHED) {
            std::shared_ptr<CalypsoPo> calypsoPo = nullptr;
            std::shared_ptr<SeReader> poReader = nullptr;
            try {
                std::shared_ptr<AbstractMatchingSe> selection =
                    seSelection.processDefaultSelection(event->getDefaultSelectionsResponse())
                              ->getActiveMatchingSe();
                calypsoPo = std::dynamic_pointer_cast<CalypsoPo>(selection);
                poReader = SeProxyService::getInstance().getPlugin(event->getPluginName())
                                                       ->getReader(event->getReaderName());
            } catch (const KeypleReaderNotFoundException& e) {
                logger->error("Reader not found! %\n", e.getMessage());
            } catch (const KeyplePluginNotFoundException& e) {
                logger->error("Plugin not found! %\n", e.getMessage());
            } catch (KeypleException e) {
                logger->error("The selection process failed! %\n", e.getMessage());
            }

            logger->info("Observer notification: the selection of the PO has succeeded\n");

            /* Retrieve the data read from the CalyspoPo updated during the transaction process */
            std::shared_ptr<ElementaryFile> efEnvironmentAndHolder =
                calypsoPo->getFileBySfi(CalypsoClassicInfo::SFI_EnvironmentAndHolder);
            const std::string environmentAndHolder =
                ByteArrayUtil::toHex(efEnvironmentAndHolder->getData()->getContent());

            /* Log the result */
            logger->info("EnvironmentAndHolder file data: %\n", environmentAndHolder);

            /* Go on with the reading of the first record of the EventLog file */
            logger->info("= #### 2nd PO exchange: reading transaction of the EventLog file\n");

            PoTransaction poTransaction(std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPo));

            /*
             * Prepare the reading order and keep the associated parser for later use once the
             * transaction has been processed.
             */
            poTransaction.prepareReadRecordFile(CalypsoClassicInfo::SFI_EventLog,
                                            CalypsoClassicInfo::RECORD_NUMBER_1);

            /*
             * Actual PO communication: send the prepared read order, then close the channel with
             * the PO
             */
            try {
                poTransaction.processPoCommands(ChannelControl::CLOSE_AFTER);
                logger->info("The reading of the EventLog has succeeded\n");

                /* Retrieve the data read from the CalyspoPo updated during the transaction process */
                std::shared_ptr<ElementaryFile> efEventLog =
                calypsoPo->getFileBySfi(CalypsoClassicInfo::SFI_EventLog);
                const std::string eventLog =
                    ByteArrayUtil::toHex(efEventLog->getData()->getContent());

                /* Log the result */
                logger->info("EventLog file data: %\n", eventLog);

            } catch (const CalypsoPoTransactionException& e) {
                logger->error("CalypsoPoTransactionException: %\n", e.getMessage());
            } catch (const CalypsoPoCommandException& e) {
                logger->error("PO command {} failed with the status code 0x%." \
                              " %\n",
                              e.getCommand(),
                              e.getStatusCode() & 0xFFFF,
                              e.getMessage());
            }
            logger->info("= #### End of the Calypso PO processing\n");

        } else if (type == ReaderEvent::EventType::SE_INSERTED) {
            logger->error("SE_INSERTED event: should not have occurred due to the MATCHED_ONLY " \
                          "selection mode\n");
        } else if (type == ReaderEvent::EventType::SE_REMOVED) {
            logger->info("The PO has been removed\n");
        } else {
        }

        if (event->getEventType() == ReaderEvent::EventType::SE_INSERTED ||
            event->getEventType() == ReaderEvent::EventType::SE_MATCHED) {
            /*
             * Informs the underlying layer of the end of the SE processing, in
             * order to manage the removal sequence. <p>If closing has already
             * been requested, this method will do nothing.
             */
            try {
                std::dynamic_pointer_cast<ObservableReader>(
                    SeProxyService::getInstance()
                        .getPlugin(event->getPluginName())
                        ->getReader(event->getReaderName()))
                    ->notifySeProcessed();
            } catch (KeypleReaderNotFoundException& e) {
                logger->error("Reader not found! %\n", e.getMessage());
            } catch (const KeyplePluginNotFoundException& e) {
                logger->error("Plugin not found! %\n", e.getMessage());
            }
        }
    }
};

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    /* Create the observable object to handle the PO processing */
    auto m = std::make_shared<UseCase_Calypso2_DefaultSelectionNotification_Stub>();
    m->init();
}
