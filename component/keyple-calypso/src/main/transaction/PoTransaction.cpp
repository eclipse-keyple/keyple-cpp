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

#include "PoTransaction.h"

#include <atomic>

/* Calypso */
#include "AbstractOpenSessionCmdBuild.h"
#include "AbstractOpenSessionRespPars.h"
#include "AppendRecordCmdBuild.h"
#include "AppendRecordRespPars.h"
#include "CalypsoAtomicTransactionException.h"
#include "CalypsoAuthenticationNotVerifiedException.h"
#include "CalypsoDesynchronizedExchangesException.h"
#include "CalypsoPo.h"
#include "CalypsoPoCommandException.h"
#include "CalypsoPoIOException.h"
#include "CalypsoPoCloseSecureSessionException.h"
#include "CalypsoPoSecurityDataException.h"
#include "CalypsoPoTransactionIllegalStateException.h"
#include "CalypsoPoUtils.h"
#include "CalypsoSamCommandException.h"
#include "CalypsoSamIOException.h"
#include "CalypsoSessionAuthenticationException.h"
#include "CalypsoUnauthorizedKvcException.h"
#include "CloseSessionCmdBuild.h"
#include "CloseSessionRespPars.h"
#include "DecreaseCmdBuild.h"
#include "DigestAuthenticateCmdBuild.h"
#include "DigestAuthenticateRespPars.h"
#include "DigestCloseCmdBuild.h"
#include "DigestCloseRespPars.h"
#include "DigestInitCmdBuild.h"
#include "DigestUpdateCmdBuild.h"
#include "IncreaseCmdBuild.h"
#include "InvalidateCmdBuild.h"
#include "KeypleAssert.h"
#include "KeypleReaderException.h"
#include "KeypleReaderIOException.h"
#include "KeypleStd.h"
#include "NoSuchElementException.h"
#include "OpenSession10CmdBuild.h"
#include "OpenSession24CmdBuild.h"
#include "OpenSession31CmdBuild.h"
#include "OpenSession32CmdBuild.h"
#include "PoClass.h"
#include "PoCommandManager.h"
#include "PoSecuritySettings.h"
#include "RatificationCmdBuild.h"
#include "ReadRecordsCmdBuild.h"
#include "RehabilitateCmdBuild.h"
#include "SamCommandProcessor.h"
#include "SamGetChallengeCmdBuild.h"
#include "SamGetChallengeRespPars.h"
#include "SelectDiversifierCmdBuild.h"
#include "SvDebitCmdBuild.h"
#include "UpdateRecordCmdBuild.h"
#include "UpdateRecordRespPars.h"
#include "WriteRecordCmdBuild.h"
#include "WriteRecordRespPars.h"

/* Core */
#include "AbstractApduCommandBuilder.h"
#include "AbstractApduResponseParser.h"
#include "ApduResponse.h"
#include "ByteArrayUtil.h"
#include "ProxyReader.h"
#include "SeReader.h"

/* Common */
#include "Arrays.h"
#include "Cast.h"
#include "KeypleStd.h"
#include "stringhelper.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;
using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::calypso::command::po::builder::security;
using namespace keyple::calypso::command::po::builder::storedvalue;
using namespace keyple::calypso::command::po::exception;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::calypso::command::po::parser::security;
using namespace keyple::calypso::command::po::parser::storedvalue;
using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::builder;
using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::calypso::command::sam::exception;
using namespace keyple::calypso::command::sam::parser;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::calypso::transaction;
using namespace keyple::calypso::transaction::exception;

using AccessLevel = PoTransaction::SessionSetting::AccessLevel;
using Action = PoTransaction::SvSettings::Action;
using ReadMode = ReadRecordsCmdBuild::ReadMode;
using SessionState = PoTransaction::SessionState;
using AbstractPoCommandResponse = AbstractPoCommandBuilder<AbstractPoResponseParser>;

/* PO TRANSACTION ------------------------------------------------------------------------------- */

const std::shared_ptr<ApduResponse> PoTransaction::RESPONSE_OK =
    std::make_shared<ApduResponse>(std::vector<uint8_t>{0x90, 0x00}, nullptr);
const std::shared_ptr<ApduResponse> PoTransaction::RESPONSE_OK_POSTPONED =
    std::make_shared<ApduResponse>(std::vector<uint8_t>{0x62, 0x00}, nullptr);

PoTransaction::PoTransaction(std::shared_ptr<SeResource<CalypsoPo>> poResource,
                             std::shared_ptr<PoSecuritySettings> poSecuritySettings)
: mPoReader(std::dynamic_pointer_cast<ProxyReader>(poResource->getSeReader())),
  mPoSecuritySettings(poSecuritySettings),
  mCalypsoPo(poResource->getMatchingSe()),
  mSessionState(SessionState::SESSION_UNINITIALIZED),
  mCurrentAccessLevel(AccessLevel::SESSION_LVL_NONE),
  mModificationsCounter(mCalypsoPo->getModificationsCounter()),
  mPoCommandManager(std::make_shared<PoCommandManager>()),
  mChannelControl(ChannelControl::KEEP_OPEN)
{
    if (poSecuritySettings != nullptr)
        mSamCommandProcessor = std::make_shared<SamCommandProcessor>(poResource,poSecuritySettings);
}

PoTransaction::PoTransaction(std::shared_ptr<SeResource<CalypsoPo>> poResource)
: PoTransaction(poResource, nullptr) {}

