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

#include "UseCase_Calypso1_ExplicitSelectionAid_IOS.h"

/* Core */
#include "ByteArrayUtil.h"

/* Example */
#include "CalypsoClassicInfo.h"
#include "CalypsoClassicTransactionEngine.h"
#include "CalypsoUtilities.h"

/* Plugin */
#include "IOSPluginFactory.h"
#include "IOSProtocolSetting.h"
#include "IOSReaderImpl.h"

/* Common */
#include "LoggerFactory.h"
#include "IllegalStateException.h"

/* Core */
#include "PoSelectionRequest.h"
#include "SeCommonProtocols.h"

/* Calypso */
#include "CalypsoPo.h"
#include "ElementaryFile.h"

using namespace keyple::calypso::transaction;
using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::util;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::example::calypso::common::transaction;
using namespace keyple::example::calypso::common::postructure;
using namespace keyple::plugin::ios;

using AidSelector = SeSelector::AidSelector;
using InvalidatedPo = PoSelector::InvalidatedPo;

UseCase_Calypso1_ExplicitSelectionAid_IOS::UseCase_Calypso1_ExplicitSelectionAid_IOS(ViewController *VC)
{
    Logger::setLoggerLevel(Logger::Level::logTrace);

    mVC = VC;
}

void UseCase_Calypso1_ExplicitSelectionAid_IOS::update(const std::shared_ptr<ReaderEvent> event)
{
    logger->info("= #### 1st PO exchange: AID based selection with reading of Environment " \
                 "file\n");

    ReaderEvent::EventType type = event->getEventType();

    if (type == ReaderEvent::EventType::SE_MATCHED) {
        /* Prepare a Calypso PO selection */
        SeSelection seSelection;

        /*
         * Setting of an AID based selection of a Calypso REV3 PO
         * Select the first application matching the selection AID whatever the SE communication
         * protocol keep the logical channel open after the selection
         */

        /*
         * Calypso selection: configures a PoSelectionRequest with all the desired attributes to
         * make the selection and read additional information afterwards
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

        /*
         * Add the selection case to the current selection (we could have added other cases here)
         * Ignore the returned index since we have only one selection here.
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

        /* Prepare the reading order */
        poTransaction.prepareReadRecordFile(CalypsoClassicInfo::SFI_EventLog,
                                            CalypsoClassicInfo::RECORD_NUMBER_1);

        /*
         * Actual PO communication: send the prepared read order, then close the channel with the PO
         */
        poTransaction.prepareReleasePoChannel();
        poTransaction.processPoCommands();
        logger->info("The reading of the EventLog has succeeded\n");

        /* Retrieve the data read from the CalyspoPo updated during the transaction process */
        std::shared_ptr<ElementaryFile> efEventLog =
            calypsoPo->getFileBySfi(CalypsoClassicInfo::SFI_EventLog);
        const std::string eventLog = ByteArrayUtil::toHex(efEventLog->getData()->getContent());

        /* Log the result */
        logger->info("EventLog file data: %\n", eventLog);

        std::string msg = "EventLog file data: ";
        msg.append(eventLog);
        success(msg);

        logger->info("= #### End of the Calypso PO processing\n");
    }

    SeProxyService::getInstance().unregisterPlugin("IOSPlugin");
}

void UseCase_Calypso1_ExplicitSelectionAid_IOS::start()
{
    /* Get the instance of the SeProxyService (Singleton pattern) */
    SeProxyService& seProxyService = SeProxyService::getInstance();

    /* Assign IOSPlugin to the SeProxyService */
    seProxyService.registerPlugin(std::make_shared<IOSPluginFactory>());

    /* Get a PO reader ready to work with Calypso PO */
    poReader = SeProxyService::getInstance().getPlugins().at("IOSPlugin")
                                           ->getReaders().at("IOSReader");
    if (poReader == nullptr)
        throw IllegalStateException("Bad PO reader setup");

    poReader->addSeProtocolSetting(
            SeCommonProtocols::PROTOCOL_ISO14443_4,
            IOSProtocolSetting::IOS_PROTOCOL_SETTING[SeCommonProtocols::PROTOCOL_ISO14443_4]);

    reset();

    /*
     * Prepare a Calypso PO selection
     */
    seSelection = std::make_shared<SeSelection>();

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

    /* Prepare the reading */
    poSelectionRequest->prepareReadRecordFile(CalypsoClassicInfo::SFI_EnvironmentAndHolder,
                                              CalypsoClassicInfo::RECORD_NUMBER_1);

    /*
     * Add the selection case to the current selection (we could have added
     * other cases here)
     */
    auto abstract = std::reinterpret_pointer_cast
                        <AbstractSeSelectionRequest<AbstractApduCommandBuilder>>(
                            poSelectionRequest);
    seSelection->prepareSelection(abstract);

    /*
     * Provide the SeReader with the selection operation to be processed
     * when a PO is inserted.
     */
    (std::dynamic_pointer_cast<ObservableReader>(poReader))
        ->setDefaultSelectionRequest(
            seSelection->getSelectionOperation(),
            ObservableReader::NotificationMode::MATCHED_ONLY,
            ObservableReader::PollingMode::REPEATING);

    std::dynamic_pointer_cast<ObservableReader>(poReader)->addObserver(shared_from_this());
    
    logger->info("=============== UseCase Calypso #1: AID based explicit selection ==============" \
                 "====\n");
    logger->info("= PO Reader  NAME = %\n", poReader->getName());
}

void UseCase_Calypso1_ExplicitSelectionAid_IOS::success(const std::string& msg)
{
    NSString *nsMsg = [[NSString alloc] initWithCString:msg.c_str() encoding:NSUTF8StringEncoding];

    dispatch_async(dispatch_get_main_queue(), ^{
        [mVC.resultTextView setText:nsMsg];
        [mVC.resultTextView setBackgroundColor:[UIColor systemGreenColor]];
    });
}

void UseCase_Calypso1_ExplicitSelectionAid_IOS::failure(const std::string& msg)
{
    NSString *nsMsg = [[NSString alloc] initWithCString:msg.c_str() encoding:NSUTF8StringEncoding];

    dispatch_async(dispatch_get_main_queue(), ^{
        [mVC.resultTextView setText:nsMsg];
        [mVC.resultTextView setBackgroundColor:[UIColor systemRedColor]];
    });
}

void UseCase_Calypso1_ExplicitSelectionAid_IOS::reset()
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [mVC.resultTextView setText:@""];
        [mVC.resultTextView setBackgroundColor:[UIColor systemGray5Color]];
    });
}
