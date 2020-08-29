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

#include "PoTransaction.h"

/* Calypso */
#include "AbstractOpenSessionCmdBuild.h"
#include "AbstractOpenSessionRespPars.h"
#include "AppendRecordCmdBuild.h"
#include "AppendRecordRespPars.h"
#include "CalypsoBuilderParser.h"
#include "CalypsoPo.h"
#include "CalypsoPoTransactionIllegalStateException.h"
#include "CloseSessionCmdBuild.h"
#include "CloseSessionRespPars.h"
#include "DecreaseCmdBuild.h"
#include "DigestAuthenticateCmdBuild.h"
#include "DigestAuthenticateRespPars.h"
#include "DigestCloseCmdBuild.h"
#include "DigestCloseRespPars.h"
#include "DigestInitCmdBuild.h"
#include "DigestUpdateCmdBuild.h"
#include "KeypleReaderException.h"
#include "KeypleCalypsoSecureSessionException.h"
#include "KeypleCalypsoSecureSessionUnauthorizedKvcException.h"
#include "IncreaseCmdBuild.h"
#include "PoBuilderParser.h"
#include "PoCustomReadCommandBuilder.h"
#include "PoResource.h"
#include "ReadDataStructure.h"
#include "ReadRecordsCmdBuild.h"
#include "SamGetChallengeCmdBuild.h"
#include "SamSendableInSession.h"
#include "SamGetChallengeRespPars.h"
#include "SelectDiversifierCmdBuild.h"
#include "SendableInSession.h"
#include "UpdateRecordCmdBuild.h"
#include "UpdateRecordRespPars.h"
#include "WriteRecordCmdBuild.h"
#include "WriteRecordRespPars.h"

/* Core */
#include "AbstractApduCommandBuilder.h"
#include "AbstractApduResponseParser.h"
#include "ByteArrayUtil.h"
#include "ProxyReader.h"
#include "SeReader.h"

/* Common */
#include "Arrays.h"
#include "Cast.h"
#include "stringhelper.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::core::command;
using namespace keyple::core::seproxy::exception;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;
using namespace keyple::calypso::command;
using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::builder;
using namespace keyple::calypso::command::po::builder::security;
using namespace keyple::calypso::command::po::parser;
using namespace keyple::calypso::command::po::parser::security;
using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::builder;
using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::calypso::command::sam;
using namespace keyple::calypso::command::sam::parser;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::calypso::transaction::exception;

using SessionAccessLevel = PoTransaction::SessionAccessLevel;
using DefaultKeyInfo     = SecuritySettings::DefaultKeyInfo;

const int PoTransaction::SESSION_BUFFER_CMD_ADDITIONAL_COST = 6;
const int PoTransaction::APDU_HEADER_LENGTH = 5;

std::vector<uint8_t> PoTransaction::ratificationCmdApduLegacy =
    {0x94, 0xB2, 0x00, 0x00, 0x00};
std::vector<uint8_t> PoTransaction::ratificationCmdApdu =
    {0x00, 0xB2, 0x00, 0x00, 0x00};

PoTransaction::PoTransaction(
  std::shared_ptr<SeResource<CalypsoPo> poResource,
  std::shared_ptr<PoSecuritySettings> poSecuritySettings)
: PoTransaction(poResource),
  mPoSecuritySettings(poSecuritySettings),
  mSamCommandProcessor(
      std::make_shared<SamCommandProcessor>(poResource, poSecuritySettings)) {}

PoTransaction::PoTransaction(std::shared_ptr<SeResource<CalypsoPo> poResource)
: mPoReader(poResource->getSeReader()),
  mCalypsoPo(poResource->getMatchingSe()),
  mSessionState(SessionState::SESSION_UNINITIALIZED),
{
    mModificationCounter = mCalypsoPo->getModificationCounter();
}