void PoTransaction::processAtomicOpening(
    const AccessLevel& accessLevel,
    std::vector<std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>>& poCommands)
{
    /* This method should be called only if no session was previously open */
    checkSessionIsNotOpen();

    if (mPoSecuritySettings == nullptr)
        throw CalypsoPoTransactionIllegalStateException("No SAM resource is available");

    /* Gets the terminal challenge */
    const std::vector<uint8_t>& sessionTerminalChallenge =
        mSamCommandProcessor->getSessionTerminalChallenge();

    /* PO ApduRequest List to hold Open Secure Session and other optional commands */
    std::vector<std::shared_ptr<ApduRequest>> poApduRequests;

    /*
     * The sfi and record number to be read when the open secure session command is executed. The
     * default value is 0 (no record to read) but we will optimize the exchanges if a read record
     * command has been prepared.
     */
    int sfi = 0;
    int recordNumber = 0;

    /*
     * Let's check if we have a read record command at the top of the command list.
     *
     * If so, then the command is withdrawn in favour of its equivalent executed at the same time as
     * the open secure session command.
     */
    if (!poCommands.empty()) {
        std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>> poCommand = poCommands[0];
        auto readRecCmdBuild = std::reinterpret_pointer_cast<ReadRecordsCmdBuild>(poCommand);
        auto commandRef = std::dynamic_pointer_cast<CalypsoPoCommand>(poCommand->getCommandRef());

        if (*commandRef.get() == CalypsoPoCommand::READ_RECORDS &&
            readRecCmdBuild->getReadMode() == ReadRecordsCmdBuild::ReadMode::ONE_RECORD) {
            sfi = readRecCmdBuild->getSfi();
            recordNumber = readRecCmdBuild->getFirstRecordNumber();
            poCommands.erase(poCommands.begin());
        }
    }

    /* Build the PO Open Secure Session command */
    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>> openSessionCmdBuild =
        AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
            mCalypsoPo->getRevision(),
            accessLevel.getSessionKey(),
            sessionTerminalChallenge,
            sfi,
            recordNumber);

    /* Add the resulting ApduRequest to the PO ApduRequest list */
    poApduRequests.push_back(openSessionCmdBuild->getApduRequest());

    /* Add all optional commands to the PO ApduRequest list */
    const std::vector<std::shared_ptr<ApduRequest>>& commands = getApduRequests(poCommands);
    for (const auto& command : commands)
        poApduRequests.push_back(command);

    /* Create a SeRequest from the ApduRequest list, PO AID as Selector, keep channel open */
    auto poSeRequest = std::make_shared<SeRequest>(poApduRequests);

    /* Transmit the commands to the PO */
    std::shared_ptr<SeResponse> poSeResponse = safePoTransmit(poSeRequest,
                                                              ChannelControl::KEEP_OPEN);


    /* Retrieve and check the ApduResponses */
    std::vector<std::shared_ptr<ApduResponse>>& poApduResponses = poSeResponse->getApduResponses();

    /* Do some basic checks */
    checkCommandsResponsesSynchronization(poApduRequests.size(), poApduResponses.size());

    /*
     * Parse the response to Open Secure Session (the first item of poApduResponses). The
     * updateCalypsoPo method fills the CalypsoPo object with the command data and return the parser
     * used for an internal usage here.
     */
    auto poOpenSessionPars =
        std::dynamic_pointer_cast<AbstractOpenSessionRespPars>(
            CalypsoPoUtils::updateCalypsoPo(
                mCalypsoPo,
                std::reinterpret_pointer_cast<AbstractPoCommandBuilder<AbstractPoResponseParser>>(
                    openSessionCmdBuild),
                poApduResponses[0]));

    /*
     * Build the Digest Init command from PO Open Session the session challenge is needed for the
     * SAM digest computation
     */
    const std::vector<uint8_t>& sessionCardChallenge = poOpenSessionPars->getPoChallenge();

    /* The PO KIF */
    const uint8_t poKif = poOpenSessionPars->getSelectedKif();

    /* The PO KVC, may be null for PO Rev 1.0 */
    const uint8_t poKvc = poOpenSessionPars->getSelectedKvc();

    logger->debug("processAtomicOpening => opening: CARDCHALLENGE = %, POKIF = %, POKVC = %\n",
                  ByteArrayUtil::toHex(sessionCardChallenge),
                  StringHelper::uint8ToHexString(poKif),
                  StringHelper::uint8ToHexString(poKvc));

    if (!poKvc || !mPoSecuritySettings->isSessionKvcAuthorized(poKvc))
        throw CalypsoUnauthorizedKvcException("PO KVC = " + StringHelper::uint8ToHexString(poKvc));

    /*
     * Initialize the digest processor. It will store all digest operations
     * (Digest Init, Digest Update) until the session closing. At this moment,
     * all SAM Apdu will be processed at once.
     */
    mSamCommandProcessor->initializeDigester(accessLevel,
                                             false,
                                             false,
                                             poKif,
                                             poKvc,
                                             poApduResponses[0]->getDataOut());

    /*
     * Add all commands data to the digest computation. The first command in the
     * list is the open secure session command. This command is not included in
     * the digest computation, so we skip it and start the loop at index 1.
     */
    if (!poCommands.empty())
        /* Add requests and responses to the digest processor */
        mSamCommandProcessor->pushPoExchangeDataList(poApduRequests,
                                                     poApduResponses,
                                                     1);

    /* Remove Open Secure Session response and create a new SeResponse */
    poApduResponses.erase(poApduResponses.begin());

    /* Update CalypsoPo with the received data */
    CalypsoPoUtils::updateCalypsoPo(mCalypsoPo, poCommands, poApduResponses);

    mSessionState = SessionState::SESSION_OPEN;
}

void PoTransaction::processAtomicOpening(const AccessLevel& accessLevel)
{
    std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>> empty;

        processAtomicOpening(accessLevel, empty);
}

std::vector<std::shared_ptr<ApduRequest>> PoTransaction::getApduRequests(
    const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>>& poCommands) const
{
    std::vector<std::shared_ptr<ApduRequest>> apduRequests;

    if (poCommands.size()) {
        for (const auto& commandBuilder : poCommands)
            apduRequests.push_back(commandBuilder->getApduRequest());
    }

    return apduRequests;
}

void PoTransaction::processAtomicPoCommands(
    const std::vector<std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>>&
        poCommands,
    const ChannelControl channelControl)
{
    /* Get the PO ApduRequest List */
    const std::vector<std::shared_ptr<ApduRequest>> poApduRequests = getApduRequests(poCommands);

    /*
     * Create a SeRequest from the ApduRequest list, PO AID as Selector, manage the logical channel
     * according to the channelControl enum
     */
    auto  poSeRequest = std::make_shared<SeRequest>(poApduRequests);

    /* Transmit the commands to the PO */
    std::shared_ptr<SeResponse> poSeResponse = safePoTransmit(poSeRequest, channelControl);

    /* Retrieve and check the ApduResponses */
    const std::vector<std::shared_ptr<ApduResponse>>& poApduResponses =
        poSeResponse->getApduResponses();

    /* Do some basic checks */
    checkCommandsResponsesSynchronization(poApduRequests.size(), poApduResponses.size());

    /*
     * Add all commands data to the digest computation if this method is called
     * within a Secure Session.
     */
    if (mSessionState == SessionState::SESSION_OPEN)
        mSamCommandProcessor->pushPoExchangeDataList(poApduRequests, poApduResponses, 0);

    CalypsoPoUtils::updateCalypsoPo(mCalypsoPo, poCommands, poSeResponse->getApduResponses());
}

