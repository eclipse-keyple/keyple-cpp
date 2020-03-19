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

#include "ByteArrayUtil.h"
#include "CalypsoClassicInfo.h"
#include "CalypsoClassicTransactionEngine.h"
#include "KeypleReaderNotFoundException.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "MatchingSelection.h"
#include "ObservableReader_Import.h"
#include "PoSelectionRequest.h"
#include "PoSelector.h"
#include "ReaderEvent_Import.h"
#include "ReaderPlugin.h"
#include "ReadRecordsRespPars.h"
#include "SeCommonProtocols_Import.h"
#include "SeProxyService.h"
#include "StubCalypsoClassic.h"
#include "StubSamCalypsoClassic.h"
#include "StubProtocolSetting_Import.h"
#include "StubPlugin.h"
#include "StubPluginFactory.h"
#include "StubReader.h"
#include "StubReaderImpl.h"

/* Common */
#include "Thread.h"

using namespace keyple::example::calypso::common::transaction;
using namespace keyple::example::calypso::common::postructure;
using namespace keyple::example::calypso::pc::stub::se;
using namespace keyple::plugin::stub;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;

class UseCase_Calypso2_DefaultSelectionNotification_Stub
: public std::enable_shared_from_this<
      UseCase_Calypso2_DefaultSelectionNotification_Stub>,
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
    int readEnvironmentParserIndex;

    /**
     *
     */
    std::shared_ptr<SeSelection> seSelection;