void PoTransaction::processAtomicOpening(
    const std::shared_ptr<AccessLevel> accessLevel,
    std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>>& poCommands)
{
    /* This method should be called only if no session was previously open */
    checkSessionIsNotOpen();

    if (mPoSecuritySettings == nullptr)
        throw CalypsoPoTransactionIllegalStateException(
                  "No SAM resource is available");

    /* Gets the terminal challenge */
    const std::vector<uint8_t>& sessionTerminalChallenge =
        mSamCommandProcessor->getSessionTerminalChallenge();

    /*
     * PO ApduRequest List to hold Open Secure Session and other optional
     * commands
     */
    std::vector<std::shared_ptr<ApduRequest>> poApduRequests;

    /*
     * The sfi and record number to be read when the open secure session command
     * is executed. The default value is 0 (no record to read) but we will
     * optimize the exchanges if a read record command has been prepared.
     */
    int sfi = 0;
    int recordNumber = 0;

    /*
     * Let's check if we have a read record command at the top of the command
     * list.
     *
     * If so, then the command is withdrawn in favour of its equivalent executed
     * at the same time as the open secure session command.
     */
    if (poCommands != nullptr && !poCommands.empty()) {
        std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
            poCommand = poCommands[0];

        std::shared_ptr<ReadRecordsCmdBuild> readRecCmdBuild =
            std::dynamic_pointer_cast<ReadRecordsCmdBuild>(poCommand);

        if (poCommand->getCommandRef() == CalypsoPoCommand::READ_RECORDS &&
            readRecCmdBuild->getReadMode() ==
                ReadRecordsCmdBuild::ReadMode::ONE_RECORD) {
            sfi = readRecCmdBuild->getSfi();
            recordNumber = readRecCmdBuild->getFirstRecordNumber();
            poCommands.erase(poCommands.begin());
        }
    }

    /* Build the PO Open Secure Session command */
    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        openSessionCmdBuild =
            AbstractOpenSessionCmdBuild::create(mCalypsoPo->getRevision(),
                                                accessLevel->getSessionKey(),
                                                sessionTerminalChallenge,
                                                sfi,
                                                recordNumber);

    /* Add the resulting ApduRequest to the PO ApduRequest list */
    poApduRequests.push_back(openSessionCmdBuild->getApduRequest());

    /* Add all optional commands to the PO ApduRequest list */
    const std::vector<std::shared_ptr<ApduRequest>>& commands =
        getApduRequests(poCommands);
    for (const auto& command : commands)
        poApduRequests.push_back(command);

    /*
     * Create a SeRequest from the ApduRequest list, PO AID as Selector, keep
     * channel open
     */
    std::shared_ptr<SeRequest> poSeRequest =
        std::make_shared<SeRequest>(poApduRequests);

    /* Transmit the commands to the PO */
    std::shared_ptr<SeResponse> poSeResponse =
        safePoTransmit(poSeRequest, ChannelControl::KEEP_OPEN);


    /* Retrieve and check the ApduResponses */
    const std::vector<std::shared_ptr<ApduResponse>>& poApduResponses =
        poSeResponse->getApduResponses();

    /* Do some basic checks */
    checkCommandsResponsesSynchronization(poApduRequests.size(),
                                          poApduResponses.size());

    /*
     * Parse the response to Open Secure Session (the first item of
     * poApduResponses). The updateCalypsoPo method fills the CalypsoPo object
     * with the command data and return the parser used for an internal usage
     * here.
     */
    std::shared_ptr<AbstractOpenSessionRespPars> poOpenSessionPars =
        std::dynamic_pointer_cast<AbstractOpenSessionRespPars>(
            CalypsoPoUtils::updateCalypsoPo(mCalypsoPo,
                                            openSessionCmdBuild,
                                            poApduResponses[0]);

    /*
     * Build the Digest Init command from PO Open Session
     * the session challenge is needed for the SAM digest computation
     */
    const std::vector<uint8_t>& sessionCardChallenge =
        poOpenSessionPars->getPoChallenge();

    /* The PO KIF */
    const uint8_t poKif = poOpenSessionPars->getSelectedKif();

    /* The PO KVC, may be null for PO Rev 1.0 */
    const std::shared_ptr<Byte> poKvc = poOpenSessionPars->getSelectedKvc();

    logger->debug("processAtomicOpening => opening: CARDCHALLENGE = %, POKIF " \
                  "= %, POKVC = %",
                  ByteArrayUtil::toHex(sessionCardChallenge),
                  StringHelper::formatSimple("%02X", poKif),
                  StringHelper::formatSimple("%02X", poKvc);

    if (!mPoSecuritySettings->isSessionKvcAuthorized(poKvc))
        throw CalypsoUnauthorizedKvcException(
                  StringHelper::formatSimple("PO KVC = %02X", poKvc));

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
    if ((poCommands != nullptr) && !poCommands->empty())
        /* Add requests and responses to the digest processor */
        mSamCommandProcessor->pushPoExchangeDataList(poApduRequests,
                                                     poApduResponses,
                                                     1);

    /* Remove Open Secure Session response and create a new SeResponse */
    poApduResponses.erase(0);

    /* Update CalypsoPo with the received data */
    CalypsoPoUtils::updateCalypsoPo(calypsoPo, poCommands, poApduResponses);

    mSessionState = SessionState::SESSION_OPEN;
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

std::shared_ptr<SeResponse> PoTransaction::processAtomicPoCommands(
    const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoCommandBuilder<AbstractPoResponseParser>>>& poCommands,
    const ChannelControl channelControl)
{
    /* Get the PO ApduRequest List */
    const std::vector<std::shared_ptr<ApduRequest>> poApduRequests =
        getApduRequests(poCommands);

    /*
     * Create a SeRequest from the ApduRequest list, PO AID as Selector, manage
     * the logical channel according to the channelControl enum
     */
    std::shared_ptr<SeRequest> poSeRequest =
        std::make_shared<SeRequest>(poApduRequests);

    /* Transmit the commands to the PO */
    std::shared_ptr<SeResponse> poSeResponse = safePoTransmit(poSeRequest,
                                                              channelControl);

    /* Retrieve and check the ApduResponses */
    const std::vector<std::shared_ptr<ApduResponse>>& poApduResponses =
        poSeResponse->getApduResponses();

    /* Do some basic checks */
    checkCommandsResponsesSynchronization(poApduRequests.size(),
                                          poApduResponses.size());

    /*
     * Add all commands data to the digest computation if this method is called
     * within a Secure Session.
     */
    if (mSessionState == SessionState::SESSION_OPEN)
        samCommandProcessor->pushPoExchangeDataList(poApduRequests,
                                                    poApduResponses,
                                                    0);

    CalypsoPoUtils::updateCalypsoPo(calypsoPo,
                                    poCommands,
                                    poSeResponse->getApduResponses());
}

void PoTransaction::processAtomicClosing(
    const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>>& poModificationCommands,
    const std::vector<std::shared_ptr<ApduResponse>>&
        poAnticipatedResponses,
    const SessionSetting::RatificationMode ratificationMode,
    const ChannelControl channelControl)
{
    checkSessionIsOpen();

    /* Get the PO ApduRequest List - for the first PO exchange */
    const std::vector<std::shared_ptr<ApduRequest>>& poApduRequests =
        getApduRequests(poModificationCommands);

    /*
     * Compute "anticipated" Digest Update (for optional poModificationCommands)
     */
    if (!poModificationCommands.empty() && !poApduRequests.empty()) {
        checkCommandsResponsesSynchronization(poApduRequests.size(),
                                              poAnticipatedResponses.size());

        /*
         * Add all commands data to the digest computation: commands and
         * anticipated responses.
         */
        mSamCommandProcessor->pushPoExchangeDataList(poApduRequests,
                                                     poAnticipatedResponses,
                                                     0);
    }

    /*
     * All SAM digest operations will now run at once.
     * Get Terminal Signature from the latest response
     */
    const std::vector<uint8_t>& sessionTerminalSignature =
        mSamCommandProcessor->getTerminalSignature();

    bool ratificationCommandResponseReceived;

    /* Build the PO Close Session command. The last one for this session */
    CloseSessionCmdBuild closeSessionCmdBuild(
        mCalypsoPo->getPoClass(),
        SessionSetting::RatificationMode::CLOSE_RATIFIED == ratificationMode,
        sessionTerminalSignature);

    poApduRequests.push_back(closeSessionCmdBuild.getApduRequest());

    /* Keep the position of the Close Session command in request list */
    const int closeCommandIndex = poApduRequests.size() - 1;

    /* Add the PO Ratification command if any */
    bool ratificationCommandAdded;

    if (ratificationMode == RatificationMode::CLOSE_RATIFIED) &&
        mCalypsoPo->getTransmissionMode() == TransmissionMode::CONTACTLESS) {
        poApduRequests.push_back(
            RatificationCmdBuild::getApduRequest(mCalypsoPo.getPoClass()));
        ratificationCommandAdded = true;
    } else {
        ratificationCommandAdded = false;
    }

    /* Transfer PO commands */
    std::shared_ptr<SeRequest> poSeRequest =
        std::make_shared<SeRequest>(poApduRequests);

    std::shared_ptr<SeResponse> poSeResponse;

    try {
        poSeResponse = poReader->transmitSeRequest(poSeRequest, channelControl);

        /*
         * If the ratification command was added and no error occured then the
         * response has been received
         */
        ratificationCommandResponseReceived = ratificationCommandAdded;
    } catch (const KeypleReaderIOException& e) {
        poSeResponse = e.getSeResponse();

        /*
         * The current exception may have been caused by a communication issue
         * with the PO during the ratification command.
         *
         * In this case, we do not stop the process and consider the Secure
         * Session close. We'll check the signature.
         *
         * We should have one response less than requests.
         */
        if (!ratificationCommandAdded ||
            poSeResponse == nullptr ||
            poSeResponse->getApduResponses().size() != poApduRequests.size()-1)
            throw CalypsoPoIOException(
                      "PO IO Exception while transmitting commands.", e);

        /*
         * We received all responses except the response to the ratification
         * command.
         */
        ratificationCommandResponseReceived = false;
    }

    const std::vector<std::shared_ptr<ApduResponse>>& poApduResponses =
        poSeResponse->getApduResponses();

    /* before last if ratification, otherwise last one */
    std::shared_ptr<CloseSessionRespPars> poCloseSessionPars =
            closeSessionCmdBuild.createResponseParser(
                poApduResponses[closeCommandIndex]);
    try {
        poCloseSessionPars->checkStatus();
    } catch (const CalypsoPoCommandException& e) {
        throw CalypsoPoCloseSecureSessionException(
                  "Close Secure Session failed on PO side.", e);
    }

    try {
        mSamCommandProcessor->authenticatePoSignature(
            poCloseSessionPars->getSignatureLo());
    } catch (const CalypsoSamIOException& e) {
        throw CalypsoAuthenticationNotVerifiedException(e.getMessage());
    } catch (const CalypsoSamCommandException& e) {
        throw CalypsoSessionAuthenticationException(
                  "PO authentication failed on SAM side.", e);
    }

    mSessionState = SessionState::SESSION_CLOSED;

    if (ratificationCommandResponseReceived)
        /* Remove the ratification response */
        poApduResponses.erase(poApduResponses.size() - 1);

    /* Remove Close Secure Session response and create a new SeResponse */
    poApduResponses.erase(poApduResponses.size() - 1);

    CalypsoPoUtils::updateCalypsoPo(mCalypsoPo,
                                    poModificationCommands,
                                    poApduResponses);
}

void PoTransaction::processAtomicClosing(
    const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>& poCommands,
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

const int PoTransaction::getCounterValue(const uint8_t sfi, const int counter)
    const
{
    try {
        std::shared_ptr<ElementaryFile> ef = mCalypsoPo->getFileBySfi(sfi);
        return ef->getData().getContentAsCounterValue(counter);
    } catch (const NoSuchElementException& e) {
        throw CalypsoPoTransactionIllegalStateException(
                  StringHelper::formatSimple(
                      "Anticipated response. Unable to determine anticipated" \
                      " value of counter %d in EF sfi %02x",
                      counter,
                      sfi);
    }
}

std::unique_ptr<ApduResponse> PoTransaction::createIncreaseDecreaseResponse(
    const int newCounterValue) const
{
    /* response = NNNNNN9000 */
    const std::vector<uint8_t> response = {
        (newCounterValue & 0x00FF0000) >> 16,
        (newCounterValue & 0x0000FF00) >> 8,
        (newCounterValue & 0x000000FF),
        0x90,
        0x00
    };

    return std::unique_ptr<ApduResponse>(new ApduResponse(response, nullptr));
}

std::vector<std::shared_ptr<ApduResponse>>
    PoTransaction::getAnticipatedResponses(
        const std::vector<std::shared_ptr<AbstractPoCommandBuilder<
            AbstractPoResponseParser>>>& poCommands) const
{
    std::vector<std::shared_ptr<ApduResponse>> apduResponses;

    for (const auto& commandBuilder : poCommands) {
        if (commandBuilder->getCommandRef() == DECREASE) {
            std::shared_ptr<DecreaseCmdBuild> builder =
                std::dynamic_ptr_cast<DecreaseCmdBuild>(commandBuilder);
            const int sfi = builder->getSfi();
            const int counter = builder->getCounterNumber();
            const int newCounterValue = getCounterValue(sfi, counter) -
                                        builder->getDecValue();
            apduResponses.push_back(
                createIncreaseDecreaseResponse(newCounterValue));

        } else if (commandBuilder->getCommandRef() == INCREASE) {
            std::shared_ptr<IncreaseCmdBuild> builder =
                std::dynamic_ptr_cast<IncreaseCmdBuild>(commandBuilder);
            const int sfi = builder->getSfi();
            const int counter = builder->getCounterNumber();
            const int newCounterValue = getCounterValue(sfi, counter) +
                                        builder->getIncValue();
            apduResponses.push_back(
                createIncreaseDecreaseResponse(newCounterValue));

        } else {
            /* Append/Update/Write Record: response = 9000 */
            apduResponses.push_back(
                std::make_shared<ApduResponse>({0x90, 0x00}, nullptr));
        }
    }

    return apduResponses;
}

void PoTransaction::processOpening(const AccessLevel accessLevel)
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
                processAtomicOpening(currentAccessLevel, poAtomicCommands);

                /*
                 * Closes the session, resets the modifications buffer counters
                 * for the next round (set the contact mode to avoid the
                 * transmission of the ratification)
                 */
                processAtomicClosing(nullptr,
                                     RatificationMode::CLOSE_RATIFIED,
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

    processAtomicOpening(currentAccessLevel, poAtomicCommands);

    /* sets the flag indicating that the commands have been executed */
    mPoCommandManager->notifyCommandsProcessed();
}

void PoTransaction::processPoCommands(const ChannelControl channelControl)
{
     /* This method should be called only if no session was previously open */
    checkSessionIsNotOpen();

    /*
     * PO commands sent outside a Secure Session. No modifications buffer
     * limitation.
     */
    processAtomicPoCommands(mPoCommandManager->getPoCommandBuilders(),
                            channelControl);

    /* Sets the flag indicating that the commands have been executed */
    mPoCommandManager->notifyCommandsProcessed();
}

void PoTransaction::processPoCommandsInSession()
{
    /* This method should be called only if a session was previously open */
    checkSessionIsOpen();

    /* A session is open, we have to care about the PO modifications buffer */
    std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>> poAtomicBuilders;

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
                /*
                 * The current command would overflow the modifications buffer
                 * in the PO. We send the current commands and update the
                 * parsers. The parsers Iterator is kept all along the process.
                 */
                processAtomicPoCommands(poAtomicBuilders,
                                        ChannelControl::KEEP_OPEN);

                /*
                 * Close the session and reset the modifications buffer counters
                 * for the next round (set the contact mode to avoid the
                 * transmission of the ratification)
                 */
                processAtomicClosing(nullptr,
                                     RatificationMode::CLOSE_RATIFIED,
                                     ChannelControl::KEEP_OPEN);
                resetModificationsBufferCounter();

                /*
                 * We reopen a new session for the remaining commands to be sent
                 */
                processAtomicOpening(mCurrentAccessLevel, nullptr);

                /*
                 * Clear the list and add the command that did not fit in the PO
                 * modifications buffer. We also update the usage counter
                 * without checking the result.
                 */
                poAtomicBuilders.clear();
                poAtomicBuilders.push_back(commandBuilder);

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

void PoTransaction::processClosing(const ChannelControl channelControl)
{
    checkSessionIsOpen();

    bool atLeastOneReadCommand = false;
    bool sessionPreviouslyClosed = false;

    std::atomic<int> neededSessionBufferSpace;
    std::atomic<bool> overflow;

    std::vector<std::shared_ptr<AbstractPoCommandBuilder<
        AbstractPoResponseParser>>> poAtomicCommands;

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
                /*
                 * Reopen a session with the same access level if it was
                 * previously closed in this current processClosing
                 */
                if (sessionPreviouslyClosed)
                    processAtomicOpening(mCurrentAccessLevel, nullptr);

                /*
                 * If at least one non-modifying was prepared, we use
                 * processAtomicPoCommands instead of processAtomicClosing to
                 * send the list
                 */
                if (atLeastOneReadCommand) {
                    processAtomicPoCommands(poAtomicCommands,
                                            ChannelControl::KEEP_OPEN);

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
                 * Add the command that did not fit in the PO modifications
                 * buffer. We also update the usage counter without checking the
                 * result.
                 */
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
            atLeastOneReadCommand = true;
        }
    }

    if (sessionPreviouslyClosed)
        /*
         * Reopen if needed, to close the session with the requested conditions
         * (CommunicationMode and channelControl)
         */
        processAtomicOpening(mCurrentAccessLevel, nullptr);

    /* Finally, close the session as requested */
    processAtomicClosing(poAtomicCommands,
                         mPoSecuritySettings->getRatificationMode(),
                         channelControl);

    /* Sets the flag indicating that the commands have been executed */
    mPoCommandManager->notifyCommandsProcessed();
}

void PoTransaction::processCancel(const ChannelControl channelControl)
{
    /* PO ApduRequest List to hold Close Secure Session command */
    std::vector<std::shared_ptr<ApduRequest>> poApduRequests;

    /*
     * Build the PO Close Session command (in "abort" mode since no signature is
     * provided).
     */
    CloseSessionCmdBuild closeSessionCmdBuild(mCalypsoPo->getPoClass());

    poApduRequests.push_back(closeSessionCmdBuild.getApduRequest());

    /* Transfer PO commands */
    std::shared_ptr<SeRequest> poSeRequest =
        std::make_shared<SeRequest>(poApduRequests);

    std::shared_ptr<SeResponse> poSeResponse = safePoTransmit(poSeRequest,
                                                              channelControl);

    closeSessionCmdBuild.createResponseParser(
        poSeResponse->getApduResponses()[0])->checkStatus();

    /* sets the flag indicating that the commands have been executed */
    mPoCommandManager->notifyCommandsProcessed();

    /*
     * Session is now considered closed regardless the previous state or the
     * result of the abort session command sent to the PO.
     */
    mSessionState = SessionState::SESSION_CLOSED;
}

std::shared_ptr<SeResponse> PoTransaction::safePoTransmit(
    const std::shared_ptr<SeRequest> poSeRequest,
    const ChannelControl channelControl)
{
    try {
        return mPoReader->transmitSeRequest(poSeRequest, channelControl);
    } catch (const KeypleReaderIOException& e) {
        throw CalypsoPoIOException(
                  "PO IO Exception while transmitting commands.", e);
    }
}

void PoTransaction::checkSessionIsOpen() const
{
    if (mSessionState != SessionState::SESSION_OPEN) {
        std::stringstream ss << "Bad session state. Current: " << mSessionState
                             << ", expected: " << SessionState::SESSION_OPEN;
        throw CalypsoPoTransactionIllegalStateException(ss.str());
    }
}

void PoTransaction::checkSessionIsNotOpen() const
{
    if (mSessionState == SessionState::SESSION_OPEN) {
        std::stringstream ss << "Bad session state. Current: " << mSessionState
                             << ", expected: not open";
        throw CalypsoPoTransactionIllegalStateException(ss.str());
    }
}

void PoTransaction::checkCommandsResponsesSynchronization(
    const int commandsNumber, const int responsesNumber) const
{
    if (commandsNumber != responsesNumber) {
        std::stringstream ss << "The number of commands/responses does not "
                             << "match: " << commandsNumber << ", resp="
                             << responsesNumber;
        throw CalypsoDesynchronizedExchangesException(ss.str());
    }
}

bool PoTransaction::checkModifyingCommand(
    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser> builder,
    std::atomic<bool>& overflow,
    std::atomic<int>& neededSessionBufferSpace)
{
    if (builder->isSessionBufferUsed()) {
        /* This command affects the PO modifications buffer */
        neededSessionBufferSpace = builder->getApduRequest().getBytes().size() +
                                   SESSION_BUFFER_CMD_ADDITIONAL_COST -
                                   APDU_HEADER_LENGTH);

        if (isSessionBufferOverflowed(neededSessionBufferSpace)) {
            /* raise an exception if in atomic mode */
            if (mPoSecuritySettings->getSessionModificationMode() ==
                    ModificationMode::ATOMIC)
                throw CalypsoAtomicTransactionException(
                          "ATOMIC mode error! This command would overflow the" \
                          " PO modifications buffer: " + builder->getName());

            overflow = true;
        } else {
            overflow = false;
        }

        return true;
    } else {
        return false;
    }
}

bool PoTransaction::isSessionBufferOverflowed(
    const int sessionBufferSizeConsumed)
{
    bool isSessionBufferFull = false;

    if (mCalypsoPo->isModificationsCounterInBytes()) {
        if (mModificationsCounter - sessionBufferSizeConsumed >= 0) {
            mModificationsCounter -= sessionBufferSizeConsumed;
        } else {
            logger->debug("Modifications buffer overflow! BYTESMODE, " \
                          "CURRENTCOUNTER = %, REQUIREMENT = %",
                          mModificationsCounter,
                          sessionBufferSizeConsumed);

            isSessionBufferFull = true;
        }
    } else {
        if (mModificationsCounter > 0) {
            mModificationsCounter--;
        } else {
            logger->debug("Modifications buffer overflow! COMMANDSMODE, " \
                          "CURRENTCOUNTER = %, REQUIREMENT = %",
                          mModificationsCounter,
                          1);

            isSessionBufferFull = true;
        }
    }

    return isSessionBufferFull;
}

void PoTransaction::resetModificationsBufferCounter()
{
    logger->trace("Modifications buffer counter reset: PREVIOUSVALUE = %, " \
                  "NEWVALUE = %s",
                  mModificationsCounter,
                  mCalypsoPo->getModificationsCounter());

    mModificationsCounter = mCalypsoPo->getModificationsCounter();
}

void PoTransaction::prepareSelectFile(const std::vector<uint8_>& lid)
{
    /* Create the builder and add it to the list of commands */
    mPoCommandManager->addRegularCommand(
        CalypsoPoUtils::prepareSelectFile(mCalypsoPo->getPoClass(), lid));
}

void PoTransaction::prepareSelectFile(const SelectFileControl control)
{
    /* Create the builder and add it to the list of commands */
    mPoCommandManager->addRegularCommand(
        CalypsoPoUtils::prepareSelectFile(mCalypsoPo->getPoClass(), control));
}

void PoTransaction::prepareReadRecordFile(const uint8_t sfi,
                                          const int recordNumber)
{
    /* Create the builder and add it to the list of commands */
    mPoCommandManager->addRegularCommand(
        CalypsoPoUtils::prepareReadRecordFile(mCalypsoPo.getPoClass(),
                                              sfi,
                                              recordNumber));
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
            std::make_shared<ReadRecordsCmdBuild>(mCalypsoPo.getPoClass(),
                                                  sfi,
                                                  firstRecordNumber,
                                                  ReadMode::ONE_RECORD,
                                                  recordSize));
    } else {
        /*
         * Manages the reading of multiple records taking into account the
         * transmission capacity of the PO and the response format (2 extra
         * bytes)
         * Multiple APDUs can be generated depending on record size and
         * transmission capacity.
         */
        const int recordsPerApdu = mCalypsoPo->getPayloadCapacity() /
                                   (recordSize + 2);
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
                std::make_shared<ReadRecordsCmdBuild>(mCalypsoPo->getPoClass(),
                                                      sfi,
                                                      startRecordNumber,
                                                      ReadMode::MULTIPLE_RECORD,
                                                      expectedLength));
        }
    }
}

