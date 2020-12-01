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

#include "CalypsoClassicTransactionEngine.h"

/* Common */
#include "Byte.h"
#include "LoggerFactory.h"

/* Core */
#include "ByteArrayUtil.h"
#include "ChannelControl.h"
#include "KeypleReaderException.h"
#include "ObservableReader.h"
#include "PoSelectionRequest.h"
#include "PoSelector.h"
#include "SeReader.h"
#include "SeSelector.h"
#include "SeCommonProtocols.h"
#include "TransmissionMode.h"

/* Calypso */
#include "AppendRecordRespPars.h"
#include "CalypsoPoCommandException.h"
#include "CalypsoPoIOException.h"
#include "CalypsoPo.h"
#include "ReadRecordsRespPars.h"

/* Examples */
#include "CalypsoUtilities.h"
#include "CalypsoClassicInfo.h"

namespace keyple {
namespace example {
namespace calypso {
namespace common {
namespace transaction {

using namespace keyple::calypso::command::po::exception;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::calypso::transaction;
using namespace keyple::calypso::transaction::exception;
using namespace keyple::example::calypso::common::postructure;
using namespace keyple::example::generic::common;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;
using namespace keyple::common;

using AccessLevel = PoTransaction::SessionSetting::AccessLevel;
using AidSelector = SeSelector::AidSelector;
using InvalidatedPo = PoSelector::InvalidatedPo;
using AtrFilter = PoSelector::AtrFilter;

CalypsoClassicTransactionEngine::CalypsoClassicTransactionEngine()
: AbstractReaderObserverEngine(), mSamChannelOpen(false) {}

void CalypsoClassicTransactionEngine::setReaders(
    std::shared_ptr<SeReader> poReader, std::shared_ptr<SeReader> samReader)
{
    mPoReader  = poReader;
    mSamReader = samReader;
}

void CalypsoClassicTransactionEngine::doCalypsoReadWriteTransaction(
    std::shared_ptr<CalypsoPo> calypsoPo,
    std::shared_ptr<PoTransaction> poTransaction,
    const bool closeSeChannel)
{
    (void)closeSeChannel;

    /*
     * Read commands to execute during the opening step: EventLog, ContractList
     */

    /* prepare Environment and Holder read record */
    poTransaction->prepareReadRecordFile(CalypsoClassicInfo::SFI_EnvironmentAndHolder,
                                         CalypsoClassicInfo::RECORD_NUMBER_1);

    /* Prepare Event Log read record */
    poTransaction->prepareReadRecordFile(CalypsoClassicInfo::SFI_EventLog,
                                         CalypsoClassicInfo::RECORD_NUMBER_1);

    /* Prepare Contract List read record */
    poTransaction->prepareReadRecordFile(CalypsoClassicInfo::SFI_ContractList,
                                         CalypsoClassicInfo::RECORD_NUMBER_1);

    mLogger->info("========= PO Calypso session ======= Opening ============================\n");

    /*
     * Open Session for the debit key - with reading of the first record of the
     * cyclic EF of Environment and Holder file
     */
    poTransaction->processOpening(AccessLevel::SESSION_LVL_DEBIT);

    std::shared_ptr<ElementaryFile> efEventLog =
        calypsoPo->getFileBySfi(CalypsoClassicInfo::SFI_EventLog);

    const std::vector<uint8_t> eventLog = efEventLog->getData()->getContent();
    mLogger->info("EventLog file: %\n", ByteArrayUtil::toHex(eventLog));

    std::shared_ptr<ElementaryFile> efContractList =
        calypsoPo->getFileBySfi(CalypsoClassicInfo::SFI_ContractList);

    const std::vector<uint8_t> contractList = efContractList->getData()->getContent(1);
    mLogger->info("ContractList file: %\n", ByteArrayUtil::toHex(contractList));

    if (!calypsoPo->isDfRatified()) {
        mLogger->info("========= Previous Secure Session was not ratified =====================\n");

        /*
         * [------------------------------------]
         *
         * The previous Secure Session has not been ratified, so we simply close
         * the Secure Session.
         *
         * We would analyze here the event log read during the opening phase.
         *
         * [------------------------------------]
         */

        mLogger->info("========= PO Calypso session ======= Closing ============================" \
                      "\n");

        /*
         * A ratification command will be sent (CONTACTLESS_MODE).
         */
        poTransaction->prepareReleasePoChannel();
        poTransaction->processClosing();

    } else {
        /*
         * [------------------------------------]
         *
         * Place to analyze the PO profile available in seResponse:
         * Environment/Holder, EventLog, ContractList.
         *
         * The information available allows the determination of the contract to
         * be read.
         *
         * [------------------------------------]
         */
        mLogger->info("========= PO Calypso session " \
                      "======= Processing of PO commands =======================\n");

        /*
         * Read contract command (we assume we have determine Contract #1 to be
         * read.
         */

        /* Read all 4 contracts command, record size set to 29 */
        poTransaction->prepareReadRecordFile(CalypsoClassicInfo::SFI_Contracts,
                                             CalypsoClassicInfo::RECORD_NUMBER_1,
                                             4,
                                             29);

        /* Proceed with the sending of commands, don't close the channel */
        poTransaction->processPoCommands();

        std::shared_ptr<ElementaryFile> efContracts =
            calypsoPo->getFileBySfi(CalypsoClassicInfo::SFI_Contracts);

        const std::map<int, std::vector<uint8_t>>& records =
            efContracts->getData()->getAllRecordsContent();

        for (const auto& entry_it : records) {
            mLogger->info("Contract #%: %\n",
                          entry_it.first,
                          ByteArrayUtil::toHex(entry_it.second));
        }

        mLogger->info("========= PO Calypso session " \
                      "======= Closing ============================\n");

        /*
         * [------------------------------------]
         *
         * Place to analyze the Contract (in seResponse).
         *
         * The content of the contract will grant or not access.
         *
         * In any case, a new record will be added to the EventLog.
         *
         * [------------------------------------]
         */

        /* prepare Event Log append record */
        poTransaction->prepareAppendRecord(
            CalypsoClassicInfo::SFI_EventLog,
            ByteArrayUtil::fromHex(CalypsoClassicInfo::eventLog_dataFill));

        /*
         * A ratification command will be sent (CONTACTLESS_MODE).
         */
        poTransaction->prepareReleasePoChannel();
        poTransaction->processClosing();
    }

    mLogger->info("========= PO Calypso session ======= SUCCESS !!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
}

std::shared_ptr<AbstractDefaultSelectionsRequest>
    CalypsoClassicTransactionEngine::preparePoSelection()
{
    /*
     * Initialize the selection process for the poReader
     */
    mSeSelection = std::make_shared<SeSelection>();

    /* operate multiple PO selections */
    std::string poFakeAid1 = "AABBCCDDEE"; // fake AID 1
    std::string poFakeAid2 = "EEDDCCBBAA"; // fake AID 2

    /*
     * Add selection case 1: Fake AID1, protocol ISO, target rev 3
     */
    auto aidSelector = AidSelector::builder()->aidToSelect(poFakeAid1).build();
    auto seSelector = PoSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                            .aidSelector(aidSelector)
                                            .invalidatedPo(InvalidatedPo::REJECT)
                                            .build();
    auto poSelector = std::dynamic_pointer_cast<PoSelector>(seSelector);
    auto request = std::make_shared<PoSelectionRequest>(poSelector);
    auto abstract = std::reinterpret_pointer_cast
                        <AbstractSeSelectionRequest<AbstractApduCommandBuilder>>(request);
    mSeSelection->prepareSelection(abstract);

    /*
     * Add selection case 2: Calypso application, protocol ISO, target rev 2 or 3
     *
     * addition of read commands to execute following the selection
     */
    aidSelector = AidSelector::builder()->aidToSelect(CalypsoClassicInfo::AID).build();
    seSelector = PoSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_ISO14443_4)
                                       .aidSelector(aidSelector)
                                       .invalidatedPo(InvalidatedPo::ACCEPT)
                                       .build();
    poSelector = std::dynamic_pointer_cast<PoSelector>(seSelector);
    auto poSelectionRequestCalypsoAid = std::make_shared<PoSelectionRequest>(poSelector);

    poSelectionRequestCalypsoAid->prepareSelectFile(CalypsoClassicInfo::LID_DF_RT);
    poSelectionRequestCalypsoAid->prepareSelectFile(CalypsoClassicInfo::LID_EventLog);
    poSelectionRequestCalypsoAid->prepareReadRecordFile(CalypsoClassicInfo::SFI_EventLog,
                                                        CalypsoClassicInfo::RECORD_NUMBER_1);
    abstract = std::reinterpret_pointer_cast
                        <AbstractSeSelectionRequest<AbstractApduCommandBuilder>>(
                            poSelectionRequestCalypsoAid);
    mSeSelection->prepareSelection(abstract);

    /* Add selection case 3: Fake AID2, unspecified protocol, target rev 2 or 3 */
    aidSelector = AidSelector::builder()->aidToSelect(poFakeAid2).build();
    seSelector = PoSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_B_PRIME)
                                       .aidSelector(aidSelector)
                                       .build();
    poSelector = std::dynamic_pointer_cast<PoSelector>(seSelector);
    request = std::make_shared<PoSelectionRequest>(poSelector);
    abstract = std::reinterpret_pointer_cast
                        <AbstractSeSelectionRequest<AbstractApduCommandBuilder>>(request);
    mSeSelection->prepareSelection(abstract);

