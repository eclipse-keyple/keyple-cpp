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

#include "CalypsoClassicInfo.h"
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
#include "SeSelection.h"

/* Common */
#include "ByteArrayUtil.h"
#include "stringhelper.h"

using namespace keyple::common;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::example::calypso::common::postructure;
using namespace keyple::example::calypso::common::transaction;
using namespace keyple::example::calypso::pc;
using namespace keyple::example::generic::pc;
using namespace keyple::plugin::pcsc;

class UseCase_Calypso3_Rev1Selection_Pcsc {
};

const std::shared_ptr<Logger> logger =
    LoggerFactory::getLogger(typeid(UseCase_Calypso3_Rev1Selection_Pcsc));
const std::string poAtrRegex = ".*";
const std::string poDfRtPath = "2000";

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    /* Get the instance of the PC/SC plugin */
    PcscPlugin pcscPlugin = PcscPlugin::getInstance();
    pcscPlugin.initReaders();
    std::shared_ptr<PcscPlugin> shared_plugin =
        std::shared_ptr<PcscPlugin>(&pcscPlugin);

    /* Assign PcscPlugin to the SeProxyService */
    SeProxyService& seProxyService = SeProxyService::getInstance();
    seProxyService.addPlugin(shared_plugin);

    /*
     * Get a PO reader ready to work with Calypso PO. Use the getReader helper
     * method from the CalypsoUtilities class.
     */
    std::shared_ptr<SeReader> poReader = CalypsoUtilities::getDefaultPoReader();

    /* Check if the reader exists */
    if (poReader == nullptr) {
        throw std::make_shared<IllegalStateException>("Bad PO reader setup");
    }

    logger->info("=============== UseCase Calypso #1: ATR based explicit "
                 "selection (PO Rev1) ===========\n");
    logger->info("= PO Reader  NAME = %s\n", poReader->getName().c_str());

    /* Check if a PO is present in the reader */
    if (poReader->isSePresent()) {

        logger->info("======================================================="
                     "===========================\n");
        logger->info("= 1st PO exchange: ATR based selection with reading of "
                     "Environment file.         =\n");
        logger->info("======================================================="
                     "===========================\n");

        /*
         * Prepare a Calypso PO selection
         */
        std::shared_ptr<SeSelection> seSelection =
            std::make_shared<SeSelection>();

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
                    SeCommonProtocols::PROTOCOL_ISO14443_4,
                    std::make_shared<PoSelector::PoAtrFilter>(poAtrRegex),
                    nullptr, StringHelper::formatSimple("ATR: ", poAtrRegex)),
                ChannelState::KEEP_OPEN);

        /*
         * Prepare the selection of the DF RT.
         */
        std::vector<uint8_t> dfrt = ByteArrayUtil::fromHex(poDfRtPath);
        int selectFileParserIndex = poSelectionRequest->prepareSelectFileCmd(
            dfrt, StringHelper::formatSimple("Select file: %s", poDfRtPath));

        /*
         * Prepare the reading order and keep the associated parser for later
         * use once the selection has been made.
         */
        int readEnvironmentParserIndex =
            poSelectionRequest->prepareReadRecordsCmd(
                CalypsoClassicInfo::SFI_EnvironmentAndHolder,
                ReadDataStructure::SINGLE_RECORD_DATA,
                CalypsoClassicInfo::RECORD_NUMBER_1,
                StringHelper::formatSimple(
                    "EnvironmentAndHolder (SFI=%02X))",
                    CalypsoClassicInfo::SFI_EnvironmentAndHolder));

        /*
         * Add the selection case to the current selection (we could have added
         * other cases here)
         */
        seSelection->prepareSelection(poSelectionRequest);

        /*
         * Actual PO communication: operate through a single request the Calypso
         * PO selection and the file read
         */
        std::shared_ptr<SelectionsResult> selectionResult =
            seSelection->processExplicitSelection(poReader);

        if (selectionResult->hasActiveSelection()) {
            std::shared_ptr<MatchingSelection> matchingSelection =
                selectionResult->getActiveSelection();

            std::shared_ptr<CalypsoPo> calypsoPo =
                std::static_pointer_cast<CalypsoPo>(
                    matchingSelection->getMatchingSe());
            logger->info("The selection of the PO has succeeded\n");

            std::shared_ptr<SelectFileRespPars> selectFileRespPars =
                std::dynamic_pointer_cast<SelectFileRespPars>(
                    matchingSelection->getResponseParser(
                        selectFileParserIndex));

            std::shared_ptr<ReadRecordsRespPars> readEnvironmentParser =
                std::dynamic_pointer_cast<ReadRecordsRespPars>(
                    matchingSelection->getResponseParser(
                        readEnvironmentParserIndex));

            logger->info(
                "DF RT FCI: %s\n",
                ByteArrayUtil::toHex(selectFileRespPars->getSelectionData())
                    .c_str());

            /*
             * Retrieve the data read from the parser updated during the
             * selection process
             */
            std::vector<uint8_t> environmentAndHolder =
                (*(readEnvironmentParser->getRecords().get()))[static_cast<int>(
                    CalypsoClassicInfo::RECORD_NUMBER_1)];

            /* Log the result */
            logger->info("Environment file data: %s\n",
                         ByteArrayUtil::toHex(environmentAndHolder).c_str());

            /*
             * Go on with the reading of the first record of the EventLog file
             */
            logger->info("==================================================="
                         "===============================\n");
            logger->info("= 2nd PO exchange: reading transaction of the Event"
                         "Log file.                     =\n");
            logger->info("==================================================="
                         "===============================\n");

            std::shared_ptr<PoTransaction> poTransaction =
                std::make_shared<PoTransaction>(
                    std::make_shared<PoResource>(poReader, calypsoPo));

            /*
             * Prepare the reading order and keep the associated parser for
             * later use once the transaction has been processed.
             */
            int readEventLogParserIndex = poTransaction->prepareReadRecordsCmd(
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
            if (poTransaction->processPoCommands(ChannelState::CLOSE_AFTER)) {
                logger->info("The reading of the EventLog has succeeded.");

                /*
                 * Retrieve the data read from the parser updated during the
                 * transaction process
                 */
                std::shared_ptr<ReadRecordsRespPars> parser =
                    std::dynamic_pointer_cast<ReadRecordsRespPars>(
                        poTransaction->getResponseParser(
                            readEventLogParserIndex));

                std::vector<uint8_t> eventLog =
                    (*(parser->getRecords()
                           .get()))[CalypsoClassicInfo::RECORD_NUMBER_1];

                /* Log the result */
                logger->info("EventLog file data: %s\n",
                             ByteArrayUtil::toHex(eventLog).c_str());
            }

            logger->info("==================================================="
                         "===============================\n");
            logger->info("= End of the Calypso PO processing.                "
                         "                              =\n");
            logger->info("==================================================="
                         "===============================\n");
        } else {
            logger->error("The selection of the PO has failed\n");
        }
    } else {
        logger->error("No PO were detected\n");
    }

    return 0;
}