void PoTransaction::processAtomicClosing(
    const std::vector<std::shared_ptr<AbstractPoCommandBuilder< AbstractPoResponseParser>>>&
        poModificationCommands,
    const std::vector<std::shared_ptr<ApduResponse>>& poAnticipatedResponses,
    const SessionSetting::RatificationMode ratificationMode,
    const ChannelControl channelControl)
{
    checkSessionIsOpen();

    /* Get the PO ApduRequest List - for the first PO exchange */
    std::vector<std::shared_ptr<ApduRequest>> poApduRequests =
        getApduRequests(poModificationCommands);

    /*
     * Compute "anticipated" Digest Update (for optional poModificationCommands)
     */
    if (!poModificationCommands.empty() && !poApduRequests.empty()) {
        checkCommandsResponsesSynchronization(poApduRequests.size(), poAnticipatedResponses.size());

        /* Add all commands data to the digest computation: commands and anticipated responses */
        mSamCommandProcessor->pushPoExchangeDataList(poApduRequests, poAnticipatedResponses, 0);
    }

    /*
     * All SAM digest operations will now run at once.
     * Get Terminal Signature from the latest response
     */
    const std::vector<uint8_t>& sessionTerminalSignature =
        mSamCommandProcessor->getTerminalSignature();

    bool ratificationCommandResponseReceived;

    /* Build the PO Close Session command. The last one for this session */
    auto closeSessionCmdBuild = std::make_shared<CloseSessionCmdBuild>(
                                    mCalypsoPo->getPoClass(),
                                    RatificationMode::CLOSE_RATIFIED == ratificationMode,
                                    sessionTerminalSignature);

    poApduRequests.push_back(closeSessionCmdBuild->getApduRequest());

    /* Keep the position of the Close Session command in request list */
    const int closeCommandIndex = poApduRequests.size() - 1;

    /* Add the PO Ratification command if any */
    bool ratificationCommandAdded;

    if (ratificationMode == RatificationMode::CLOSE_RATIFIED &&
        mCalypsoPo->getTransmissionMode() == TransmissionMode::CONTACTLESS) {
        poApduRequests.push_back(RatificationCmdBuild::getApduRequest(mCalypsoPo->getPoClass()));
        ratificationCommandAdded = true;
    } else {
        ratificationCommandAdded = false;
    }

    /* Transfer PO commands */
    auto poSeRequest = std::make_shared<SeRequest>(poApduRequests);
    std::shared_ptr<SeResponse> poSeResponse;

    try {
        poSeResponse = mPoReader->transmitSeRequest(poSeRequest, channelControl);

        /*
         * If the ratification command was added and no error occured then the response has been
         * received
         */
        ratificationCommandResponseReceived = ratificationCommandAdded;
    } catch (const KeypleReaderIOException& e) {
        poSeResponse = e.getSeResponse();

        /*
         * The current exception may have been caused by a communication issue with the PO during
         * the ratification command.
         *
         * In this case, we do not stop the process and consider the Secure Session close. We'll
         * check the signature.
         *
         * We should have one response less than requests.
         */
        if (!ratificationCommandAdded ||
            poSeResponse == nullptr ||
            poSeResponse->getApduResponses().size() != poApduRequests.size() - 1)
            throw CalypsoPoIOException("PO IO Exception while transmitting commands.", e);

        /* We received all responses except the response to the ratification command */
        ratificationCommandResponseReceived = false;
    }

    std::vector<std::shared_ptr<ApduResponse>> poApduResponses = poSeResponse->getApduResponses();

    /*
     * Check the commands executed before closing the secure session (only responses to these
     * commands will be taken into account)
     */
    CalypsoPoUtils::updateCalypsoPo(mCalypsoPo, poModificationCommands, poApduResponses);

    /* Check the PO's response to Close Secure Session */
    std::shared_ptr<CloseSessionRespPars> poCloseSessionPars;

    try {
        auto cmd = std::reinterpret_pointer_cast<AbstractPoCommandResponse>(closeSessionCmdBuild);
        std::shared_ptr<AbstractPoResponseParser> parser =
            CalypsoPoUtils::updateCalypsoPo(mCalypsoPo, cmd, poApduResponses[closeCommandIndex]);
        poCloseSessionPars = std::dynamic_pointer_cast<CloseSessionRespPars>(parser);
    } catch (const CalypsoPoSecurityDataException& ex) {
        throw CalypsoPoCloseSecureSessionException("Invalid PO session", ex);
    }

    /* Check the PO signatures */
    try {
        mSamCommandProcessor->authenticatePoSignature(poCloseSessionPars->getSignatureLo());
    } catch (const CalypsoSamIOException& e) {
        throw CalypsoAuthenticationNotVerifiedException(e.getMessage());
    } catch (const CalypsoSamCommandException& e) {
        throw CalypsoSessionAuthenticationException("PO authentication failed on SAM side.", e);
    }

    /* If necessary, we check the status of the SV after the session has been successfully closed */
    if (mPoCommandManager->isSvOperationCompleteOneTime())
        mSamCommandProcessor->checkSvStatus(poCloseSessionPars->getPostponedData());

    mSessionState = SessionState::SESSION_CLOSED;

    if (ratificationCommandResponseReceived)
        /* Remove the ratification response */
        poApduResponses.pop_back();

    /* Remove Close Secure Session response and create a new SeResponse */
    poApduResponses.pop_back();
}

void PoTransaction::processAtomicClosing(
    const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>>& poCommands,
    const RatificationMode ratificationMode,
    const ChannelControl channelControl)
{
    const std::vector<std::shared_ptr<ApduResponse>>& poAnticipatedResponses =
        getAnticipatedResponses(poCommands);

    processAtomicClosing(poCommands,
                         poAnticipatedResponses,
                         ratificationMode,
                         channelControl);
}

void PoTransaction::processAtomicClosing(
    const RatificationMode ratificationMode,
    const ChannelControl channelControl)
{
    const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>> empty;

    processAtomicClosing(empty,
                         ratificationMode,
                         channelControl);
}

int PoTransaction::getCounterValue(const uint8_t sfi, const int counter) const
{
    try {
        std::shared_ptr<ElementaryFile> ef = mCalypsoPo->getFileBySfi(sfi);
        return ef->getData()->getContentAsCounterValue(counter);
    } catch (const NoSuchElementException& e) {
        throw CalypsoPoTransactionIllegalStateException(
                  "Anticipated response. " \
                  "Unable to determine anticipated value of counter "  +
                  std::to_string(counter) +
                  " in EF sfi " + StringHelper::uint8ToHexString(sfi));
    }
}

std::unique_ptr<ApduResponse> PoTransaction::createIncreaseDecreaseResponse(
    const int newCounterValue) const
{
    /* response = NNNNNN9000 */
    const std::vector<uint8_t> response = {
        static_cast<uint8_t>((newCounterValue & 0x00FF0000) >> 16),
        static_cast<uint8_t>((newCounterValue & 0x0000FF00) >> 8),
        static_cast<uint8_t>((newCounterValue & 0x000000FF)),
        0x90,
        0x00
    };

    return std::unique_ptr<ApduResponse>(new ApduResponse(response, nullptr));
}

std::vector<std::shared_ptr<ApduResponse>> PoTransaction::getAnticipatedResponses(
    const std::vector<std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>>&
        poCommands) const
{
    std::vector<std::shared_ptr<ApduResponse>> apduResponses;

    for (const auto& commandBuilder : poCommands) {
        std::shared_ptr<SeCommand> cmd = commandBuilder->getCommandRef();
        auto calypsoCmd = std::dynamic_pointer_cast<CalypsoPoCommand>(cmd);
        if (*calypsoCmd.get() == CalypsoPoCommand::DECREASE) {
            auto builder = std::reinterpret_pointer_cast<DecreaseCmdBuild>(commandBuilder);
            const int sfi = builder->getSfi();
            const int counter = builder->getCounterNumber();
            const int newCounterValue = getCounterValue(sfi, counter) - builder->getDecValue();
            apduResponses.push_back(createIncreaseDecreaseResponse(newCounterValue));

        } else if (*calypsoCmd.get() == CalypsoPoCommand::INCREASE) {
            auto builder = std::reinterpret_pointer_cast<IncreaseCmdBuild>(commandBuilder);
            const int sfi = builder->getSfi();
            const int counter = builder->getCounterNumber();
            const int newCounterValue = getCounterValue(sfi, counter) + builder->getIncValue();
            apduResponses.push_back(createIncreaseDecreaseResponse(newCounterValue));

         } else if (*calypsoCmd.get() == CalypsoPoCommand::SV_RELOAD ||
                    *calypsoCmd.get() == CalypsoPoCommand::SV_DEBIT  ||
                    *calypsoCmd.get() == CalypsoPoCommand::SV_UNDEBIT) {
            apduResponses.push_back(RESPONSE_OK_POSTPONED);
        } else {
            /* Append/Update/Write Record: response = 9000 */
            apduResponses.push_back(RESPONSE_OK);
        }
    }

    return apduResponses;
}

