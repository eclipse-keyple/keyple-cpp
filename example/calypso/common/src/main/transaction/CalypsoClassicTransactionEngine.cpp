/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#include "AppendRecordRespPars.h"
#include "Byte.h"
#include "ByteArrayUtil.h"
#include "CalypsoClassicTransactionEngine.h"
#include "CalypsoUtilities.h"
#include "CalypsoPo.h"
#include "CalypsoClassicInfo.h"
#include "ChannelState.h"
#include "KeypleReaderException.h"
#include "Logger.h"
#include "LoggerFactory.h"
#include "MatchingSelection.h"
#include "ObservableReader_Import.h"
#include "PoResource.h"
#include "PoSelectionRequest.h"
#include "PoSelector.h"
#include "ReadDataStructure.h"
#include "ReadRecordsRespPars.h"
#include "SeCommonProtocols_Import.h"
#include "SeReader.h"
#include "SeSelector_Import.h"
#include "TransmissionMode.h"

namespace keyple {
namespace example {
namespace calypso {
namespace common {
namespace transaction {

using namespace keyple::calypso::command::po::parser;
using namespace keyple::calypso::transaction;
using namespace keyple::example::calypso::common::postructure;
using namespace keyple::example::generic::common;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;
using namespace keyple::common;

std::shared_ptr<Logger> CalypsoClassicTransactionEngine::logger = LoggerFactory::getLogger(typeid(CalypsoClassicTransactionEngine));
const std::shared_ptr<SecuritySettings> CalypsoClassicTransactionEngine::securitySettings;

CalypsoClassicTransactionEngine::CalypsoClassicTransactionEngine() : AbstractReaderObserverEngine()
{
    this->samChannelOpen = false;
}

void CalypsoClassicTransactionEngine::setReaders(std::shared_ptr<SeReader> poReader, std::shared_ptr<SeReader> samReader)
{
    this->poReader = poReader;
    this->samReader = samReader;
}

void CalypsoClassicTransactionEngine::doCalypsoReadWriteTransaction(std::shared_ptr<PoTransaction> poTransaction, bool closeSeChannel)
{

    bool poProcessStatus;

    (void)closeSeChannel;
    (void)poProcessStatus;

    /*
     * Read commands to execute during the opening step: EventLog, ContractList
     */

    /* prepare Event Log read record */
    logger->debug("doCalypsoReadWriteTransaction - preparing event log read record\n");
    int readEventLogParserIndex = poTransaction->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_EventLog,
                                                                       ReadDataStructure::SINGLE_RECORD_DATA,
                                                                       CalypsoClassicInfo::RECORD_NUMBER_1,
                                                                       StringHelper::formatSimple("EventLog (SFI=%02X, recnbr=%d))",
                                                                                                  CalypsoClassicInfo::SFI_EventLog,
                                                                                                  CalypsoClassicInfo::RECORD_NUMBER_1));

    /* prepare Contract List read record */
    logger->debug("doCalypsoReadWriteTransaction - preparing contract list read record\n");
    int readContractListParserIndex = poTransaction->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_ContractList,
                                                                           ReadDataStructure::SINGLE_RECORD_DATA,
                                                                           CalypsoClassicInfo::RECORD_NUMBER_1,
                                                                           StringHelper::formatSimple("ContractList (SFI=%02X))",
                                                                                                      CalypsoClassicInfo::SFI_ContractList));

    logger->info("========= PO Calypso session ======= Opening ============================\n");

    /*
     * Open Session for the debit key - with reading of the first record of the cyclic EF of
     * Environment and Holder file
     */
    poProcessStatus = poTransaction->processOpening(PoTransaction::ModificationMode::ATOMIC,
                                                    PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT,
                                                    CalypsoClassicInfo::SFI_EnvironmentAndHolder,
                                                    CalypsoClassicInfo::RECORD_NUMBER_1);

    logger->info("Parsing Read EventLog file: %s\n",
                 poTransaction->getResponseParser(readEventLogParserIndex)->getStatusInformation());

    logger->info("Parsing Read ContractList file: %s\n",
                 poTransaction->getResponseParser(readContractListParserIndex)->getStatusInformation());