    /*
     * Add selection case 4: ATR selection, rev 1 atrregex
     */
    auto atrFilter = std::make_shared<AtrFilter>(CalypsoClassicInfo::ATR_REV1_REGEX);
    seSelector = PoSelector::builder()->seProtocol(SeCommonProtocols::PROTOCOL_B_PRIME)
                                       .atrFilter(atrFilter)
                                       .invalidatedPo(InvalidatedPo::REJECT)
                                       .build();
    poSelector = std::dynamic_pointer_cast<PoSelector>(seSelector);
    request = std::make_shared<PoSelectionRequest>(poSelector);
    abstract = std::reinterpret_pointer_cast
                        <AbstractSeSelectionRequest<AbstractApduCommandBuilder>>(request);
    mSeSelection->prepareSelection(abstract);

    return mSeSelection->getSelectionOperation();
}

void CalypsoClassicTransactionEngine::processSeMatch(
    const std::shared_ptr<AbstractDefaultSelectionsResponse> defaultSelectionsResponse)
{
    mLogger->debug("processSeMatch\n");

    const std::shared_ptr<AbstractMatchingSe> matchingSe =
        mSeSelection->processDefaultSelection(defaultSelectionsResponse)->getActiveMatchingSe();
    auto calypsoPo = std::dynamic_pointer_cast<CalypsoPo>(matchingSe);


    if (calypsoPo != nullptr) {
        mLogger->info("DF RT header: %\n", calypsoPo->getDirectoryHeader());

        std::shared_ptr<ElementaryFile> eventLogEF =
            calypsoPo->getFileBySfi(CalypsoClassicInfo::SFI_EventLog);

        mLogger->info("Event Log header: %\n", eventLogEF->getHeader());

        const std::vector<uint8_t> eventLogBytes =
            eventLogEF->getData()->getContent(CalypsoClassicInfo::RECORD_NUMBER_1);

        const std::string eventLog = ByteArrayUtil::toHex(eventLogBytes);

        mLogger->info("EventLog: %\n", eventLog);

        try {
            /* First time: check SAM */
            if (!mSamChannelOpen) {
                /* The following method will throw an exception if the SAM is not available */
                mSamResource = CalypsoUtilities::checkSamAndOpenChannel(mSamReader);
                mSamChannelOpen = true;
            }

            /* Define the SAM parameters to provide when creating PoTransaction */
            std::shared_ptr<PoSecuritySettings> poSecuritySettings =
                CalypsoUtilities::getSecuritySettings(mSamResource);

            auto seResource = std::make_shared<SeResource<CalypsoPo>>(mPoReader, calypsoPo);
            auto poTransaction = std::make_shared<PoTransaction>(seResource, poSecuritySettings);

            doCalypsoReadWriteTransaction(calypsoPo, poTransaction, true);

        } catch (const CalypsoPoCommandException& e) {
            mLogger->error("PO command % failed with the status code %. %\n",
                           e.getCommand(),
                           e.getStatusCode() & 0xFFFF,
                           e.getMessage());
        }
    }
}

void CalypsoClassicTransactionEngine::processSeInserted()
{
    mLogger->trace("processSeInserted - Unexpected SE insertion event\n");
}

void CalypsoClassicTransactionEngine::processSeRemoval()
{
    mLogger->trace("processSeRemoval - SE removal event\n");
}

void CalypsoClassicTransactionEngine::processUnexpectedSeRemoval()
{
    mLogger->trace("processUnexpectedSeRemoval - Unexpected SE removal event\n");
}
}
}
}
}
}