void PoTransaction::processOpening(const AccessLevel& accessLevel)
{
    mCurrentAccessLevel = accessLevel;

    /* create a sublist of AbstractPoCommandBuilder to be sent atomically */
    std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>> poAtomicCommands;

    std::atomic<int> neededSessionBufferSpace;
    std::atomic<bool> overflow;

    for (const auto& commandBuilder : mPoCommandManager->getPoCommandBuilders())
    {
        /*
         * Check if the command is a modifying one and get it status (overflow
         * yes/no, neededSessionBufferSpace)
         * if the command overflows the session buffer in atomic modification
         * mode, an exception is raised.
         */
        if (checkModifyingCommand(commandBuilder,
                                  overflow,
                                  neededSessionBufferSpace)) {
            if (overflow) {
                /* Open the session with the current commands */
                processAtomicOpening(mCurrentAccessLevel, poAtomicCommands);

                /*
                 * Closes the session, resets the modifications buffer counters
                 * for the next round (set the contact mode to avoid the
                 * transmission of the ratification)
                 */
                processAtomicClosing(RatificationMode::CLOSE_RATIFIED,
                                     ChannelControl::KEEP_OPEN);
                resetModificationsBufferCounter();

                /*
                 * Clear the list and add the command that did not fit in the PO
                 * modifications buffer. We also update the usage counter
                 * without checking the result.
                 */
                poAtomicCommands.clear();
                poAtomicCommands.push_back(commandBuilder);

                /*
                 * Just update modifications buffer usage counter, ignore result
                 * (always false)
                 */
                isSessionBufferOverflowed(neededSessionBufferSpace);
            } else {
                /*
                 * The command fits in the PO modifications buffer, just add it
                 * to the list
                 */
                poAtomicCommands.push_back(commandBuilder);
            }
        } else {
            /* This command does not affect the PO modifications buffer */
            poAtomicCommands.push_back(commandBuilder);
        }
    }

    processAtomicOpening(mCurrentAccessLevel, poAtomicCommands);

    /* Sets the flag indicating that the commands have been executed */
    mPoCommandManager->notifyCommandsProcessed();
}

void PoTransaction::processPoCommands()
{
    if (mSessionState == SessionState::SESSION_OPEN)
        processPoCommandsInSession();
    else
        processPoCommandsOutOfSession(mChannelControl);
}

void PoTransaction::processPoCommandsOutOfSession(const ChannelControl channelControl)
{
    /* PO commands sent outside a Secure Session. No modifications buffer limitation */
    processAtomicPoCommands(mPoCommandManager->getPoCommandBuilders(), channelControl);

    /* Sets the flag indicating that the commands have been executed */
    mPoCommandManager->notifyCommandsProcessed();

    /* If an SV transaction was performed, we check the signature returned by the PO here */
    if (mPoCommandManager->isSvOperationCompleteOneTime())
        mSamCommandProcessor->checkSvStatus(CalypsoPoUtils::getSvOperationSignature());
}

void PoTransaction::processPoCommandsInSession()
{
    /* This method should be called only if a session was previously open */
    checkSessionIsOpen();

    /* A session is open, we have to care about the PO modifications buffer */
    std::vector<std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>>
        poAtomicBuilders;

    std::atomic<int> neededSessionBufferSpace;
    std::atomic<bool> overflow;

    for (const auto& commandBuilder : mPoCommandManager->getPoCommandBuilders()) {
        /*
         * Check if the command is a modifying one and get it status (overflow yes/no,
         * neededSessionBufferSpace) if the command overflows the session buffer in atomic
         * modification mode, an exception is raised.
         */
        if (checkModifyingCommand(commandBuilder, overflow, neededSessionBufferSpace)) {
            if (overflow) {
                /*
                 * The current command would overflow the modifications buffer in the PO. We send
                 * the current commands and update the parsers. The parsers Iterator is kept all
                 * along the process.
                 */
                processAtomicPoCommands(poAtomicBuilders, ChannelControl::KEEP_OPEN);

                /*
                 * Close the session and reset the modifications buffer counters for the next round
                 * (set the contact mode to avoid the transmission of the ratification)
                 */
                processAtomicClosing(RatificationMode::CLOSE_RATIFIED, ChannelControl::KEEP_OPEN);
                resetModificationsBufferCounter();

                /* We reopen a new session for the remaining commands to be sent */
                processAtomicOpening(mCurrentAccessLevel);

                /*
                 * Clear the list and add the command that did not fit in the PO modifications
                 * buffer. We also update the usage counter without checking the result.
                 */
                poAtomicBuilders.clear();
                poAtomicBuilders.push_back(commandBuilder);

                /* Just update modifications buffer usage counter, ignore result (always false) */
                isSessionBufferOverflowed(neededSessionBufferSpace);
            } else {
                /* The command fits in the PO modifications buffer, just add it to the list */
                poAtomicBuilders.push_back(commandBuilder);
            }
        } else {
            /* This command does not affect the PO modifications buffer */
            poAtomicBuilders.push_back(commandBuilder);
        }
    }

    if (!poAtomicBuilders.empty())
        processAtomicPoCommands(poAtomicBuilders, ChannelControl::KEEP_OPEN);

    /* Sets the flag indicating that the commands have been executed */
    mPoCommandManager->notifyCommandsProcessed();
}

void PoTransaction::processClosing()
{
    checkSessionIsOpen();

    bool atLeastOneReadCommand = false;
    bool sessionPreviouslyClosed = false;

    std::atomic<int> neededSessionBufferSpace;
    std::atomic<bool> overflow;

    std::vector<std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>>
        poAtomicCommands;

    for (const auto& commandBuilder : mPoCommandManager->getPoCommandBuilders()) {
        /*
         * Check if the command is a modifying one and get it status (overflow yes/no,
         * neededSessionBufferSpace) if the command overflows the session buffer in atomic
         * modification mode, an exception is raised.
         */
        if (checkModifyingCommand(commandBuilder, overflow, neededSessionBufferSpace)) {
            if (overflow) {
                /*
                 * Reopen a session with the same access level if it was previously closed in this
                 * current processClosing
                 */
                if (sessionPreviouslyClosed)
                    processAtomicOpening(mCurrentAccessLevel);

                /*
                 * If at least one non-modifying was prepared, we use processAtomicPoCommands
                 * instead of processAtomicClosing to send the list
                 */
                if (atLeastOneReadCommand) {
                    processAtomicPoCommands(poAtomicCommands, ChannelControl::KEEP_OPEN);

                    /* Clear the list of commands sent */
                    poAtomicCommands.clear();
                    processAtomicClosing(poAtomicCommands,
                                         RatificationMode::CLOSE_RATIFIED,
                                         ChannelControl::KEEP_OPEN);
                    resetModificationsBufferCounter();
                    sessionPreviouslyClosed = true;
                    atLeastOneReadCommand = false;
                } else {
                    /* All commands in the list are 'modifying the PO' */
                    processAtomicClosing(poAtomicCommands,
                                         RatificationMode::CLOSE_RATIFIED,
                                         ChannelControl::KEEP_OPEN);

                    /* Clear the list of commands sent */
                    poAtomicCommands.clear();
                    resetModificationsBufferCounter();
                    sessionPreviouslyClosed = true;
                }

                /*
                 * Add the command that did not fit in the PO modifications buffer. We also update
                 * the usage counter without checking the result.
                 */
                poAtomicCommands.push_back(commandBuilder);

                /* Just update modifications buffer usage counter, ignore result (always false) */
                isSessionBufferOverflowed(neededSessionBufferSpace);
            } else {
                /* The command fits in the PO modifications buffer, just add it to the list */
                poAtomicCommands.push_back(commandBuilder);
            }
        } else {
            /* This command does not affect the PO modifications buffer */
            poAtomicCommands.push_back(commandBuilder);
            atLeastOneReadCommand = true;
        }
    }

    if (sessionPreviouslyClosed)
        /*
         * Reopen if needed, to close the session with the requested conditions
         * (CommunicationMode and channelControl)
         */
        processAtomicOpening(mCurrentAccessLevel);

    /* Finally, close the session as requested */
    processAtomicClosing(poAtomicCommands,
                         mPoSecuritySettings->getRatificationMode(),
                         mChannelControl);

    /* Sets the flag indicating that the commands have been executed */
    mPoCommandManager->notifyCommandsProcessed();
}