    if (!poTransaction->wasRatified()) {
        logger->info("========= Previous Secure Session was not ratified. =====================\n");

        /*
         * [------------------------------------]
         *
         * The previous Secure Session has not been ratified, so we simply close the Secure
         * Session.
         *
         * We would analyze here the event log read during the opening phase.
         *
         * [------------------------------------]
         */

        logger->info("========= PO Calypso session ======= Closing ============================\n");

        /*
         * A ratification command will be sent (CONTACTLESS_MODE).
         */
        poProcessStatus = poTransaction->processClosing(ChannelState::KEEP_OPEN);

    } else {
        /*
         * [------------------------------------]
         *
         * Place to analyze the PO profile available in seResponse: Environment/Holder,
         * EventLog, ContractList.
         *
         * The information available allows the determination of the contract to be read.
         *
         * [------------------------------------]
         */
        logger->info("========= PO Calypso session ======= Processing of PO commands =======================\n");

        /* Read contract command (we assume we have determine Contract #1 to be read. */
        /* prepare Contract #1 read record */
        int readContractsParserIndex = poTransaction->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_Contracts,
                                                                            ReadDataStructure::MULTIPLE_RECORD_DATA,
                                                                            CalypsoClassicInfo::RECORD_NUMBER_1,
                                                                            StringHelper::formatSimple("Contracts (SFI=%02X, recnbr=%d)",
                                                                                                       CalypsoClassicInfo::SFI_Contracts,
                                                                                                       CalypsoClassicInfo::RECORD_NUMBER_1));

        /* proceed with the sending of commands, don't close the channel */
        poProcessStatus = poTransaction->processPoCommandsInSession();

        logger->info("Parsing Read Contract file: %s",
                     poTransaction->getResponseParser(readContractsParserIndex)->getStatusInformation());

        logger->info("========= PO Calypso session ======= Closing ============================\n");

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
        std::vector<char> eventlog = ByteArrayUtil::fromHex(CalypsoClassicInfo::eventLog_dataFill);
        int appendEventLogParserIndex = poTransaction->prepareAppendRecordCmd(CalypsoClassicInfo::SFI_EventLog,
                                                                              eventlog,
                                                                              StringHelper::formatSimple("EventLog (SFI=%02X)",
                                                                                                         CalypsoClassicInfo::SFI_EventLog));

        /*
         * A ratification command will be sent (CONTACTLESS_MODE).
         */
        poProcessStatus = poTransaction->processClosing(ChannelState::KEEP_OPEN);

        logger->info("Parsing Append EventLog file: %s\n",
                     poTransaction->getResponseParser(appendEventLogParserIndex)->getStatusInformation());
    }

