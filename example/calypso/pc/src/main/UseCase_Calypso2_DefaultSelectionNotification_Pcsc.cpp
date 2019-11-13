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
#include "CalypsoUtilities.h"
#include "KeypleBaseException.h"
#include "ReaderUtilities.h"
#include "KeypleReaderNotFoundException.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "MatchingSelection.h"
#include "ObservableReader_Import.h"
#include "PcscPlugin.h"
#include "PcscReader.h"
#include "PcscReadersSettings.h"
#include "PcscReaderSettings_Import.h"
#include "PcscReadersSettings.h"
#include "PcscProtocolSetting.h"
#include "PcscReadersSettings.h"
#include "PoSelectionRequest.h"
#include "ReaderEvent_Import.h"
#include "SeProxyService.h"
#include "SeReader.h"

/* Common */
#include "stringhelper.h"

using namespace keyple::common;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::example::calypso::common::transaction;
using namespace keyple::example::calypso::common::postructure;
using namespace keyple::example::calypso::pc;
using namespace keyple::example::generic::pc;
using namespace keyple::plugin::pcsc;

class UseCase_Calypso2_DefaultSelectionNotification_Pcsc
: public std::enable_shared_from_this<
             UseCase_Calypso2_DefaultSelectionNotification_Pcsc>,
  public ObservableReader::ReaderObserver {
private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
            LoggerFactory::getLogger(
                typeid(UseCase_Calypso2_DefaultSelectionNotification_Pcsc));

    /**
     *
     */
    std::shared_ptr<SeSelection> seSelection;

    /**
     *
     */
    int readEnvironmentParserIndex;

public:
    /**
     *
     */
    UseCase_Calypso2_DefaultSelectionNotification_Pcsc()
    {
        /* Get the instance of the PC/SC plugin */
        PcscPlugin pcscPlugin = PcscPlugin::getInstance();
        pcscPlugin.initReaders();
        std::shared_ptr<PcscPlugin> shared_plugin =
            std::shared_ptr<PcscPlugin>(&pcscPlugin);

        /* Assign PcscPlugin to the SeProxyService */
        SeProxyService& seProxyService = SeProxyService::getInstance();
        seProxyService.addPlugin(shared_plugin);

        /*
         * Get a PO reader ready to work with Calypso PO. Use the getReader
         * helper method from the CalypsoUtilities class.
         */
        std::shared_ptr<SeReader> poReader =
            CalypsoUtilities::getDefaultPoReader();

        /* Check if the reader exists */
        if (poReader == nullptr) {
            throw std::make_shared<IllegalStateException>(
                      "Bad PO reader setup");
        }

        logger->info("=============== UseCase Calypso #2: AID based default " \
                     "selection ===================\n");
        logger->info("= PO Reader  NAME = %s\n", poReader->getName());

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
                                               CalypsoClassicInfo::AID)),
                ChannelState::KEEP_OPEN);

        /*
         * Prepare the reading order and keep the associated parser for later
         * use once the selection has been made.
         */
        readEnvironmentParserIndex =
            poSelectionRequest->prepareReadRecordsCmd(
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

        /*UseCase_Calypso2_DefaultSelectionNotification_Pcsc
         * Provide the SeReader with the selection operation to be processed
         * when a PO is inserted.
         */
        (std::dynamic_pointer_cast<ObservableReader>(poReader))
            ->setDefaultSelectionRequest(
                seSelection->getSelectionOperation(),
                ObservableReader::NotificationMode::MATCHED_ONLY);

        /* Set the current class as Observer of the first reader */
        std::shared_ptr<UseCase_Calypso2_DefaultSelectionNotification_Pcsc>
            shared_this(this);
        (std::dynamic_pointer_cast<ObservableReader>(poReader))
            ->addObserver(shared_from_this());

        logger->info("=======================================================" \
                     "===========================\n");
        logger->info("= Wait for a PO. The default AID based selection with " \
                     "reading of Environment     =\n");
        logger->info("= file is ready to be processed as soon as the PO is " \
                     "detected.                   =\n");
        logger->info("=======================================================" \
                     "===========================\n");

        /* Wait for ever (exit with CTRL-C) */
        while(1);
    }

    /**
     *
     */
    virtual ~UseCase_Calypso2_DefaultSelectionNotification_Pcsc() {}

    void update(std::shared_ptr<ReaderEvent> event)
    {
        ReaderEvent::EventType type = event->getEventType();

        if (type == ReaderEvent::EventType::SE_MATCHED) {
            std::shared_ptr<MatchingSelection> matchingSelection
                = seSelection->processDefaultSelection(
                      event->getDefaultSelectionsResponse())
                      ->getActiveSelection();

            std::shared_ptr<SeReader> poReader;
            try {
                poReader = SeProxyService::getInstance().getPlugin(
                               event->getPluginName())
                               ->getReader(event->getReaderName());
            } catch (KeyplePluginNotFoundException& e) {
                logger->error("update - caught KeyplePluginNotFoundException " \
                              "(msg: %s, cause: %s)\n", e.getMessage(),
                              e.getCause().what());
            } catch (KeypleReaderNotFoundException& e) {
                logger->error("update - caught KeypleReaderNotFoundException " \
                              "(msg: %s, cause: %s)\n", e.getMessage(),
                              e.getCause().what());
            }

            std::shared_ptr<CalypsoPo> calypsoPo =
                std::dynamic_pointer_cast<CalypsoPo>(
                    matchingSelection->getMatchingSe());

            logger->info("Observer notification: the selection of the PO has " \
                         "succeeded\n");

            std::shared_ptr<ReadRecordsRespPars> readEnvironmentParser =
                std::dynamic_pointer_cast<ReadRecordsRespPars>(
                    matchingSelection->getResponseParser(
                        readEnvironmentParserIndex));

            std::vector<char> environmentAndHolder =
                (*(readEnvironmentParser->getRecords().get()))[
                    static_cast<int>(CalypsoClassicInfo::RECORD_NUMBER_1)];

            logger->info("Environment file data: %s\n",
                         ByteArrayUtil::toHex(environmentAndHolder));

            /*
             * Go on with the reading of the first record of the EventLog file
             */
            logger->info("===================================================" \
                         "===============================\n");
            logger->info("= 2nd PO exchange: reading transaction of the " \
                         "EventLog file.                     =\n");
            logger->info("===================================================" \
                         "===============================\n");

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
             * Actual PO communication: send the prepared read order, then close
             * the channel with the PO
             */
            try {
                if (poTransaction->processPoCommands(ChannelState::CLOSE_AFTER))
                {
                    logger->info("The reading of the EventLog has succeeded\n");

                    /*
                     * Retrieve the data read from the parser updated during the
                     * transaction process
                     */
                    std::shared_ptr<ReadRecordsRespPars> parser =
                        std::dynamic_pointer_cast<ReadRecordsRespPars>(
                            poTransaction->getResponseParser(
                                readEventLogParserIndex));
                    std::vector<char> eventLog =
                        (*(parser->getRecords().get()))[
                            CalypsoClassicInfo::RECORD_NUMBER_1];

                    /* Log the result */
                    logger->info("EventLog file data: %s\n",
                                 ByteArrayUtil::toHex(eventLog));
                }
            } catch (const KeypleReaderException &e) {
                logger->error("update - caught KeypleReaderException " \
                              "(msg: %s, cause: %s)\n", e.getMessage(),
                              e.getCause().what());
            }

            logger->info("===================================================" \
                         "===============================\n");
            logger->info("= End of the Calypso PO processing.                " \
                         "                              =\n");
            logger->info("===================================================" \
                         "===============================\n");

        } else if (type == ReaderEvent::EventType::SE_INSERTED) {
            logger->error("SE_INSERTED event: should not have occurred due to "\
                          "the MATCHED_ONLY selection mode\n");
        } else if (type == ReaderEvent::EventType::SE_REMOVAL) {
            logger->info("The PO has been removed\n");
        } else {
        }
    }
};

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    /* Create the observable object to handle the PO processing */
    std::shared_ptr<UseCase_Calypso2_DefaultSelectionNotification_Pcsc> m =
        std::make_shared<UseCase_Calypso2_DefaultSelectionNotification_Pcsc>();
}