void PoTransaction::processCancel()
{
    /* PO ApduRequest List to hold Close Secure Session command */
    std::vector<std::shared_ptr<ApduRequest>> poApduRequests;

    /* Build the PO Close Session command (in "abort" mode since no signature is provided) */
    CloseSessionCmdBuild closeSessionCmdBuild(mCalypsoPo->getPoClass());

    poApduRequests.push_back(closeSessionCmdBuild.getApduRequest());

    /* Transfer PO commands */
    auto poSeRequest = std::make_shared<SeRequest>(poApduRequests);
    std::shared_ptr<SeResponse> poSeResponse = safePoTransmit(poSeRequest, mChannelControl);

    closeSessionCmdBuild.createResponseParser(poSeResponse->getApduResponses()[0])->checkStatus();

    /* Sets the flag indicating that the commands have been executed */
    mPoCommandManager->notifyCommandsProcessed();

    /*
     * Session is now considered closed regardless the previous state or the result of the abort
     * session command sent to the PO.
     */
    mSessionState = SessionState::SESSION_CLOSED;
}

void PoTransaction::processVerifyPin(const std::vector<uint8_t>& pin)
{
    KeypleAssert::getInstance().isEqual(pin.size(), CalypsoPoUtils::PIN_LENGTH, "PIN length");

    if (!mCalypsoPo->isPinFeatureAvailable())
        throw CalypsoPoTransactionIllegalStateException("PIN is not available for this PO.");

    if (mPoCommandManager->hasCommands())
        throw CalypsoPoTransactionIllegalStateException("No commands should have been prepared " \
                                                        "prior to a PIN submission.");

    if (mPoSecuritySettings != nullptr &&
        mPoSecuritySettings->getPinTransmissionMode() == PinTransmissionMode::ENCRYPTED) {
        auto challenge = std::make_shared<PoGetChallengeCmdBuild>(mCalypsoPo->getPoClass());
        auto command = std::reinterpret_pointer_cast
                           <AbstractPoCommandBuilder<AbstractPoResponseParser>>(challenge);
        mPoCommandManager->addRegularCommand(command);

        /* Transmit and receive data with the PO */
        processAtomicPoCommands(mPoCommandManager->getPoCommandBuilders(),
                                ChannelControl::KEEP_OPEN);

        /* Sets the flag indicating that the commands have been executed */
        mPoCommandManager->notifyCommandsProcessed();

        // Get the encrypted PIN with the help of the SAM
        const std::vector<uint8_t> cipheredPin =
            mSamCommandProcessor->getCipheredPinData(CalypsoPoUtils::getPoChallenge(),
                                                     pin,
                                                     std::vector<uint8_t>{});

        auto verify = std::make_shared<VerifyPinCmdBuild>(mCalypsoPo->getPoClass(),
                                                          PinTransmissionMode::ENCRYPTED,
                                                          cipheredPin);
        command = std::reinterpret_pointer_cast
                      <AbstractPoCommandBuilder<AbstractPoResponseParser>>(verify);
        mPoCommandManager->addRegularCommand(command);
    } else {
        auto verify = std::make_shared<VerifyPinCmdBuild>(mCalypsoPo->getPoClass(),
                                                          PinTransmissionMode::PLAIN,
                                                          pin);
        auto command = std::reinterpret_pointer_cast
                      <AbstractPoCommandBuilder<AbstractPoResponseParser>>(verify);
        mPoCommandManager->addRegularCommand(command);
    }

    /* Transmit and receive data with the PO */
    processAtomicPoCommands(mPoCommandManager->getPoCommandBuilders(), mChannelControl);

    /* Sets the flag indicating that the commands have been executed */
    mPoCommandManager->notifyCommandsProcessed();
}

void PoTransaction::processVerifyPin(const std::string& pin)
{
    const std::vector<uint8_t> bPin(pin.begin(), pin.end());
    processVerifyPin(bPin);
}

std::shared_ptr<SeResponse> PoTransaction::safePoTransmit(
    const std::shared_ptr<SeRequest> poSeRequest,
    const ChannelControl channelControl)
{
    try {
        return mPoReader->transmitSeRequest(poSeRequest, channelControl);
    } catch (const KeypleReaderIOException& e) {
        throw CalypsoPoIOException("PO IO Exception while transmitting commands.", e);
    }
}

void PoTransaction::checkSessionIsOpen() const
{
    if (mSessionState != SessionState::SESSION_OPEN) {
        std::stringstream ss;
        ss << "Bad session state. Current: " << mSessionState << ", expected: "
           << SessionState::SESSION_OPEN;
        throw CalypsoPoTransactionIllegalStateException(ss.str());
    }
}

void PoTransaction::checkSessionIsNotOpen() const
{
    if (mSessionState == SessionState::SESSION_OPEN) {
        std::stringstream ss;
        ss << "Bad session state. Current: " << mSessionState << ", expected: not open";
        throw CalypsoPoTransactionIllegalStateException(ss.str());
    }
}

void PoTransaction::checkCommandsResponsesSynchronization(const int commandsNumber,
                                                          const int responsesNumber) const
{
    if (commandsNumber != responsesNumber) {
        std::stringstream ss;
        ss << "The number of commands/responses does not match: " << commandsNumber << ", resp="
           << responsesNumber;
        throw CalypsoDesynchronizedExchangesException(ss.str());
    }
}

bool PoTransaction::checkModifyingCommand(
    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>> builder,
    std::atomic<bool>& overflow,
    std::atomic<int>& neededSessionBufferSpace)
{
    if (builder->isSessionBufferUsed()) {
        /* This command affects the PO modifications buffer */
        neededSessionBufferSpace = builder->getApduRequest()->getBytes().size()+
                                   SESSION_BUFFER_CMD_ADDITIONAL_COST -
                                   APDU_HEADER_LENGTH;

        if (isSessionBufferOverflowed(neededSessionBufferSpace)) {
            /* raise an exception if in atomic mode */
            if (mPoSecuritySettings->getSessionModificationMode() == ModificationMode::ATOMIC)
                throw CalypsoAtomicTransactionException("ATOMIC mode error! This command would " \
                                                        "overflow the PO modifications buffer: " +
                                                        builder->getName());
            overflow = true;
        } else {
            overflow = false;
        }

        return true;
    } else {
        return false;
    }
}