void PoTransaction::prepareReadCounterFile(const uint8_t sfi,
                                           const int countersNumber)
{
    prepareReadRecordFile(sfi, 1, 1, countersNumber * 3);
}


void PoTransaction::prepareAppendRecord(const uint8_t sfi,
                                        const std::vector<uint8_t>& recordData)
{
    KeypleAssert::getInstance()
        .isInRange(sfi,
                   CalypsoPoUtils::SFI_MIN,
                   CalypsoPoUtils::SFI_MAX,
                   "sfi");

    /* Create the builder and add it to the list of commands */
    mPoCommandManager->addRegularCommand(
        std::make_shared<AppendRecordCmdBuild>(mCalypsoPo->getPoClass(),
                                               sfi,
                                               recordData));
}

void PoTransaction::prepareUpdateRecord(const uint8_t sfi,
                                        const int recordNumber,
                                        const std::vector<uint8_t>& recordData)
{
    KeypleAssert::getInstance()
        .isInRange(sfi, CalypsoPoUtils::SFI_MIN, CalypsoPoUtils::SFI_MAX, "sfi")
        .isInRange(recordNumber,
                   CalypsoPoUtil::NB_REC_MIN,
                   CalypsoPoUtils::NB_REC_MAX,
                   "recordNumber");

    /* cReate the builder and add it to the list of commands */
    MpoCommandManager->addRegularCommand(
        std::make_shared<UpdateRecordCmdBuild>(mCalypsoPo->getPoClass(),
                                               sfi,
                                               recordNumber,
                                               recordData));
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
        std::make_shared<WriteRecordCmdBuild>(mCalypsoPo->getPoClass(),
                                              sfi,
                                              recordNumber,
                                              recordData));
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
    mPoCommandManager.addRegularCommand(
        std::make_shared<WriteRecordCmdBuild>(mCalypsoPo->getPoClass(),
                                              sfi,
                                              counterNumber,
                                              incValue));
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
    mPoCommandManager.addRegularCommand(
        std::make_shared<WriteRecordCmdBuild>(mCalypsoPo->getPoClass(),
                                              sfi,
                                              counterNumber,
                                              decValue));
}

/* ACCESS LEVEL ------------------------------------------------------------- */

AccessLevel AccessLevel::SESSION_LVL_PERSO("perso", 0x01);
AccessLevel AccessLevel::SESSION_LVL_LOAD("load", 0x02);
AccessLevel AccessLevel::SESSION_LVL_DEBIT("debit", 0x03);

AccessLevel::AccessLevel(const std::string& name, const uint8_t sessionKey)
: mNname(name), mSessionKey(sessionKey) {}

const std::string& AccessLevel::getName() const
{
    return mName;
}

const uint8_t AccessLevel::getSessionKey() const
{
    return mSessionKey;
}

bool AccessLevel::operator==(const AccessLevel& other) const
{
    return mName == other.mName &&
           mSessionKey == other.mSessionKey;
}

bool AccessLevel::operator!=(const AccessLevel& other) const
{
    return !(*this == other);
}

}
}
}
