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
#include "KeypleReaderNotFoundException.h"
#include "LoggerFactory.h"
#include "MatchingSelection.h"
#include "ObservableReader.h"
#include "PoSelectionRequest.h"
#include "PoSelector.h"
#include "ReaderEvent.h"
#include "ReaderPlugin.h"
#include "ReadRecordsRespPars.h"
#include "SeCommonProtocols.h"
#include "SeProxyService.h"
#include "StubCalypsoClassic.h"
#include "StubSamCalypsoClassic.h"
#include "StubProtocolSetting.h"
#include "StubPlugin.h"
#include "StubPluginFactory.h"
#include "StubReader.h"

using namespace keyple::example::calypso::common::transaction;
using namespace keyple::example::calypso::common::postructure;
using namespace keyple::example::calypso::pc::stub::se;
using namespace keyple::plugin::stub;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;

class UseCase_Calypso4_PoAuthentication_Stub {
};

const std::shared_ptr<Logger> logger =
    LoggerFactory::getLogger(typeid(UseCase_Calypso4_PoAuthentication_Stub));

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    /* Get the instance of the SeProxyService (Singleton pattern) */
    SeProxyService& seProxyService = SeProxyService::getInstance();

    const std::string STUB_PLUGIN_NAME = "stub1";

    /* Register Stub plugin in the platform */
    seProxyService.registerPlugin(new StubPluginFactory(STUB_PLUGIN_NAME));
    ReaderPlugin* stubPlugin = seProxyService.getPlugin(STUB_PLUGIN_NAME);

    /* Plug PO and SAM stub reader. */
    dynamic_cast<StubPlugin*>(stubPlugin)->plugStubReader("poReader", true);
    dynamic_cast<StubPlugin*>(stubPlugin)->plugStubReader("samReader", true);

    /* Get a PO and a SAM reader ready to work with a Calypso PO */
    std::shared_ptr<StubReader> poReader =
        std::dynamic_pointer_cast<StubReader>(
            stubPlugin->getReader("poReader"));
    std::shared_ptr<StubReader> samReader =
        std::dynamic_pointer_cast<StubReader>(
            stubPlugin->getReader("samReader"));

    /* Check if the reader exists */
    if (poReader == nullptr || samReader == nullptr) {
        throw std::make_shared<IllegalStateException>(
            "Bad PO or SAM reader setup");
    }

    samReader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO7816_3,
        StubProtocolSetting::STUB_PROTOCOL_SETTING
            [SeCommonProtocols::PROTOCOL_ISO7816_3]);

    /* Create 'virtual' Calypso PO */
    std::shared_ptr<StubSecureElement> calypsoStubSe =
        std::make_shared<StubCalypsoClassic>();

    logger->info("Insert stub PO\n");
    poReader->insertSe(calypsoStubSe);

    /* Create 'virtual' Calypso SAM */
    std::shared_ptr<StubSecureElement> calypsoSamStubSe =
        std::make_shared<StubSamCalypsoClassic>();

    logger->info("Insert stub SAM\n");
    samReader->insertSe(calypsoSamStubSe);

    /*
     * Open logical channel for the SAM inserted in the reader
     *
     * (We expect the right is inserted)
     */
    std::shared_ptr<SamResource> samResource =
        CalypsoUtilities::checkSamAndOpenChannel(samReader);

    /* Check if the readers exists */
    if (poReader == nullptr || samReader == nullptr) {
        throw std::make_shared<IllegalStateException>(
            "Bad PO or SAM reader setup");
    }

    logger->info("=============== UseCase Calypso #4: Po Authentication ====="
                 "=============\n");
    logger->info("= PO Reader  NAME = %\n", poReader->getName());
    logger->info("= SAM Reader  NAME = %\n", samReader->getName());

    poReader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING
            [SeCommonProtocols::PROTOCOL_ISO14443_4]);

    /* Check if a PO is present in the reader */
    if (poReader->isSePresent()) {

        logger->info("======================================================="
                     "===========================\n");
        logger->info("= 1st PO exchange: AID based selection with reading of "
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
            std::make_shared<PoSelectionRequest>(std::make_shared<PoSelector>(
                SeCommonProtocols::PROTOCOL_ISO14443_4, nullptr,
                std::make_shared<PoSelector::PoAidSelector>(
                    std::make_shared<SeSelector::AidSelector::IsoAid>(
                        CalypsoClassicInfo::AID),
                    PoSelector::InvalidatedPo::REJECT),
                StringHelper::formatSimple("AID: %s",
                                           CalypsoClassicInfo::AID)));

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

            /*
             * Go on with the reading of the first record of the EventLog file
             */
            logger->info("==================================================="
                         "===============================\n");
            logger->info("= 2nd PO exchange: open and close a secure session "
                         "to perform authentication.    =\n");
            logger->info("==================================================="
                         "===============================\n");

            std::shared_ptr<PoTransaction> poTransaction =
                std::make_shared<PoTransaction>(
                    std::make_shared<PoResource>(poReader, calypsoPo),
                    samResource, CalypsoUtilities::getSecuritySettings());

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

            (void)readEventLogParserIndex;

            /*
             * Open Session for the debit key
             */
            try {
                bool poProcessStatus = poTransaction->processOpening(
                    PoTransaction::ModificationMode::ATOMIC,
                    PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT, 0, 0);

                if (!poProcessStatus) {
                    throw std::make_shared<IllegalStateException>(
                        "processingOpening failure.");
                }
            
				if (!poTransaction->wasRatified()) {
					logger->info("========= Previous Secure Session was not "
								 "ratified. =====================\n");
				}

				/*
				 * Prepare the reading order and keep the associated parser for
				 * later use once the transaction has been processed.
				 */
				int readEventLogParserIndexBis =
					poTransaction->prepareReadRecordsCmd(
						CalypsoClassicInfo::SFI_EventLog,
						ReadDataStructure::SINGLE_RECORD_DATA,
						CalypsoClassicInfo::RECORD_NUMBER_1,
						StringHelper::formatSimple(
							"EventLog (SFI=%02X, recnbr=%d))",
							CalypsoClassicInfo::SFI_EventLog,
							CalypsoClassicInfo::RECORD_NUMBER_1));

				poProcessStatus = poTransaction->processPoCommandsInSession();

				/*
				 * Retrieve the data read from the parser updated during the
				 * transaction process
				 */
				std::shared_ptr<ReadRecordsRespPars> parser =
					std::dynamic_pointer_cast<ReadRecordsRespPars>(
						poTransaction->getResponseParser(
							readEventLogParserIndexBis));
				std::vector<uint8_t> eventLog =
					(*(parser->getRecords()
						   .get()))[CalypsoClassicInfo::RECORD_NUMBER_1];

				/* Log the result */
				logger->info("EventLog file data: %\n", eventLog);

				if (!poProcessStatus) {
					throw std::make_shared<IllegalStateException>(
						"processPoCommandsInSession failure.");
				}

				/*
				 * Close the Secure Session.
				 */
				logger->info("========= PO Calypso session ======= Closing =="
							 "==========================\n");

				/*
				 * A ratification command will be sent (CONTACTLESS_MODE).
				 */
				poProcessStatus =
					poTransaction->processClosing(ChannelControl::CLOSE_AFTER);

				if (!poProcessStatus) {
					throw std::make_shared<IllegalStateException>(
						"processClosing failure.");
				}
            } catch (const std::invalid_argument& e) {
                logger->error("main - caught std::invalid_argument, msg: %s\n",
                              e.what());
                return -1;
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