bool PoTransaction::isSessionBufferOverflowed(const int sessionBufferSizeConsumed)
{
    bool isSessionBufferFull = false;

    if (mCalypsoPo->isModificationsCounterInBytes()) {
        if (mModificationsCounter - sessionBufferSizeConsumed >= 0) {
            mModificationsCounter -= sessionBufferSizeConsumed;
        } else {
            logger->debug("Modifications buffer overflow! BYTESMODE, CURRENTCOUNTER = %, " \
                          "REQUIREMENT = %",
                          mModificationsCounter,
                          sessionBufferSizeConsumed);
            isSessionBufferFull = true;
        }
    } else {
        if (mModificationsCounter > 0) {
            mModificationsCounter--;
        } else {
            logger->debug("Modifications buffer overflow! COMMANDSMODE, CURRENTCOUNTER = %, " \
                          "REQUIREMENT = %",
                          mModificationsCounter,
                          1);
            isSessionBufferFull = true;
        }
    }

    return isSessionBufferFull;
}

void PoTransaction::resetModificationsBufferCounter()
{
    logger->trace("Modifications buffer counter reset: PREVIOUSVALUE = %, NEWVALUE = %s",
                  mModificationsCounter,
                  mCalypsoPo->getModificationsCounter());

    mModificationsCounter = mCalypsoPo->getModificationsCounter();
}

void PoTransaction::prepareReleasePoChannel()
{
    mChannelControl = ChannelControl::CLOSE_AFTER;
}

void PoTransaction::prepareSelectFile(const std::vector<uint8_t>& lid)
{
    /* Create the builder and add it to the list of commands */
    std::shared_ptr<SelectFileCmdBuild> cmd =
        CalypsoPoUtils::prepareSelectFile(mCalypsoPo->getPoClass(), lid);

    mPoCommandManager->addRegularCommand(
        std::reinterpret_pointer_cast<
            AbstractPoCommandBuilder<AbstractPoResponseParser>>(cmd));
}

void PoTransaction::prepareSelectFile(const SelectFileControl control)
{
    /* Create the builder and add it to the list of commands */
    std::shared_ptr<SelectFileCmdBuild> cmd =
        CalypsoPoUtils::prepareSelectFile(mCalypsoPo->getPoClass(), control);

    mPoCommandManager->addRegularCommand(
        std::reinterpret_pointer_cast<
            AbstractPoCommandBuilder<AbstractPoResponseParser>>(cmd));
}

void PoTransaction::prepareReadRecordFile(const uint8_t sfi,
                                          const int recordNumber)
{
    /* Create the builder and add it to the list of commands */
    std::shared_ptr<ReadRecordsCmdBuild> cmd =
        CalypsoPoUtils::prepareReadRecordFile(mCalypsoPo->getPoClass(),
                                              sfi,
                                              recordNumber);
    mPoCommandManager->addRegularCommand(
        std::reinterpret_pointer_cast<
            AbstractPoCommandBuilder<AbstractPoResponseParser>>(cmd));
}

void PoTransaction::prepareReadRecordFile(const uint8_t sfi,
                                          const int firstRecordNumber,
                                          const int numberOfRecords,
                                          const int recordSize) {

    KeypleAssert::getInstance()
        .isInRange(sfi, CalypsoPoUtils::SFI_MIN, CalypsoPoUtils::SFI_MAX, "sfi")
        .isInRange(firstRecordNumber,
                   CalypsoPoUtils::NB_REC_MIN,
                   CalypsoPoUtils::NB_REC_MAX,
                   "firstRecordNumber")
        .isInRange(numberOfRecords,
                   CalypsoPoUtils::NB_REC_MIN,
                   CalypsoPoUtils::NB_REC_MAX - firstRecordNumber,
                   "numberOfRecords");

    if (numberOfRecords == 1) {
        /* Create the builder and add it to the list of commands */
        mPoCommandManager->addRegularCommand(
            std::reinterpret_pointer_cast<
                AbstractPoCommandBuilder<AbstractPoResponseParser>>(
                    std::make_shared<ReadRecordsCmdBuild>(
                        mCalypsoPo->getPoClass(),
                        sfi,
                        firstRecordNumber,
                        ReadMode::ONE_RECORD,
                        recordSize)));
    } else {
        /*
         * Manages the reading of multiple records taking into account the transmission capacity of
         * the PO and the response format (2 extra bytes)
         * Multiple APDUs can be generated depending on record size and transmission capacity.
         */
        const int recordsPerApdu = mCalypsoPo->getPayloadCapacity() / (recordSize + 2);
        const int maxSizeDataPerApdu = recordsPerApdu * (recordSize + 2);
        int remainingRecords = numberOfRecords;
        int startRecordNumber = firstRecordNumber;

        while (remainingRecords > 0) {
            int expectedLength;
            if (remainingRecords > recordsPerApdu) {
                expectedLength = maxSizeDataPerApdu;
                remainingRecords -= recordsPerApdu;
                startRecordNumber += recordsPerApdu;
            } else {
                expectedLength = remainingRecords * (recordSize + 2);
                remainingRecords = 0;
            }

            /* Create the builder and add it to the list of commands */
            mPoCommandManager->addRegularCommand(
                std::reinterpret_pointer_cast<
                    AbstractPoCommandBuilder<AbstractPoResponseParser>>(
                        std::make_shared<ReadRecordsCmdBuild>(
                            mCalypsoPo->getPoClass(),
                            sfi,
                            startRecordNumber,
                            ReadMode::MULTIPLE_RECORD,
                            expectedLength)));
        }
    }
}

void PoTransaction::prepareReadCounterFile(const uint8_t sfi, const int countersNumber)
{
    prepareReadRecordFile(sfi, 1, 1, countersNumber * 3);
}


void PoTransaction::prepareAppendRecord(const uint8_t sfi, const std::vector<uint8_t>& recordData)
{
    KeypleAssert::getInstance().isInRange(sfi,
                                          CalypsoPoUtils::SFI_MIN,
                                          CalypsoPoUtils::SFI_MAX,
                                          "sfi");

    /* Create the builder and add it to the list of commands */
    mPoCommandManager->addRegularCommand(
        std::reinterpret_pointer_cast<
            AbstractPoCommandBuilder<AbstractPoResponseParser>>(
                std::make_shared<AppendRecordCmdBuild>(mCalypsoPo->getPoClass(),
                                                       sfi,
                                                       recordData)));
}

void PoTransaction::prepareUpdateRecord(const uint8_t sfi,
                                        const int recordNumber,
                                        const std::vector<uint8_t>& recordData)
{
    KeypleAssert::getInstance()
        .isInRange(sfi, CalypsoPoUtils::SFI_MIN, CalypsoPoUtils::SFI_MAX, "sfi")
        .isInRange(recordNumber,
                   CalypsoPoUtils::NB_REC_MIN,
                   CalypsoPoUtils::NB_REC_MAX,
                   "recordNumber");

    /* cReate the builder and add it to the list of commands */
    mPoCommandManager->addRegularCommand(
        std::reinterpret_pointer_cast<
            AbstractPoCommandBuilder<AbstractPoResponseParser>>(
                std::make_shared<UpdateRecordCmdBuild>(mCalypsoPo->getPoClass(),
                                                       sfi,
                                                       recordNumber,
                                                       recordData)));
}

