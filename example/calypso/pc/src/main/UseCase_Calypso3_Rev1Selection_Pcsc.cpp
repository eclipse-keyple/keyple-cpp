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
#include "PoSelectionRequest.h"
#include "SeProxyService.h"
#include "SeReader.h"

/* Example */
#include "CalypsoClassicInfo.h"
#include "CalypsoClassicTransactionEngine.h"
#include "CalypsoUtilities.h"

/* Common */
#include "IllegalStateException.h"
#include "LoggerFactory.h"

/* Plugin */
#include "PcscPluginFactory.h"
#include "PcscReadersSettings.h"
#include "PcscProtocolSetting.h"

/* Common */
#include "stringhelper.h"

/* Calypso */
#include "CalypsoPo.h"
#include "CalypsoPoCommandException.h"
#include "CalypsoPoTransactionException.h"
#include "ElementaryFile.h"

using namespace keyple::calypso::command::po::exception;
using namespace keyple::calypso::transaction;
using namespace keyple::calypso::transaction::exception;
using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;
using namespace keyple::example::calypso::common::postructure;
using namespace keyple::example::calypso::common::transaction;
using namespace keyple::plugin::pcsc;

using AidSelector = SeSelector::AidSelector;
using AtrFilter = SeSelector::AtrFilter;
using InvalidatedPo = PoSelector::InvalidatedPo;

class UseCase_Calypso3_Rev1Selection_Pcsc {
};

const std::shared_ptr<Logger> logger =
    LoggerFactory::getLogger(typeid(UseCase_Calypso3_Rev1Selection_Pcsc));
const std::string PO_ATR_REGEX = ".*";
const std::string PO_DF_RT_PATH = "2000";

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    /* Get the instance of the SeProxyService (Singleton pattern) */
    SeProxyService& seProxyService = SeProxyService::getInstance();

    /* Assign PcscPlugin to the SeProxyService */
    seProxyService.registerPlugin(std::make_shared<PcscPluginFactory>());

    /*
     * Get a PO reader ready to work with Calypso PO. Use the getReader helper method from the
     * CalypsoUtilities class.
     */
    std::shared_ptr<SeReader> poReader = CalypsoUtilities::getDefaultPoReader();

    poReader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_B_PRIME,
        PcscProtocolSetting::PCSC_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_B_PRIME]);

    logger->info("=============== UseCase Calypso #1: ATR based explicit selection (PO Rev1) ====" \
                 "=======\n");
    logger->info("= PO Reader  NAME = %\n", poReader->getName());

    /* Check if a PO is present in the reader */
    if (poReader->isSePresent()) {

        logger->info("= #### 1st PO exchange: ATR based selection with reading of Environment " \
                     "file\n");

        /*
         * Prepare a Calypso PO selection Setting up a selection of a Calypso REV1 PO (B Prime)
         * based on ATR
         *
         * Select the first application matching the selection.
         */
        SeSelection seSelection;

        /*
         * Calypso selection: configures a PoSelector with all the desired attributes to make the
         * selection and read additional information afterwards
         */
        auto atrFilter = std::make_shared<AtrFilter>(PO_ATR_REGEX);
        auto seSelector = PoSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_B_PRIME)
                                                .atrFilter(atrFilter)
                                                .invalidatedPo(InvalidatedPo::REJECT)
                                                .build();
        auto poSelector = std::dynamic_pointer_cast<PoSelector>(seSelector);
        auto poSelectionRequest = std::make_shared<PoSelectionRequest>(poSelector);

        /* Prepare the selection of the DF RT */
        poSelectionRequest->prepareSelectFile(ByteArrayUtil::fromHex(PO_DF_RT_PATH));

        /* Prepare the reading order */
        poSelectionRequest->prepareReadRecordFile(CalypsoClassicInfo::SFI_EnvironmentAndHolder,
                                                  CalypsoClassicInfo::RECORD_NUMBER_1);

        /*
         * Add the selection case to the current selection (we could have added
         * other cases here)
         */
        auto abstract = std::reinterpret_pointer_cast
                            <AbstractSeSelectionRequest<AbstractApduCommandBuilder>>(
                                poSelectionRequest);
        seSelection.prepareSelection(abstract);

        /*
         * Actual PO communication: operate through a single request the Calypso PO selection and
         * the file read
         */
        std::shared_ptr<SelectionsResult> results = seSelection.processExplicitSelection(poReader);
        std::shared_ptr<AbstractMatchingSe> matchingSe = results->getActiveMatchingSe();
        auto calypsoPo = std::dynamic_pointer_cast<CalypsoPo>(matchingSe);
        logger->info("The selection of the PO has succeeded\n");

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
         * transaction has been processed. We provide the expected record length since the REV1
         * PO need it. TODO Check if we need to specify the expected length (29 bytes here)
         */
        poTransaction.prepareReadRecordFile(CalypsoClassicInfo::SFI_EventLog,
                                            CalypsoClassicInfo::RECORD_NUMBER_1);

        /*
         * Actual PO communication: send the prepared read order, then close the channel with the PO
         */
        poTransaction.processPoCommands(ChannelControl::CLOSE_AFTER);
        logger->info("The reading of the EventLog has succeeded\n");

        /* Retrieve the data read from the CalyspoPo updated during the transaction process */
        std::shared_ptr<ElementaryFile> efEventLog =
            calypsoPo->getFileBySfi(CalypsoClassicInfo::SFI_EventLog);
        const std::string eventLog = ByteArrayUtil::toHex(efEventLog->getData()->getContent());

        /* Log the result */
        logger->info("EventLog file data: %\n", eventLog);

        logger->info("= #### End of the Calypso PO processing\n");
    } else {
        logger->error("The selection of the PO has failed\n");
    }

    return 0;
}
