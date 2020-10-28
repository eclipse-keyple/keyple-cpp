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

/* Plugin */
#include "PcscPluginFactory.h"
#include "PcscReadersSettings.h"

/* Common */
#include "LoggerFactory.h"

/* Core */
#include "PoSelectionRequest.h"
#include "SeCommonProtocols.h"
#include "SeResource.h"

/* Calypso */
#include "CalypsoPo.h"
#include "CalypsoSam.h"
#include "ElementaryFile.h"
#include "PoSecuritySettings.h"

using namespace keyple::calypso::transaction;
using namespace keyple::common;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;
using namespace keyple::example::calypso::common::postructure;
using namespace keyple::example::calypso::common::transaction;
using namespace keyple::plugin::pcsc;

using AccessLevel = PoTransaction::SessionSetting::AccessLevel;
using AidSelector = SeSelector::AidSelector;
using InvalidatedPo = PoSelector::InvalidatedPo;
using ModificationMode = PoTransaction::SessionSetting::ModificationMode;

class UseCase_Calypso5_MultipleSession_Pcsc {
};

const std::shared_ptr<Logger> logger =
    LoggerFactory::getLogger(typeid(UseCase_Calypso5_MultipleSession_Pcsc));

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

    /*
     * Get a SAM reader ready to work with Calypso PO. Use the getReader helper method from the
     * CalypsoUtilities class.
     */
    std::shared_ptr<SeResource<CalypsoSam>> samResource = CalypsoUtilities::getDefaultSamResource();

    logger->info("=============== UseCase Calypso #5: Po Authentication ==================\n");
    logger->info("= PO Reader  NAME = %\n", poReader->getName());
    logger->info("= SAM Reader  NAME = %\n", samResource->getSeReader()->getName());

    /* Check if a PO is present in the reader */
    if (poReader->isSePresent()) {

        logger->info("= ##### 1st PO exchange: AID based selection with reading of Environment " \
                     "file.");

        /* Prepare a Calypso PO selection */
        SeSelection seSelection;

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

        /* Go on with the reading of the first record of the EventLog file */
        logger->info("= ##### 2nd PO exchange: open and close a secure session to perform " \
                     "authentication\n");

        /* The default KIF values for debiting */
        const uint8_t DEFAULT_KIF_DEBIT = 0x30;

        /*
         * The default key record number values for debiting
         * The actual value should be adjusted.
         */
        const uint8_t DEFAULT_KEY_RECORD_NUMBER_DEBIT = 0x03;

        /* define the security parameters to provide when creating PoTransaction */
        std::shared_ptr<PoSecuritySettings> poSecuritySettings =
            PoSecuritySettings::PoSecuritySettingsBuilder(samResource)
                .sessionDefaultKif(AccessLevel::SESSION_LVL_DEBIT, DEFAULT_KIF_DEBIT)
                .sessionDefaultKeyRecordNumber(AccessLevel::SESSION_LVL_DEBIT,
                                               DEFAULT_KEY_RECORD_NUMBER_DEBIT)
                .sessionModificationMode(ModificationMode::MULTIPLE)
                .build();

        auto seResource = std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPo);
        PoTransaction poTransaction(seResource, poSecuritySettings);

        /* Open Session for the debit key */
        poTransaction.processOpening(AccessLevel::SESSION_LVL_DEBIT);

        if (!calypsoPo->isDfRatified())
            logger->info("========= Previous Secure Session was not ratified. ===================" \
                         "==\n");

        /*
         * Compute the number of append records (29 bytes) commands that will overflow the PO
         * modifications buffer. Each append records will consume 35 (29 + 6) bytes in the buffer.
         *
         * We'll send one more command to demonstrate the MULTIPLE mode
         */
        const int modificationsBufferSize = 430;
        const int nbCommands = (modificationsBufferSize / 35) + 1;

        logger->info("==== Send % Append Record commands. Modifications buffer capacity = % bytes" \
                     " i.e. % 29-byte commands ====\n",
                     nbCommands,
                     modificationsBufferSize,
                     modificationsBufferSize / 35);

        for (int i = 0; i < nbCommands; i++)
            poTransaction.prepareAppendRecord(
                CalypsoClassicInfo::SFI_EventLog,
                ByteArrayUtil::fromHex(CalypsoClassicInfo::eventLog_dataFill));

        /* Proceed with the sending of commands, don't close the channel */
        poTransaction.processPoCommandsInSession();

        /* Close the Secure Session */
        logger->info("========= PO Calypso session ======= Closing ============================\n");

        /* A ratification command will be sent (CONTACTLESS_MODE) */
        poTransaction.processClosing(ChannelControl::KEEP_OPEN);

        logger->info("= #### End of the Calypso PO processing\n");
    } else {
        logger->error("The selection of the PO has failed\n");
    }

    return 0;
}