    if (poTransaction->isSuccessful()) {
        logger->info("========= PO Calypso session ======= SUCCESS !!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    } else {
        logger->error("========= PO Calypso session ======= ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    }
}

std::shared_ptr<AbstractDefaultSelectionsRequest> CalypsoClassicTransactionEngine::preparePoSelection()
{
    /*
     * Initialize the selection process for the poReader
     */
    seSelection = std::make_shared<SeSelection>();

    /* operate multiple PO selections */
    std::string poFakeAid1 = "AABBCCDDEE"; // fake AID 1
    std::string poFakeAid2 = "EEDDCCBBAA"; // fake AID 2

    /*
     * Add selection case 1: Fake AID1, protocol ISO, target rev 3
     */
    logger->debug("preparePoSelection - case 1 prepareSelection\n");
    seSelection->prepareSelection(
        std::make_shared<PoSelectionRequest>(
            std::make_shared<PoSelector>(SeCommonProtocols::PROTOCOL_ISO14443_4, nullptr,
                                         std::make_shared<PoSelector::PoAidSelector>(
                                             std::make_shared<SeSelector::AidSelector::IsoAid>(poFakeAid1),
                                             PoSelector::InvalidatedPo::REJECT),
                                         "Selector with fake AID1"),
                                             ChannelState::KEEP_OPEN));

    /*
     * Add selection case 2: Calypso application, protocol ISO, target rev 2 or 3
     *
     * addition of read commands to execute following the selection
     */
    logger->debug("preparePoSelection - case 2 poSelectionRequestCalypsoAid\n");
    std::shared_ptr<PoSelectionRequest> poSelectionRequestCalypsoAid =
        std::make_shared<PoSelectionRequest>(
            std::make_shared<PoSelector>(SeCommonProtocols::PROTOCOL_ISO14443_4, nullptr,
                                         std::make_shared<PoSelector::PoAidSelector>(
                                             std::make_shared<SeSelector::AidSelector::IsoAid>(CalypsoClassicInfo::AID),
                                             PoSelector::InvalidatedPo::ACCEPT),
                                         "Calypso selector"),
                                             ChannelState::KEEP_OPEN);

    logger->debug("preparePoSelection - case 2 prepareReadRecordsCmd\n");
    poSelectionRequestCalypsoAid->prepareReadRecordsCmd(CalypsoClassicInfo::SFI_EventLog, ReadDataStructure::SINGLE_RECORD_DATA,
                                                        CalypsoClassicInfo::RECORD_NUMBER_1, "EventLog (selection step)");

    logger->debug("preparePoSelection - case 2 prepareSelection\n");
    seSelection->prepareSelection(poSelectionRequestCalypsoAid);

    /*
     * Add selection case 3: Fake AID2, unspecified protocol, target rev 2 or 3
     */
    logger->debug("preparePoSelection - case 3 prepareSelection\n");
    seSelection->prepareSelection(
        std::make_shared<PoSelectionRequest>(
            std::make_shared<PoSelector>(SeCommonProtocols::PROTOCOL_B_PRIME, nullptr,
                                         std::make_shared<PoSelector::PoAidSelector>(
                                             std::make_shared<SeSelector::AidSelector::IsoAid>(poFakeAid2),
                                             PoSelector::InvalidatedPo::REJECT),
                                         "Selector with fake AID2"),
                                             ChannelState::KEEP_OPEN));

    /*
     * Add selection case 4: ATR selection, rev 1 atrregex
     */
    logger->debug("preparePoSelection - case 4 prepareSelection\n");
    seSelection->prepareSelection(
        std::make_shared<PoSelectionRequest>(
            std::make_shared<PoSelector>(SeCommonProtocols::PROTOCOL_B_PRIME,
                                         std::make_shared<PoSelector::PoAtrFilter>(CalypsoClassicInfo::ATR_REV1_REGEX),
                                         nullptr, "Selector with fake AID2"),
                                             ChannelState::KEEP_OPEN));

    return seSelection->getSelectionOperation();
}

void CalypsoClassicTransactionEngine::processSeMatch(std::shared_ptr<AbstractDefaultSelectionsResponse> defaultSelectionsResponse)
{
    logger->debug("processSeMatch\n");

    std::shared_ptr<CalypsoPo> calypsoPo = std::dynamic_pointer_cast<CalypsoPo>(
         seSelection->processDefaultSelection(defaultSelectionsResponse)->getActiveSelection()->getMatchingSe());

    if (calypsoPo != nullptr) {
        try {
            /* first time: check SAM */
            if (!this->samChannelOpen) {
                /* the following method will throw an exception if the SAM is not available. */
                this->samResource = CalypsoUtilities::checkSamAndOpenChannel(samReader);
                this->samChannelOpen = true;
            }

            //std::shared_ptr<Profiler> profiler = std::make_shared<Profiler>("Entire transaction");

            /* Time measurement */
            //profiler->start("Initial selection");

            //profiler->start("Calypso1");

            std::shared_ptr<PoTransaction> poTransaction = std::make_shared<PoTransaction>(
                                                               std::make_shared<PoResource>(poReader, calypsoPo),
                                                               samResource, securitySettings);

            doCalypsoReadWriteTransaction(poTransaction, true);

            //profiler->stop();
            //logger->warn(System::getProperty("line.separator") + "{}", profiler);
        }
        catch (const Exception &e) {
            e.printStackTrace();
        }
    }
}

void CalypsoClassicTransactionEngine::processSeInsertion()
{
    logger->trace("processSeInsertion - Unexpected SE insertion event\n");
}

void CalypsoClassicTransactionEngine::processSeRemoval()
{
    logger->trace("processSeRemoval - SE removal event\n");
}

void CalypsoClassicTransactionEngine::processUnexpectedSeRemoval()
{
    logger->trace("processUnexpectedSeRemoval - Unexpected SE removal event\n");
}
}
}
}
}
}
