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

class UseCase_Calypso4_PoAuthentication_Pcsc {
};

const std::shared_ptr<Logger> logger =
    LoggerFactory::getLogger(typeid(UseCase_Calypso4_PoAuthentication_Pcsc));

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

    const std::vector<uint8_t>& serialNumber = samResource->getMatchingSe()->getSerialNumber();
    const std::string samSerialNumber = ByteArrayUtil::toHex(serialNumber);
    logger->info("=============== UseCase Calypso #4: Po Authentication ==================\n");
    logger->info("= PO Reader  NAME = %\n", poReader->getName());
    logger->info("= SAM Reader  NAME = %, SERIAL NUMBER = %\n",
                 samResource->getSeReader()->getName(),
                 samSerialNumber);

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

        /* Prepare the reading of the Environment and Holder file */
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

        /*
         * All data collected from the PO are available in CalypsoPo
         * Get the Environment and Holder data
         */
        std::shared_ptr<ElementaryFile> efEnvironmentAndHolder =
            calypsoPo->getFileBySfi(CalypsoClassicInfo::SFI_EnvironmentAndHolder);
        const std::string environmentAndHolder =
            ByteArrayUtil::toHex(efEnvironmentAndHolder->getData()->getContent());
        logger->info("File Environment and Holder: %\n", environmentAndHolder);

        /* Go on with the reading of the first record of the EventLog file */
        logger->info("= ##### 2nd PO exchange: open and close a secure session to perform " \
                     "authentication\n");

        PoTransaction poTransaction(std::make_shared<SeResource<CalypsoPo>>(poReader, calypsoPo),
                                    CalypsoUtilities::getSecuritySettings(samResource));

        /* Read the EventLog file at the Session Opening */
        poTransaction.prepareReadRecordFile(CalypsoClassicInfo::SFI_EventLog,
                                            CalypsoClassicInfo::RECORD_NUMBER_1);

        /* Open Session for the debit key */
        poTransaction.processOpening(AccessLevel::SESSION_LVL_DEBIT);

        /* Get the EventLog data */
        std::shared_ptr<ElementaryFile> efEventLog =
            calypsoPo->getFileBySfi(CalypsoClassicInfo::SFI_EventLog);
        const std::string eventLog = ByteArrayUtil::toHex(efEventLog->getData()->getContent());
        logger->info("File Event log: %\n", eventLog);

        if (!calypsoPo->isDfRatified())
            logger->info("========= Previous Secure Session was not ratified. ===================" \
                         "==\n");

        /* Read the ContractList file inside the Secure Session */
        poTransaction.prepareReadRecordFile(CalypsoClassicInfo::SFI_ContractList,
                                            CalypsoClassicInfo::RECORD_NUMBER_1);

        poTransaction.processPoCommands();

        /* Get the ContractList data */
        std::shared_ptr<ElementaryFile> efContractList =
            calypsoPo->getFileBySfi(CalypsoClassicInfo::SFI_ContractList);
        const std::vector<uint8_t>& contract = efContractList->getData()->getContent();
        const std::string contractList = ByteArrayUtil::toHex(contract);
        logger->info("File Contract List: %\n", contractList);

        /* Append a new record to EventLog. Just increment the first byte */
        std::vector<uint8_t> log = efEventLog->getData()->getContent();
        log[0] += 1;
        poTransaction.prepareAppendRecord(CalypsoClassicInfo::SFI_EventLog, log);

        /* Execute Append Record and close the Secure Session */
        logger->info("========= PO Calypso session ======= Closing ============================\n");

        /* A ratification command will be sent (CONTACTLESS_MODE) */
       poTransaction.prepareReleasePoChannel();
        poTransaction.processPoCommands();

        logger->info("The Calypso session ended successfully\n");

        logger->info("= ##### End of the Calypso PO processing\n");
    } else {
        logger->error("The selection of the PO has failed\n");
    }

    return 0;
}