void PoTransaction::prepareWriteRecord(const uint8_t sfi,
                                       const int recordNumber,
                                       const std::vector<uint8_t>& recordData)
{
    KeypleAssert::getInstance()
        .isInRange(sfi, CalypsoPoUtils::SFI_MIN, CalypsoPoUtils::SFI_MAX, "sfi")
        .isInRange(recordNumber,
                   CalypsoPoUtils::NB_REC_MIN,
                   CalypsoPoUtils::NB_REC_MAX,
                   "recordNumber");

    /* Create the builder and add it to the list of commands */
    mPoCommandManager->addRegularCommand(
        std::reinterpret_pointer_cast<
            AbstractPoCommandBuilder<AbstractPoResponseParser>>(
                std::make_shared<WriteRecordCmdBuild>(mCalypsoPo->getPoClass(),
                                                      sfi,
                                                      recordNumber,
                                                      recordData)));
}

void PoTransaction::prepareIncreaseCounter(const uint8_t sfi,
                                           const int counterNumber,
                                           const int incValue)
{
    KeypleAssert::getInstance()
        .isInRange(sfi, CalypsoPoUtils::SFI_MIN, CalypsoPoUtils::SFI_MAX, "sfi")
        .isInRange(counterNumber,
                   CalypsoPoUtils::NB_CNT_MIN,
                   CalypsoPoUtils::NB_CNT_MAX,
                   "counterNumber")
        .isInRange(incValue,
                   CalypsoPoUtils::CNT_VALUE_MIN,
                   CalypsoPoUtils::CNT_VALUE_MAX,
                   "incValue");

    /* Create the builder and add it to the list of commands */
    mPoCommandManager->addRegularCommand(
        std::reinterpret_pointer_cast<AbstractPoCommandResponse>(
                std::make_shared<IncreaseCmdBuild>(mCalypsoPo->getPoClass(),
                                                   sfi,
                                                   counterNumber,
                                                   incValue)));
}

void PoTransaction::prepareDecreaseCounter(const uint8_t sfi,
                                           const int counterNumber,
                                           const int decValue)
{
    KeypleAssert::getInstance()
        .isInRange(sfi, CalypsoPoUtils::SFI_MIN, CalypsoPoUtils::SFI_MAX, "sfi")
        .isInRange(counterNumber,
                   CalypsoPoUtils::NB_CNT_MIN,
                   CalypsoPoUtils::NB_CNT_MAX,
                   "counterNumber")
        .isInRange(decValue,
                   CalypsoPoUtils::CNT_VALUE_MIN,
                   CalypsoPoUtils::CNT_VALUE_MAX,
                   "decValue");

    /* Create the builder and add it to the list of commands */
    auto decrease = std::make_shared<DecreaseCmdBuild>(mCalypsoPo->getPoClass(),
                                                       sfi,
                                                       counterNumber,
                                                       decValue);
    auto cmd = std::reinterpret_pointer_cast<AbstractPoCommandResponse>(decrease);
    mPoCommandManager->addRegularCommand(cmd);
}

void PoTransaction::prepareCheckPinStatus()
{
    if (!mCalypsoPo->isPinFeatureAvailable())
        throw CalypsoPoTransactionIllegalStateException("PIN is not available for this PO.");

    /* Create the builder and add it to the list of commands */
    auto verify = std::make_shared<VerifyPinCmdBuild>(mCalypsoPo->getPoClass());
    auto cmd = std::reinterpret_pointer_cast<AbstractPoCommandResponse>(verify);
    mPoCommandManager->addRegularCommand(cmd);
}

void PoTransaction::prepareSvGet(const Operation& svOperation, const Action& svAction)
{
    if (!mCalypsoPo->isSvFeatureAvailable())
        throw CalypsoPoTransactionIllegalStateException("Stored Value is not available for this " \
                                                        "PO.");

    if (mPoSecuritySettings->getSvGetLogReadMode() == LogRead::ALL &&
        mCalypsoPo->getRevision() != PoRevision::REV3_2) {
        /*
         * @see Calypso Layer ID 8.09/8.10 (200108): both reload and debit logs are requested
         * for a non rev3.2 PO add two SvGet commands (for RELOAD then for DEBIT).
         */
        Operation operation1 = svOperation == Operation::RELOAD?Operation::DEBIT:Operation::RELOAD;
        auto get = std::make_shared<SvGetCmdBuild>(mCalypsoPo->getPoClass(),
                                                   mCalypsoPo->getRevision(),
                                                   operation1);
        auto cmd = std::reinterpret_pointer_cast<AbstractPoCommandResponse>(get);
        mPoCommandManager->addStoredValueCommand(cmd, operation1);
    }

    auto get = std::make_shared<SvGetCmdBuild>(mCalypsoPo->getPoClass(),
                                               mCalypsoPo->getRevision(),
                                               svOperation);
    auto cmd = std::reinterpret_pointer_cast<AbstractPoCommandResponse>(get);
    mPoCommandManager->addStoredValueCommand(cmd, svOperation);

    mSvAction = svAction;
}

void PoTransaction::prepareSvReload(const int amount,
                                    const std::vector<uint8_t> date,
                                    const std::vector<uint8_t> time,
                                    const std::vector<uint8_t> free)
{
    /* Create the initial builder with the application data */
    auto svReloadCmdBuild = std::make_shared<SvReloadCmdBuild>(mCalypsoPo->getPoClass(),
                                                               mCalypsoPo->getRevision(),
                                                               amount,
                                                               CalypsoPoUtils::getSvKvc(),
                                                               date,
                                                               time,
                                                               free);

    /* Get the security data from the SAM */
    const std::vector<uint8_t> svReloadComplementaryData =
        mSamCommandProcessor->getSvReloadComplementaryData(svReloadCmdBuild,
                                                           CalypsoPoUtils::getSvGetHeader(),
                                                           CalypsoPoUtils::getSvGetData());

    /* Finalize the SvReload command builder with the data provided by the SAM */
    svReloadCmdBuild->finalizeBuilder(svReloadComplementaryData);

    /* Create and keep the PoCommand */
    auto cmd = std::reinterpret_pointer_cast<AbstractPoCommandResponse>(svReloadCmdBuild);
    mPoCommandManager->addStoredValueCommand(cmd, Operation::RELOAD);
}

void PoTransaction::prepareSvReload(const int amount)
{
    const std::vector<uint8_t> zero = {0x00, 0x00};
    prepareSvReload(amount, zero, zero, zero);
}

