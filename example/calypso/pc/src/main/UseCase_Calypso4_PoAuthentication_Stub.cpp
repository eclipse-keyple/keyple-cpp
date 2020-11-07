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
#include "CalypsoUtilities.h"

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
    auto factory = std::make_shared<StubPluginFactory>(STUB_PLUGIN_NAME);
    std::shared_ptr<ReaderPlugin> stubPlugin = seProxyService.registerPlugin(factory);

    /* Plug the PO stub reader */
    std::dynamic_pointer_cast<StubPlugin>(stubPlugin)->plugStubReader("poReader", true);
    std::dynamic_pointer_cast<StubPlugin>(stubPlugin)->plugStubReader("samReader", true);

    /* Get a PO and a SAM reader ready to work with a Calypso PO */
    std::shared_ptr<SeReader> poReader = stubPlugin->getReader("poReader");
    std::shared_ptr<SeReader> samReader = stubPlugin->getReader("samReader");

    // Check if the reader exists
    if (poReader == nullptr || samReader == nullptr)
        throw IllegalStateException("Bad PO or SAM reader setup");

    samReader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO7816_3,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO7816_3]);

    /* Create 'virtual' Calypso PO */
    std::shared_ptr<StubSecureElement> calypsoStubSe = std::make_shared<StubCalypsoClassic>();
    logger->info("Insert stub PO\n");
    std::dynamic_pointer_cast<StubReader>(poReader)->insertSe(calypsoStubSe);

    /* Create 'virtual' Calypso SAM */
    std::shared_ptr<StubSecureElement> calypsoSamStubSe = std::make_shared<StubSamCalypsoClassic>();
    logger->info("Insert stub SAM.");
    std::dynamic_pointer_cast<StubReader>(samReader)->insertSe(calypsoSamStubSe);

    /*
     * Open logical channel for the SAM inserted in the reader
     * (We expect the right is inserted)
     */
    std::shared_ptr<SeResource<CalypsoSam>> samResource =
        CalypsoUtilities::checkSamAndOpenChannel(samReader);

    logger->info("=============== UseCase Calypso #4: Po Authentication ==================\n");
    logger->info("= PO Reader  NAME = %\n", poReader->getName());
    logger->info("= SAM Reader  NAME = %\n", samReader->getName());

    poReader->addSeProtocolSetting(
        SeCommonProtocols::PROTOCOL_ISO14443_4,
        StubProtocolSetting::STUB_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

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
                         "==");

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

        /*
         * Append a new record to EventLog.
         * Does not change the content as in the PC/SC example due to the Stub limitations
         */
        poTransaction.prepareAppendRecord(CalypsoClassicInfo::SFI_EventLog,
                                          efEventLog->getData()->getContent());

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