public:
    /**
     *
     */
    UseCase_Calypso2_DefaultSelectionNotification_Stub()
    {
    }

    void init()
    {
        /* Get the instance of the SeProxyService (Singleton pattern) */
        SeProxyService& seProxyService = SeProxyService::getInstance();

        const std::string STUB_PLUGIN_NAME = "stub1";

        /* Register Stub plugin in the platform */
        seProxyService.registerPlugin(new StubPluginFactory(STUB_PLUGIN_NAME));
        ReaderPlugin* stubPlugin = seProxyService.getPlugin(STUB_PLUGIN_NAME);

        /* Plug the PO stub reader */
        dynamic_cast<StubPlugin*>(stubPlugin)->plugStubReader(
            "poReader", true);

        /* Get a PO reader ready to work with Calypso PO */
        std::shared_ptr<StubReader> poReader =
            std::dynamic_pointer_cast<StubReader>(
                stubPlugin->getReader("poReader"));

        /* Check if the reader exists */
        if (poReader == nullptr) {
            throw std::make_shared<IllegalStateException>(
                "Bad PO reader setup");
        }

        poReader->addSeProtocolSetting(
            SeCommonProtocols::PROTOCOL_ISO14443_4,
            StubProtocolSetting::STUB_PROTOCOL_SETTING
                [SeCommonProtocols::PROTOCOL_ISO14443_4]);

        logger->info("=============== UseCase Calypso #2: AID based explicit "
                     "selection ===================\n");
        logger->info("= PO Reader  NAME = %s\n", poReader->getName().c_str());

        /*
         * Prepare a Calypso PO selection
         */
        seSelection = std::make_shared<SeSelection>();

        /*
         * Setting of an AID based selection of a Calypso REV3 PO
         *
         * Select the first application matching the selection AID whatever the
         * SE communication protocol keep the logical channel open after the
         * selection
         */

        /*
         * Calypso selection: configures a PoSelector with all the desired
         * attributes to make the selection and read additional information
         * afterwards
         */
        std::shared_ptr<PoSelectionRequest> poSelectionRequest =
            std::make_shared<PoSelectionRequest>(
                std::make_shared<PoSelector>(
                    SeCommonProtocols::PROTOCOL_ISO14443_4, nullptr,
                    std::make_shared<PoSelector::PoAidSelector>(
                        std::make_shared<SeSelector::AidSelector::IsoAid>(
                            CalypsoClassicInfo::AID),
                        PoSelector::InvalidatedPo::REJECT),
                    StringHelper::formatSimple("AID: %s",
                                               CalypsoClassicInfo::AID)));

        /*
         * Prepare the reading order and keep the associated parser for later
         * use once the selection has been made.
         */
        readEnvironmentParserIndex = poSelectionRequest->prepareReadRecordsCmd(
            CalypsoClassicInfo::SFI_EnvironmentAndHolder,
            ReadDataStructure::SINGLE_RECORD_DATA,
            CalypsoClassicInfo::RECORD_NUMBER_1,
            StringHelper::formatSimple(
                "EnvironmentAndHolder (SFI=%02X)",
                CalypsoClassicInfo::SFI_EnvironmentAndHolder));

        /*
         * Add the selection case to the current selection (we could have added
         * other cases here)
         */
        seSelection->prepareSelection(poSelectionRequest);

        /*
         * Provide the SeReader with the selection operation to be processed
         * when a PO is inserted
         */
        std::dynamic_pointer_cast<ObservableReader>(poReader)
            ->setDefaultSelectionRequest(
                seSelection->getSelectionOperation(),
                ObservableReader::NotificationMode::MATCHED_ONLY);

        /* Set the current class as Observer of the first reader */
        std::dynamic_pointer_cast<ObservableReader>(poReader)->addObserver(
            shared_from_this());

        logger->info("========================================================="
                     "=========================\n");
        logger->info("= Wait for a PO. The default AID based selection with "
                     "reading of Environment     =\n");
        logger->info("= file is reader to be processed as soon as the PO is "
                     "detected.                  =\n");
        logger->info("========================================================="
                     "=========================\n");

        /* Create a 'virtual' Calypso PO */
        std::shared_ptr<StubSecureElement> calypsoStubSe =
            std::make_shared<StubCalypsoClassic>();

        /* Wait a while */
        Thread::sleep(100);

        logger->info("Insert stub PO.\n");
        poReader->insertSe(calypsoStubSe);

        /* Wait a while */
        Thread::sleep(1000);

        logger->info("Remove stub PO.\n");
        poReader->removeSe();

        exit(0);
    }

    /**
     *
     */
    virtual ~UseCase_Calypso2_DefaultSelectionNotification_Stub()
    {
    }

    /**
     *
     */
    void update(std::shared_ptr<ReaderEvent> event)
    {
        ReaderEvent::EventType type = event->getEventType();

        logger->debug("update - received event of type %s\n",
                      event->getEventType().toString().c_str());

        if (type == ReaderEvent::EventType::SE_MATCHED) {

            logger->debug("update - retrieve matching selection\n");
            std::shared_ptr<MatchingSelection> matchingSelection =
                seSelection
                    ->processDefaultSelection(
                        event->getDefaultSelectionsResponse())
                    ->getActiveSelection();

            logger->debug("update - retrieve calypso Po\n");
            std::shared_ptr<CalypsoPo> calypsoPo =
                std::dynamic_pointer_cast<CalypsoPo>(
                    matchingSelection->getMatchingSe());

            logger->debug("update - is selected ?\n");
            if (calypsoPo->isSelected()) {
                logger->debug("update - yes\n");
                std::shared_ptr<SeReader> poReader;
                try {
                    logger->debug("update - retrieve plugin\n");
                    poReader = SeProxyService::getInstance()
                                   .getPlugin(event->getPluginName())
                                   ->getReader(event->getReaderName());
                } catch (KeyplePluginNotFoundException& e) {
                    logger->error("update - caught "
                                  "KeyplePluginNotFoundException (msg: %s, "
                                  "cause: %s)\n",
                                  e.getMessage().c_str(), e.getCause().what());
                } catch (KeypleReaderNotFoundException& e) {
                    logger->error("update - caught "
                                  "KeypleReaderNotFoundException (msg: %s, "
                                  "cause: %s)\n",
                                  e.getMessage().c_str(), e.getCause().what());
                }

                logger->info("Observer notification: the selection of the PO "
                             "has succeeded\n");

                /*
                 * Retrieve the data read from the parser updated during the
                 * selection process
                 */
                std::shared_ptr<ReadRecordsRespPars> readEnvironmentParser =
                    std::dynamic_pointer_cast<ReadRecordsRespPars>(
                        matchingSelection->getResponseParser(
                            readEnvironmentParserIndex));

                std::vector<uint8_t> environmentAndHolder =
                    (*(readEnvironmentParser->getRecords().get()))
                        [static_cast<int>(CalypsoClassicInfo::RECORD_NUMBER_1)];

                /* Log the result */
                logger->info(
                    "Environment file data: %s\n",
                    ByteArrayUtil::toHex(environmentAndHolder).c_str());

                /*
                 * Go on with the reading of the first record of the EventLog
                 * file
                 */
                logger->info("==============================================="
                             "===================================\n");
                logger->info("= 2nd PO exchange: reading transaction of the "
                             "EventLog file.                     =\n");
                logger->info("==============================================="
                             "===================================\n");

                std::shared_ptr<PoTransaction> poTransaction =
                    std::make_shared<PoTransaction>(
                        std::make_shared<PoResource>(poReader, calypsoPo));

                /*
                 * Prepare the reading order and keep the associated parser for
                 * later use once the transaction has been processed.
                 */
                int readEventLogParserIndex =
                    poTransaction->prepareReadRecordsCmd(
                        CalypsoClassicInfo::SFI_EventLog,
                        ReadDataStructure::SINGLE_RECORD_DATA,
                        CalypsoClassicInfo::RECORD_NUMBER_1,
                        StringHelper::formatSimple(
                            "EventLog (SFI=%02X, recnbr=%d))",
                            CalypsoClassicInfo::SFI_EventLog,
                            CalypsoClassicInfo::RECORD_NUMBER_1));

                /*
                 * Actual PO communication: send the prepared read order, then
                 * close the channel with the PO
                 */
                try {
                    if (poTransaction->processPoCommands(
                            ChannelControl::CLOSE_AFTER)) {

                        logger->info("The reading of the EventLog has "
                                     "succeeded\n");

                        /*
                         * Retrieve the data read from the parser updated during
                         * the transaction process
                         */
                        std::shared_ptr<ReadRecordsRespPars> parser =
                            std::dynamic_pointer_cast<ReadRecordsRespPars>(
                                poTransaction->getResponseParser(
                                    readEventLogParserIndex));

                        std::vector<uint8_t> eventLog = (*(
                            parser->getRecords()
                                .get()))[CalypsoClassicInfo::RECORD_NUMBER_1];

                        /* Log the result */
                        logger->info("EventLog file data: %s\n",
                                     ByteArrayUtil::toHex(eventLog).c_str());
                    }
                } catch (const KeypleReaderException& e) {
                    logger->error("update - caught KeypleReaderException "
                                  "(msg: %s, cause: %s)\n",
                                  e.getMessage().c_str(), e.getCause().what());
                }

                logger->info("==========================================="
                             "=======================================\n");
                logger->info("= End of the Calypso PO processing.        "
                             "                                      =\n");
                logger->info("==========================================="
                             "=======================================\n");
            } else {
                logger->error("The selection of the PO has failed. Should"
                              " not have occurred due to the MATCHED_ONLY"
                              "selection mode\n");
            }
        } else if (type == ReaderEvent::EventType::SE_INSERTED) {
            logger->error("update - SE_INSERTED event: should not have "
                          "occurred due to the MATCHED_ONLY selection mode\n");
        } else if (type == ReaderEvent::EventType::SE_REMOVED) {
            logger->info("update - the PO has been removed\n");
        } else {
            logger->info("update - unexpected event\n");
        }

        if (event->getEventType() == ReaderEvent::EventType::SE_INSERTED ||
            event->getEventType() == ReaderEvent::EventType::SE_MATCHED) {
            /*
             * Informs the underlying layer of the end of the SE processing, in
             * order to manage the removal sequence. <p>If closing has already
             * been requested, this method will do nothing.
             */
            try {
                std::dynamic_pointer_cast<StubReaderImpl>(
                    SeProxyService::getInstance()
                        .getPlugin(event->getPluginName())
                        ->getReader(event->getReaderName()))
                    ->notifySeProcessed();
            } catch (KeypleReaderNotFoundException& e) {
                logger->debug("KeypleReaderNotFoundException: %s - %s\n",
                              e.getMessage().c_str(), e.getCause().what());
            } catch (KeyplePluginNotFoundException& e) {
                logger->debug("KeyplePluginNotFoundException: %s - %s\n",
                              e.getMessage().c_str(), e.getCause().what());
            }
        }
    }
};

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    /* Create the observable object to handle the PO processing */
    std::shared_ptr<UseCase_Calypso2_DefaultSelectionNotification_Stub> m =
        std::make_shared<UseCase_Calypso2_DefaultSelectionNotification_Stub>();
    m->init();
}