void PoTransaction::prepareSvDebitPriv(const int amount,
                                       const std::vector<uint8_t> date,
                                       const std::vector<uint8_t> time)
{
    if (mPoSecuritySettings->getSvNegativeBalance() == NegativeBalance::FORBIDDEN &&
        (mCalypsoPo->getSvBalance() - amount) < 0)
        throw CalypsoPoTransactionIllegalStateException("Negative balances not allowed.");

    /* Create the initial builder with the application data */
    auto svDebitCmdBuild = std::make_shared<SvDebitCmdBuild>(mCalypsoPo->getPoClass(),
                                                             mCalypsoPo->getRevision(),
                                                             amount,
                                                             CalypsoPoUtils::getSvKvc(),
                                                             date,
                                                             time);

    /* Get the security data from the SAM */
    const std::vector<uint8_t> svDebitComplementaryData =
        mSamCommandProcessor->getSvDebitComplementaryData(svDebitCmdBuild,
                                                          CalypsoPoUtils::getSvGetHeader(),
                                                          CalypsoPoUtils::getSvGetData());

    /* Finalize the SvDebit command builder with the data provided by the SAM */
    svDebitCmdBuild->finalizeBuilder(svDebitComplementaryData);

    /* Create and keep the PoCommand */
    auto cmd = std::reinterpret_pointer_cast<AbstractPoCommandResponse>(svDebitCmdBuild);
    mPoCommandManager->addStoredValueCommand(cmd, Operation::DEBIT);
}

void PoTransaction::prepareSvUndebitPriv(const int amount,
                                         const std::vector<uint8_t> date,
                                         const std::vector<uint8_t> time)
{
    /* Create the initial builder with the application data */
    auto svUndebitCmdBuild = std::make_shared<SvUndebitCmdBuild>(mCalypsoPo->getPoClass(),
                                                                 mCalypsoPo->getRevision(),
                                                                 amount,
                                                                 CalypsoPoUtils::getSvKvc(),
                                                                 date,
                                                                 time);

    /* Get the security data from the SAM */
    const std::vector<uint8_t> svDebitComplementaryData =
        mSamCommandProcessor->getSvUndebitComplementaryData(svUndebitCmdBuild,
                                                            CalypsoPoUtils::getSvGetHeader(),
                                                            CalypsoPoUtils::getSvGetData());

    /* Finalize the SvUndebit command builder with the data provided by the SAM */
    svUndebitCmdBuild->finalizeBuilder(svDebitComplementaryData);

    /* Create and keep the PoCommand */
    auto cmd = std::reinterpret_pointer_cast<AbstractPoCommandResponse>(svUndebitCmdBuild);
    mPoCommandManager->addStoredValueCommand(cmd, Operation::DEBIT);
}

void PoTransaction::prepareSvDebit(const int amount,
                                   const std::vector<uint8_t> date,
                                   const std::vector<uint8_t> time)
{
    if (mSvAction == Action::DO)
        prepareSvDebitPriv(amount, date, time);
    else
        prepareSvUndebitPriv(amount, date, time);
}

void PoTransaction::prepareSvDebit(const int amount)
{
    const std::vector<uint8_t> zero = {0x00, 0x00};
    prepareSvDebit(amount, zero, zero);
}

void PoTransaction::prepareSvReadAllLogs()
{
    if (mCalypsoPo->getApplicationSubtype() != CalypsoPoUtils::STORED_VALUE_FILE_STRUCTURE_ID)
        throw CalypsoPoTransactionIllegalStateException("The currently selected application is " \
                                                        "not an SV application.");

    /* Reset SV data in CalypsoPo if any */
    mCalypsoPo->setSvData(0, 0, nullptr, nullptr);
    prepareReadRecordFile(CalypsoPoUtils::SV_RELOAD_LOG_FILE_SFI,
                          CalypsoPoUtils::SV_RELOAD_LOG_FILE_NB_REC);
    prepareReadRecordFile(CalypsoPoUtils::SV_DEBIT_LOG_FILE_SFI,
                          1,
                          CalypsoPoUtils::SV_DEBIT_LOG_FILE_NB_REC,
                          CalypsoPoUtils::SV_LOG_FILE_REC_LENGTH);
}

void PoTransaction::prepareInvalidate()
{
    if (mCalypsoPo->isDfInvalidated())
        throw CalypsoPoTransactionIllegalStateException("This PO is already invalidated.");

    auto invalidate = std::make_shared<InvalidateCmdBuild>(mCalypsoPo->getPoClass());
    auto cmd = std::reinterpret_pointer_cast<AbstractPoCommandResponse>(invalidate);
    mPoCommandManager->addRegularCommand(cmd);
}

void PoTransaction::prepareRehabilitate()
{
    if (!mCalypsoPo->isDfInvalidated())
        throw CalypsoPoTransactionIllegalStateException("This PO is not invalidated.");

    auto rehabilitate = std::make_shared<RehabilitateCmdBuild>(mCalypsoPo->getPoClass());
    auto cmd = std::reinterpret_pointer_cast<AbstractPoCommandResponse>(rehabilitate);
    mPoCommandManager->addRegularCommand(cmd);
}

/* ACCESS LEVEL --------------------------------------------------------------------------------- */

const AccessLevel AccessLevel::SESSION_LVL_NONE("none", 0x00);
const AccessLevel AccessLevel::SESSION_LVL_PERSO("perso", 0x01);
const AccessLevel AccessLevel::SESSION_LVL_LOAD("load", 0x02);
const AccessLevel AccessLevel::SESSION_LVL_DEBIT("debit", 0x03);

AccessLevel::AccessLevel(const std::string& name, const uint8_t sessionKey)
: mName(name), mSessionKey(sessionKey) {}

const std::string& AccessLevel::getName() const
{
    return mName;
}

uint8_t AccessLevel::getSessionKey() const
{
    return mSessionKey;
}

bool AccessLevel::operator==(const AccessLevel& o) const
{
    return mName == o.mName &&
           mSessionKey == o.mSessionKey;
}

bool AccessLevel::operator!=(const AccessLevel& o) const
{
    return !(*this == o);
}

/*
AccessLevel& AccessLevel::operator=(const AccessLevel& o)
{
    this->mName = o.mName;
    this->mSessionKey = o.mSessionKey;

    return *this;
}
*/
std::ostream& operator<<(std::ostream& os, const AccessLevel& al)
{
    if (al == AccessLevel::SESSION_LVL_PERSO)
        os << "SESSION LVL PERSO";
    else if (al == AccessLevel::SESSION_LVL_LOAD)
        os << "SESSION LVL LOAD";
    else if (al == AccessLevel::SESSION_LVL_DEBIT)
        os << "SESSION LVL DEBIT";
    else
        os << "UNKNOWN";

    return os;
}

bool operator<(const AccessLevel& lhs, const AccessLevel& rhs)
{
    return lhs.mSessionKey < rhs.mSessionKey;
}

/* SESSION STATE ------------------------------------------------------------ */

std::ostream& operator<<(std::ostream& os, const SessionState& ss)
{
    switch (ss) {
    case SessionState::SESSION_CLOSED:
        os << "SESSION CLOSE";
        break;
    case SessionState::SESSION_OPEN:
        os << "SESSION OPEN";
        break;
    case SessionState::SESSION_UNINITIALIZED:
        os << "SESSION UNINITIALIZED";
        break;
    default:
        os << "UNKNOWN";
        break;
    }

    return os;
}

/* OPERATION ------------------------------------------------------------------------------------- */

KEYPLECALYPSO_API std::ostream& operator<<(std::ostream& os, const Operation& o)
{
    if (o == Operation::DEBIT)
        os << "OPERATION = DEBIT";
    else if (o == Operation::NONE)
        os << "OPERATION = NONE";
    else if (o == Operation::RELOAD)
        os << "OPERATION = RELOAD";
    else
        os << "UNKONWN OPERATION";

    return os;
}

}
}
}
