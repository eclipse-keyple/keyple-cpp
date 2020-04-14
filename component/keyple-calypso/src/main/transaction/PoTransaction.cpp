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

#include "AbstractApduCommandBuilder.h"
#include "AbstractApduResponseParser_Import.h"
#include "AbstractOpenSessionCmdBuild.h"
#include "AbstractOpenSessionRespPars.h"
#include "AppendRecordCmdBuild.h"
#include "AppendRecordRespPars.h"
#include "CalypsoBuilderParser.h"
#include "CalypsoPo.h"
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
#include "PoTransaction.h"
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

const uint8_t PoTransaction::CHALLENGE_LENGTH_REV_INF_32 = 0x04;
const uint8_t PoTransaction::CHALLENGE_LENGTH_REV32      = 0x08;
const uint8_t PoTransaction::SIGNATURE_LENGTH_REV_INF_32 = 0x04;
const uint8_t PoTransaction::SIGNATURE_LENGTH_REV32      = 0x08;

std::vector<uint8_t> PoTransaction::ratificationCmdApduLegacy;
std::vector<uint8_t> PoTransaction::ratificationCmdApdu;

PoTransaction::PoTransaction(std::shared_ptr<PoResource> poResource,
                             std::shared_ptr<SamResource> samResource,
                             std::shared_ptr<SecuritySettings> securitySettings)
: PoTransaction::PoTransaction(poResource)
{
    samReader =
        std::dynamic_pointer_cast<ProxyReader>(samResource->getSeReader());
    this->securitySettings = securitySettings;
}

PoTransaction::PoTransaction(std::shared_ptr<PoResource> poResource)
: poReader(std::dynamic_pointer_cast<ProxyReader>(poResource->getSeReader())),
  calypsoPo(poResource->getMatchingSe())
{
    poRevision                    = calypsoPo->getRevision();
    poCalypsoInstanceAid          = calypsoPo->getDfName();
    modificationsCounterIsInBytes = calypsoPo->isModificationsCounterInBytes();
    modificationsCounterMax       = modificationsCounter =
        calypsoPo->getModificationsCounter();
    /* Serial Number of the selected Calypso instance. */
    sessionState                  = SessionState::SESSION_UNINITIALIZED;
    poCalypsoInstanceSerial       = calypsoPo->getApplicationSerialNumber();
    preparedCommandsProcessed     = true;
    this->currentModificationMode = ModificationMode::ATOMIC;
}

std::shared_ptr<SeResponse> PoTransaction::processAtomicOpening(
    PoTransaction::SessionAccessLevel accessLevel, uint8_t openingSfiToSelect,
    uint8_t openingRecordNumberToRead,
    std::vector<std::shared_ptr<
        PoBuilderParser<AbstractPoCommandBuilder<AbstractPoResponseParser>>>>&
        poBuilderParsers)
{
    /*
     * Counts 'select diversifier' and 'get challenge' commands. At least get
     * challenge is present
     */
    int numberOfSamCmd = 1;

    /*
     * SAM ApduRequest List to hold Select Diversifier and Get Challenge
     * commands
     */
    std::vector<std::shared_ptr<ApduRequest>> samApduRequestList;

    logger->debug("processAtomicOpening => Identification: DFNAME = %s, "
                  "SERIALNUMBER = %s\n",
                  ByteArrayUtil::toHex(poCalypsoInstanceAid).c_str(),
                  ByteArrayUtil::toHex(poCalypsoInstanceSerial).c_str());

    /* diversify only if this has not already been done. */
    if (!isDiversificationDone) {
        /*
         * Build the SAM Select Diversifier command to provide the SAM with the
         * PO S/N
         */
        try {
            std::shared_ptr<AbstractApduCommandBuilder> selectDiversifier =
                std::make_shared<SelectDiversifierCmdBuild>(
                    this->samRevision, poCalypsoInstanceSerial);
            samApduRequestList.push_back(selectDiversifier->getApduRequest());
        } catch (std::invalid_argument& e) {
            logger->error("PoTransaction - caught std::invalid_argument "
                          "exception, msg: %s\n",
                          e.what());
            throw e;
		}
        
        /* increment command number */
        numberOfSamCmd++;

        /* change the diversification status */
        isDiversificationDone = true;
    }
    /* Build the SAM Get Challenge command */
    char challengeLength = poRevision == PoRevision::REV3_2
                               ? CHALLENGE_LENGTH_REV32
                               : CHALLENGE_LENGTH_REV_INF_32;

    std::shared_ptr<AbstractSamCommandBuilder> samGetChallenge =
        std::make_shared<SamGetChallengeCmdBuild>(this->samRevision,
                                                  challengeLength);

    samApduRequestList.push_back(samGetChallenge->getApduRequest());

    /* Build a SAM SeRequest */
    std::shared_ptr<SeRequest> samSeRequest =
        std::make_shared<SeRequest>(samApduRequestList);

    logger->debug("processAtomicOpening => identification: SAMSEREQUEST = %s\n",
                  samSeRequest->toString().c_str());

    /*
     * Transmit the SeRequest to the SAM and get back the SeResponse (list of
     * ApduResponse)
     */
    logger->debug("processAtomicOpening - transmiting request on samReader"
                  "(%s)\n",
                  this->samReader->getName().c_str());
    std::shared_ptr<SeResponse> samSeResponse =
        this->samReader->transmit(samSeRequest);

    if (samSeResponse == nullptr) {
        std::vector<std::shared_ptr<ApduResponse>> emptyVector;
        std::vector<std::shared_ptr<ApduRequest>> req =
            samSeRequest->getApduRequests();
        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "Null response received",
            KeypleCalypsoSecureSessionException::Type::SAM, req, emptyVector);
    }

    logger->debug("processAtomicOpening => identification: SAMSERESPONSE = "
                  "%s\n",
                  samSeResponse->toString().c_str());

    std::vector<std::shared_ptr<ApduResponse>> samApduResponseList =
        samSeResponse->getApduResponses();
    std::vector<uint8_t> sessionTerminalChallenge;

    if ((int)samApduResponseList.size() == numberOfSamCmd &&
        samApduResponseList[numberOfSamCmd - 1]->isSuccessful() &&
        (int)samApduResponseList[numberOfSamCmd - 1]->getDataOut().size() ==
            challengeLength) {

        std::shared_ptr<SamGetChallengeRespPars> samChallengePars =
            std::make_shared<SamGetChallengeRespPars>(
                samApduResponseList[numberOfSamCmd - 1]);

        sessionTerminalChallenge = samChallengePars->getChallenge();
        logger->debug("processAtomicOpening => identification: "
                      "TERMINALCHALLENGE = %s\n",
                      ByteArrayUtil::toHex(sessionTerminalChallenge).c_str());
    } else {
        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "Invalid message received",
            KeypleCalypsoSecureSessionException::Type::SAM, samApduRequestList,
            samApduResponseList);
    }

    /*
     * PO ApduRequest List to hold Open Secure Session and other optional
     * commands
     */
    std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;

    /* Build the PO Open Secure Session command */
    // TODO decide how to define the extraInfo field. Empty for the moment.
    std::shared_ptr<AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>>
        poOpenSession =
            AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>::create(
                poRevision, accessLevel.getSessionKey(),
                sessionTerminalChallenge, openingSfiToSelect,
                openingRecordNumberToRead, "");

    /* Add the resulting ApduRequest to the PO ApduRequest list */
    poApduRequestList.push_back(poOpenSession->getApduRequest());

    /*
     * Add all optional PoSendableInSession commands to the PO ApduRequest
     * list
     */
    if (poBuilderParsers.size() > 0) {
        std::vector<std::shared_ptr<ApduRequest>> apduReq =
            this->getApduRequestsToSendInSession(poBuilderParsers);
        poApduRequestList.insert(std::end(poApduRequestList),
                                 std::begin(apduReq), std::end(apduReq));
    }

    /*
     * Create a SeRequest from the ApduRequest list, PO AID as Selector, keep
     * channel open.
     */
    std::shared_ptr<SeRequest> poSeRequest =
        std::make_shared<SeRequest>(poApduRequestList);

    logger->debug("processAtomicOpening => opening:  POSEREQUEST = %s\n",
                  poSeRequest->toString().c_str());

    /* Transmit the commands to the PO */
    std::shared_ptr<SeResponse> poSeResponse = poReader->transmit(poSeRequest);

    logger->debug("processAtomicOpening => opening:  POSERESPONSE = %s\n",
                  poSeResponse->toString().c_str());

    if (poSeResponse == nullptr) {
        std::vector<std::shared_ptr<ApduResponse>> emptyVector;
        std::vector<std::shared_ptr<ApduRequest>> req =
            poSeRequest->getApduRequests();
        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "Null response received",
            KeypleCalypsoSecureSessionException::Type::PO, req, emptyVector);
    }

    if (poSeResponse->wasChannelPreviouslyOpen() == false) {
        std::vector<std::shared_ptr<ApduResponse>> emptyVector;
        std::vector<std::shared_ptr<ApduRequest>> req =
            poSeRequest->getApduRequests();
        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "The logical channel was not open",
            KeypleCalypsoSecureSessionException::Type::PO, req, emptyVector);
    }

    /* Retrieve and check the ApduResponses */
    std::vector<std::shared_ptr<ApduResponse>> poApduResponseList =
        poSeResponse->getApduResponses();

    /* Do some basic checks */
    if (poApduRequestList.size() != poApduResponseList.size()) {
        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "Inconsistent requests and responses",
            KeypleCalypsoSecureSessionException::Type::PO, poApduRequestList,
            poApduResponseList);
    }

    for (auto apduR : poApduResponseList) {
        if (!apduR->isSuccessful()) {
            throw std::make_shared<KeypleCalypsoSecureSessionException>(
                "Invalid response",
                KeypleCalypsoSecureSessionException::Type::PO,
                poApduRequestList, poApduResponseList);
        }
    }

    /* Track Read Records for later use to build anticipated responses. */
    PoTransaction::AnticipatedResponseBuilder::storeCommandResponse(
        poBuilderParsers, poApduRequestList, poApduResponseList, true);

    /*
     * Parse the response to Open Secure Session (the first item of
     * poApduResponseList)
     */
    std::shared_ptr<AbstractOpenSessionRespPars> poOpenSessionPars =
        AbstractOpenSessionRespPars::create(poApduResponseList[0], poRevision);
    std::vector<uint8_t> sessionCardChallenge =
        poOpenSessionPars->getPoChallenge();

    /* Build the Digest Init command from PO Open Session */
    poKif = poOpenSessionPars->getSelectedKif();
    /** The PO KVC */
    // TODO handle rev 1 KVC (provided in the response to select DF. CalypsoPo?)
    std::shared_ptr<Byte> poKvc = poOpenSessionPars->getSelectedKvc();

    logger->debug("processAtomicOpening => opening: CARDCHALLENGE = %s, "
                  "POKIF = %s, POKVC = %s\n",
                  ByteArrayUtil::toHex(sessionCardChallenge).c_str(),
                  StringHelper::formatSimple("%02x", poKif).c_str(),
                  poKvc->toString().c_str());

    if (!securitySettings->isAuthorizedKvc(poKvc->byteValue())) {
        throw KeypleCalypsoSecureSessionUnauthorizedKvcException(
            StringHelper::formatSimple("PO KVC = %02X", poKvc));
    }

    uint8_t kif;
    if (poKif == KIF_UNDEFINED) {
        if (accessLevel == SessionAccessLevel::SESSION_LVL_PERSO) {
            kif = securitySettings->getKeyInfo(
                DefaultKeyInfo::SAM_DEFAULT_KIF_PERSO);
        } else if (accessLevel == SessionAccessLevel::SESSION_LVL_LOAD) {
            kif = securitySettings->getKeyInfo(
                DefaultKeyInfo::SAM_DEFAULT_KIF_LOAD);
        } else {
            kif = securitySettings->getKeyInfo(
                DefaultKeyInfo::SAM_DEFAULT_KIF_DEBIT);
        }
    } else {
        kif = poKif;
    }

    /* Keep the ratification status and read data */
    wasRatified_Renamed = poOpenSessionPars->wasRatified();
    openRecordDataRead  = poOpenSessionPars->getRecordDataRead();

    /*
     * Initialize the DigestProcessor. It will store all digest operations
     * (Digest Init, Digest Update) until the session closing. AT this moment,
     * all SAM Apdu will be processed at once.
     */
    std::vector<uint8_t> dataOut = poApduResponseList[0]->getDataOut();
    DigestProcessor::initialize(
        poRevision, samRevision, false, false,
        poRevision == (PoRevision::REV3_2),
        securitySettings->getKeyInfo(
            SecuritySettings::DefaultKeyInfo::SAM_DEFAULT_KEY_RECORD_NUMBER),
        kif, poKvc->byteValue(), dataOut);

    /*
     * Add all commands data to the digest computation. The first command in the
     * list is the open secure session command. This command is not included in
     * the digest computation, so we skip it and start the loop at index 1.
     */
    if ((poBuilderParsers.size() > 0) && !poBuilderParsers.empty()) {

        for (int i = 1; i < (int)poApduRequestList.size(); i++) {
            /* Add requests and responses to the DigestProcessor */
            DigestProcessor::pushPoExchangeData(poApduRequestList[i],
                                                poApduResponseList[i]);
        }
    }

    sessionState = SessionState::SESSION_OPEN;

    /* Remove Open Secure Session response and create a new SeResponse */
    poApduResponseList.erase(poApduResponseList.begin());

    return std::make_shared<SeResponse>(
        true, true, poSeResponse->getSelectionStatus(), poApduResponseList);
}

std::vector<std::shared_ptr<ApduRequest>>
PoTransaction::getApduRequestsToSendInSession(
    std::vector<std::shared_ptr<
        PoBuilderParser<AbstractPoCommandBuilder<AbstractPoResponseParser>>>>
        poOrSamCommandsInsideSession)
{
    std::vector<std::shared_ptr<ApduRequest>> apduRequestList;

    logger->debug("getApduRequestsToSendInSession - "
                  "poOrSamCommandsInsideSession size: %d\n",
                  poOrSamCommandsInsideSession.size());

    if (poOrSamCommandsInsideSession.size() > 0) {
        for (auto cmd : poOrSamCommandsInsideSession) {
            apduRequestList.push_back(
                cmd->getCommandBuilder()->getApduRequest());
        }
    }

    return apduRequestList;
}

std::shared_ptr<SeResponse> PoTransaction::processAtomicPoCommands(
    std::vector<std::shared_ptr<PoBuilderParser<
        AbstractPoCommandBuilder<AbstractPoResponseParser>>>>& poBuilderParsers,
    ChannelControl channelControl)
{

    /* Get PO ApduRequest List from PoSendableInSession List */
    std::vector<std::shared_ptr<ApduRequest>> poApduRequestList =
        this->getApduRequestsToSendInSession(poBuilderParsers);

    /*
     * Create a SeRequest from the ApduRequest list, PO AID as Selector, manage
     * the logical channel according to the channelControl enum
     */
    std::shared_ptr<SeRequest> poSeRequest =
        std::make_shared<SeRequest>(poApduRequestList);

    logger->debug("processAtomicPoCommands => POREQUEST = %s\n",
                  poSeRequest->toString().c_str());

    /* Transmit the commands to the PO */
    std::shared_ptr<SeResponse> poSeResponse =
        poReader->transmit(poSeRequest, channelControl);

    logger->debug("processAtomicPoCommands => PORESPONSE = %s\n",
                  poSeResponse->toString().c_str());

    if (poSeResponse == nullptr) {
        std::vector<std::shared_ptr<ApduResponse>> emptyVector;
        std::vector<std::shared_ptr<ApduRequest>> req =
            poSeRequest->getApduRequests();

        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "Null response received",
            KeypleCalypsoSecureSessionException::Type::PO, req, emptyVector);
    }

    if (poSeResponse->wasChannelPreviouslyOpen() == false) {
        std::vector<std::shared_ptr<ApduResponse>> emptyVector;
        std::vector<std::shared_ptr<ApduRequest>> req =
            poSeRequest->getApduRequests();

        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "The logical channel was not open",
            KeypleCalypsoSecureSessionException::Type::PO, req, emptyVector);
    }

    /* Retrieve and check the ApduResponses */
    std::vector<std::shared_ptr<ApduResponse>> poApduResponseList =
        poSeResponse->getApduResponses();

    /* Do some basic checks */
    if (poApduRequestList.size() != poApduResponseList.size()) {
        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "Inconsistent requests and responses",
            KeypleCalypsoSecureSessionException::Type::PO, poApduRequestList,
            poApduResponseList);
    }

    for (auto apduR : poApduResponseList) {
        if (!apduR->isSuccessful()) {
            throw std::make_shared<KeypleCalypsoSecureSessionException>(
                "Invalid response",
                KeypleCalypsoSecureSessionException::Type::PO,
                poApduRequestList, poApduResponseList);
        }
    }

    /* Track Read Records for later use to build anticipated responses. */
    AnticipatedResponseBuilder::storeCommandResponse(
        poBuilderParsers, poApduRequestList, poApduResponseList, false);

    /*
     * Add all commands data to the digest computation if this method is called
     * within a Secure Session.
     */
    if (sessionState == SessionState::SESSION_OPEN) {
        for (int i = 0; i < (int)poApduRequestList.size(); i++) {
            /* Add requests and responses to the DigestProcessor */
            PoTransaction::DigestProcessor::pushPoExchangeData(
                poApduRequestList[i], poApduResponseList[i]);
        }
    }
    return poSeResponse;
}

std::shared_ptr<SeResponse> PoTransaction::processAtomicClosing(
    std::vector<std::shared_ptr<
        PoBuilderParser<AbstractPoCommandBuilder<AbstractPoResponseParser>>>>&
        poModificationCommands,
    std::vector<std::shared_ptr<ApduResponse>>& poAnticipatedResponses,
    TransmissionMode transmissionMode, ChannelControl channelControl)
{
    if (sessionState != SessionState::SESSION_OPEN) {
        throw IllegalStateException(StringHelper::formatSimple(
            "Bad security state. Current: %d, expected: %d",
            static_cast<int>(sessionState),
            static_cast<int>(SessionState::SESSION_OPEN)));
    }

    /*
     * Get PO ApduRequest List from PoSendableInSession List - for the first
     * PO exchange
     */
    std::vector<std::shared_ptr<ApduRequest>> poApduRequestList =
        this->getApduRequestsToSendInSession(poModificationCommands);

    /*
     * Compute "anticipated" Digest Update (for optional
     * poModificationCommands)
     */
    if ((poModificationCommands.size() > 0) && !poApduRequestList.empty()) {
        if (poApduRequestList.size() == poAnticipatedResponses.size()) {
            /*
             * Add all commands data to the digest computation: commands and
             * anticipated responses.
             */
            for (int i = 0; i < (int)poApduRequestList.size(); i++) {
                /*
                 * Add requests and responses to the DigestProcessor
                 */
                DigestProcessor::pushPoExchangeData(poApduRequestList[i],
                                                    poAnticipatedResponses[i]);
            }
        } else {
            throw std::make_shared<KeypleCalypsoSecureSessionException>(
                "Inconsistent requests and anticipated responses",
                KeypleCalypsoSecureSessionException::Type::PO,
                poApduRequestList, poAnticipatedResponses);
        }
    }

    /* All SAM digest operations will now run at once. */
    /* Get the SAM Digest request from the cache manager */
    std::shared_ptr<SeRequest> samSeRequest =
        DigestProcessor::getSamDigestRequest();

    logger->debug("processAtomicClosing => SAMREQUEST = %s\n",
                  samSeRequest->toString().c_str());

    /* Transmit SeRequest and get SeResponse */
    std::shared_ptr<SeResponse> samSeResponse =
        samReader->transmit(samSeRequest);

    logger->debug("processAtomicClosing => SAMRESPONSE = %s\n",
                  samSeResponse->toString().c_str());

    if (samSeResponse == nullptr) {
        std::vector<std::shared_ptr<ApduResponse>> emptyVector;
        std::vector<std::shared_ptr<ApduRequest>> req =
            samSeRequest->getApduRequests();

        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "Null response received\n",
            KeypleCalypsoSecureSessionException::Type::SAM, req, emptyVector);
    }

    if (samSeResponse->wasChannelPreviouslyOpen() == false) {
        std::vector<std::shared_ptr<ApduResponse>> emptyVector;
        std::vector<std::shared_ptr<ApduRequest>> req =
            samSeRequest->getApduRequests();
        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "The logical channel was not open\n",
            KeypleCalypsoSecureSessionException::Type::PO, req, emptyVector);
    }

    std::vector<std::shared_ptr<ApduResponse>> samApduResponseList =
        samSeResponse->getApduResponses();

    for (int i = 0; i < (int)samApduResponseList.size(); i++) {
        if (!samApduResponseList[i]->isSuccessful()) {

            logger->debug(
                "processAtomicClosing => command failure REQUEST = "
                "%s, RESPONSE = %s\n",
                samSeRequest->getApduRequests()[i]->toString().c_str(),
                samApduResponseList[i]->toString().c_str());

            throw IllegalStateException("ProcessClosing command failure "
                                        "during digest computation process.");
        }
    }

    /* Get Terminal Signature from the latest response */
    std::vector<uint8_t> sessionTerminalSignature;
    // TODO Add length check according to Calypso REV (4 / 8)
    if (!samApduResponseList.empty()) {
        std::shared_ptr<DigestCloseRespPars> respPars =
            std::make_shared<DigestCloseRespPars>(
                samApduResponseList[samApduResponseList.size() - 1]);

        sessionTerminalSignature = respPars->getSignature();
    }

    logger->debug("processAtomicClosing => SIGNATURE = %s\n",
                  ByteArrayUtil::toHex(sessionTerminalSignature).c_str());

    std::shared_ptr<PoCustomReadCommandBuilder> ratificationCommand;
    bool ratificationAsked;

    if (transmissionMode == TransmissionMode::CONTACTLESS) {
        if (poRevision == PoRevision::REV2_4) {
            ratificationCommand = std::make_shared<PoCustomReadCommandBuilder>(
                "Ratification command", std::make_shared<ApduRequest>(
                                            ratificationCmdApduLegacy, false));
        } else {
            ratificationCommand = std::make_shared<PoCustomReadCommandBuilder>(
                "Ratification command",
                std::make_shared<ApduRequest>(ratificationCmdApdu, false));
        }

        /*
         * Ratification is done by the ratification command above so is not
         * requested in the Close Session command
         */
        ratificationAsked = false;
    } else {
        /*
         * Ratification is requested in the Close Session command in contacts
         * mode
         */
        ratificationAsked = true;
        ratificationCommand.reset();
    }

    /* Build the PO Close Session command. The last one for this session */
    std::shared_ptr<CloseSessionCmdBuild> closeCommand =
        std::make_shared<CloseSessionCmdBuild>(calypsoPo->getPoClass(),
                                               ratificationAsked,
                                               sessionTerminalSignature);

    poApduRequestList.push_back(closeCommand->getApduRequest());

    /* Keep the position of the Close Session command in request list */
    int closeCommandIndex = poApduRequestList.size() - 1;

    /*
     * Add the PO Ratification command if any
     */
    if (ratificationCommand != nullptr) {
        poApduRequestList.push_back(ratificationCommand->getApduRequest());
    }

    /*
     * Transfer PO commands
     */
    std::shared_ptr<SeRequest> poSeRequest =
        std::make_shared<SeRequest>(poApduRequestList);

    logger->debug("processAtomicClosing => POSEREQUEST = %s\n",
                  poSeRequest->toString().c_str());

    std::shared_ptr<SeResponse> poSeResponse;
    try {
        poSeResponse = poReader->transmit(poSeRequest, channelControl);
    } catch (KeypleReaderException& ex) {
        poSeResponse = ex.getSeResponse();
        /*
         * The current exception may have been caused by a communication issue
         * with the PO during the ratification command.
         *
         * In this case, we do not stop the process and consider the Secure
         * Session close. We'll check the signature.
         *
         * We should have one response less than requests.
         */
        if (ratificationAsked || poSeResponse == nullptr ||
            poSeResponse->getApduResponses().size() !=
                poApduRequestList.size() - 1) {

            /* Add current PO SeResponse to exception */
            ex.setSeResponse(poSeResponse);
            throw KeypleReaderException("PO Reader Exception while closing "
                                        "Secure Session\n"); //, ex);
        }
    }

    if (poSeResponse == nullptr) {
        std::vector<std::shared_ptr<ApduResponse>> emptyVector;
        std::vector<std::shared_ptr<ApduRequest>> req =
            poSeRequest->getApduRequests();

        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "Null response received\n",
            KeypleCalypsoSecureSessionException::Type::PO, req, emptyVector);
    }

    if (poSeResponse->wasChannelPreviouslyOpen() == false) {
        std::vector<std::shared_ptr<ApduResponse>> emptyVector;
        std::vector<std::shared_ptr<ApduRequest>> req =
            poSeRequest->getApduRequests();

        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "The logical channel was not open\n",
            KeypleCalypsoSecureSessionException::Type::PO, req, emptyVector);
    }

    logger->debug("processAtomicClosing => POSERESPONSE = %s\n",
                  poSeResponse->toString().c_str());

    std::vector<std::shared_ptr<ApduResponse>> poApduResponseList =
        poSeResponse->getApduResponses();

    /*
     * TODO add support of poRevision parameter to CloseSessionRespPars for
     * REV2.4 PO CLAss byte before last if ratification, otherwise last one
     */
    std::shared_ptr<CloseSessionRespPars> poCloseSessionPars =
        std::make_shared<CloseSessionRespPars>(
            poApduResponseList[closeCommandIndex]);

    if (!poCloseSessionPars->isSuccessful()) {
        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "Didn't get a signature",
            KeypleCalypsoSecureSessionException::Type::PO, poApduRequestList,
            poApduResponseList);
    }

    /* Check the PO signature part with the SAM */
    /* Build and send SAM Digest Authenticate command */
    std::vector<uint8_t> cmd = poCloseSessionPars->getSignatureLo();
    std::shared_ptr<AbstractApduCommandBuilder> digestAuth =
        std::make_shared<DigestAuthenticateCmdBuild>(samRevision, cmd);

    std::vector<std::shared_ptr<ApduRequest>> samApduRequestList;
    samApduRequestList.push_back(digestAuth->getApduRequest());

    samSeRequest = std::make_shared<SeRequest>(samApduRequestList);

    logger->debug("PoTransaction.DigestProcessor => checkPoSignature: "
                  "SAMREQUEST = %s\n",
                  samSeRequest->toString().c_str());

    samSeResponse = samReader->transmit(samSeRequest);

    logger->debug("PoTransaction.DigestProcessor => checkPoSignature: "
                  "SAMRESPONSE = %s\n",
                  samSeResponse->toString().c_str());

    if (samSeResponse == nullptr) {
        std::vector<std::shared_ptr<ApduResponse>> emptyVector;
        std::vector<std::shared_ptr<ApduRequest>> req =
            samSeRequest->getApduRequests();

        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "Null response received",
            KeypleCalypsoSecureSessionException::Type::SAM, req, emptyVector);
    }

    if (samSeResponse->wasChannelPreviouslyOpen() == false) {
        std::vector<std::shared_ptr<ApduResponse>> emptyVector;
        std::vector<std::shared_ptr<ApduRequest>> req =
            samSeRequest->getApduRequests();

        throw std::make_shared<KeypleCalypsoSecureSessionException>(
            "The logical channel was not open\n",
            KeypleCalypsoSecureSessionException::Type::SAM, req, emptyVector);
    }

    /* Get transaction result parsing the response */
    samApduResponseList = samSeResponse->getApduResponses();

    transactionResult = false;
    if ((samApduResponseList.size() > 0) && !samApduResponseList.empty()) {
        std::shared_ptr<DigestAuthenticateRespPars> respPars =
            std::make_shared<DigestAuthenticateRespPars>(
                samApduResponseList[0]);

        transactionResult = respPars->isSuccessful();
        if (transactionResult) {
            logger->debug("PoTransaction.DigestProcessor => checkPoSignature:"
                          "mutual authentication successful\n");
        } else {
            logger->debug("PoTransaction.DigestProcessor => checkPoSignature:"
                          " mutual authentication failure\n");
        }
    } else {
        logger->debug("DigestProcessor => checkPoSignature: no response to "
                      "Digest Authenticate\n");
        throw IllegalStateException("No response to Digest Authenticate\n");
    }

    sessionState = SessionState::SESSION_CLOSED;

    /* Remove ratification response if any */
    if (!ratificationAsked) {
        poApduResponseList.pop_back();
    }
    /* Remove Close Secure Session response and create a new SeResponse */
    poApduResponseList.pop_back();

    return std::make_shared<SeResponse>(
        true, true, poSeResponse->getSelectionStatus(), poApduResponseList);
}

std::shared_ptr<SeResponse> PoTransaction::processAtomicClosing(
    std::vector<std::shared_ptr<PoBuilderParser<
        AbstractPoCommandBuilder<AbstractPoResponseParser>>>>& poBuilderParsers,
    TransmissionMode transmissionMode, ChannelControl channelControl)
{
    std::vector<std::shared_ptr<ApduResponse>> poAnticipatedResponses =
        AnticipatedResponseBuilder::getResponses(poBuilderParsers);

    return processAtomicClosing(poBuilderParsers, poAnticipatedResponses,
                                transmissionMode, channelControl);
}

bool PoTransaction::isSuccessful()
{

    if (sessionState != SessionState::SESSION_CLOSED) {
        throw IllegalStateException(StringHelper::formatSimple(
            "Session is not closed, state: %d, expected: %d\n",
            static_cast<int>(sessionState),
            static_cast<int>(SessionState::SESSION_OPEN)));
    }

    return transactionResult;
}

bool PoTransaction::wasRatified()
{
    if (sessionState == SessionState::SESSION_UNINITIALIZED) {
        throw std::make_shared<IllegalStateException>("No active session\n");
    }

    return wasRatified_Renamed;
}

const std::vector<uint8_t>& PoTransaction::getOpenRecordDataRead() const
{
    if (sessionState == SessionState::SESSION_UNINITIALIZED) {
        throw std::make_shared<IllegalStateException>("No active session\n");
    }

    return openRecordDataRead;
}

PoTransaction::SessionAccessLevel
    PoTransaction::SessionAccessLevel::SESSION_LVL_PERSO(
        "SESSION_LVL_PERSO", InnerEnum::SESSION_LVL_PERSO, "perso",
        static_cast<char>(0x01));
PoTransaction::SessionAccessLevel
    PoTransaction::SessionAccessLevel::SESSION_LVL_LOAD(
        "SESSION_LVL_LOAD", InnerEnum::SESSION_LVL_LOAD, "load",
        static_cast<char>(0x02));
PoTransaction::SessionAccessLevel
    PoTransaction::SessionAccessLevel::SESSION_LVL_DEBIT(
        "SESSION_LVL_DEBIT", InnerEnum::SESSION_LVL_DEBIT, "debit",
        static_cast<char>(0x03));

std::vector<PoTransaction::SessionAccessLevel>
    PoTransaction::SessionAccessLevel::valueList;

PoTransaction::SessionAccessLevel::StaticConstructor::StaticConstructor()
{
    valueList.push_back(SESSION_LVL_PERSO);
    valueList.push_back(SESSION_LVL_LOAD);
    valueList.push_back(SESSION_LVL_DEBIT);
}

PoTransaction::SessionAccessLevel::StaticConstructor
    PoTransaction::SessionAccessLevel::staticConstructor;
int PoTransaction::SessionAccessLevel::nextOrdinal = 0;

PoTransaction::SessionAccessLevel::SessionAccessLevel(
    const std::string& nameValue, InnerEnum innerEnum, const std::string& name,
    char sessionKey)
: innerEnumValue(innerEnum), nameValue(nameValue), ordinalValue(nextOrdinal++),
  name(name), sessionKey(sessionKey)
{
}

std::string PoTransaction::SessionAccessLevel::getName()
{
    return name;
}

char PoTransaction::SessionAccessLevel::getSessionKey()
{
    return sessionKey;
}

bool PoTransaction::SessionAccessLevel::operator==(
    const PoTransaction::SessionAccessLevel& other)
{
    return this->ordinalValue == other.ordinalValue;
}

bool PoTransaction::SessionAccessLevel::operator!=(
    const PoTransaction::SessionAccessLevel& other)
{
    return this->ordinalValue != other.ordinalValue;
}

std::vector<PoTransaction::SessionAccessLevel>
PoTransaction::SessionAccessLevel::values()
{
    return valueList;
}

int PoTransaction::SessionAccessLevel::ordinal()
{
    return ordinalValue;
}

std::string PoTransaction::SessionAccessLevel::toString()
{
    return nameValue;
}

PoTransaction::SessionAccessLevel
PoTransaction::SessionAccessLevel::valueOf(const std::string& name)
{
    for (auto enumInstance : PoTransaction::SessionAccessLevel::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    /* Make compiler happy */
    return PoTransaction::SessionAccessLevel::valueList.front();
}

std::vector<std::vector<uint8_t>>
    PoTransaction::DigestProcessor::poDigestDataCache =
        std::vector<std::vector<uint8_t>>();

SamRevision PoTransaction::DigestProcessor::samRevision = SamRevision::NO_REV;
PoRevision PoTransaction::DigestProcessor::poRevision   = PoRevision::NO_REV;
bool PoTransaction::DigestProcessor::encryption         = false;
bool PoTransaction::DigestProcessor::verification       = false;
bool PoTransaction::DigestProcessor::revMode            = false;
uint8_t PoTransaction::DigestProcessor::keyRecordNumber = 0;
uint8_t PoTransaction::DigestProcessor::keyKIF          = 0;
uint8_t PoTransaction::DigestProcessor::keyKVC          = 0;
const std::shared_ptr<Logger> PoTransaction::DigestProcessor::logger =
    LoggerFactory::getLogger(typeid(PoTransaction::DigestProcessor));

void PoTransaction::DigestProcessor::initialize(
    PoRevision poRev, SamRevision samRev, bool sessionEncryption,
    bool verificationMode, bool rev3_2Mode, uint8_t workKeyRecordNumber,
    uint8_t workKeyKif, uint8_t workKeyKVC,
    const std::vector<uint8_t>& digestData)
{
    /* Store work context */
    poRevision      = poRev;
    samRevision     = SamRevision(samRev);
    encryption      = sessionEncryption;
    verification    = verificationMode;
    revMode         = rev3_2Mode;
    keyRecordNumber = workKeyRecordNumber;
    keyKIF          = workKeyKif;
    keyKVC          = workKeyKVC;

    logger->debug("PoTransaction.DigestProcessor => initialize: POREVISION =" \
                  " %d, SAMREVISION = %d, SESSIONENCRYPTION = %d\n",
                  static_cast<int>(poRev), static_cast<int>(samRev.ordinal()),
                  sessionEncryption, verificationMode);
    logger->debug("PoTransaction.DigestProcessor => initialize: " \
                  "VERIFICATIONMODE = %d, REV32MODE = %d KEYRECNUMBER = %d\n",
                  verificationMode, rev3_2Mode, workKeyRecordNumber);
    logger->debug("PoTransaction.DigestProcessor => initialize: KIF = %s, " \
                  "KVC %s DIGESTDATA = %s\n",
                  StringHelper::formatSimple("%02X", workKeyKif).c_str(),
                  StringHelper::formatSimple("%02X", workKeyKVC).c_str(),
                  ByteArrayUtil::toHex(digestData).c_str());

    /* Clear data cache */
    poDigestDataCache.clear();

    /*
     * Build Digest Init command as first ApduRequest of the digest computation
     * process
     */
    poDigestDataCache.push_back(digestData);
}

void PoTransaction::DigestProcessor::pushPoExchangeData(
    std::shared_ptr<ApduRequest> request,
    std::shared_ptr<ApduResponse> response)
{

    logger->debug("PoTransaction.DigestProcessor => pushPoExchangeData: "
                  "REQUEST = %s\n",
                  request->toString().c_str());

    /*
     * Add an ApduRequest to the digest computation: if the request is of case4
     * type, Le must be excluded from the digest computation. In this cas, we
     * remove here the last byte of the command buffer.
     */
    if (request->isCase4()) {
        poDigestDataCache.push_back(Arrays::copyOfRange(
            request->getBytes(), 0, request->getBytes().size() - 1));
    } else {
        poDigestDataCache.push_back(request->getBytes());
    }

    logger->debug("PoTransaction.DigestProcessor => pushPoExchangeData: "
                  "RESPONSE = %s\n",
                  response->toString().c_str());

    /* Add an ApduResponse to the digest computation */
    poDigestDataCache.push_back(response->getBytes());
}

std::shared_ptr<SeRequest> PoTransaction::DigestProcessor::getSamDigestRequest()
{
    std::vector<std::shared_ptr<ApduRequest>> samApduRequestList;

    if (poDigestDataCache.empty()) {
        logger->debug("PoTransaction.DigestProcessor => getSamDigestRequest: "
                      "no data in cache\n");
        throw IllegalStateException("Digest data cache is empty\n");
    }
    if (poDigestDataCache.size() % 2 == 0) {
        /* the number of buffers should be 2*n + 1 */
        logger->debug("PoTransaction.DigestProcessor => getSamDigestRequest: "
                      "wrong number of buffer in cache NBR = %s\n",
                      poDigestDataCache.size());
        throw IllegalStateException("Digest data cache is inconsistent\n");
    }

    /*
     * Build and append Digest Init command as first ApduRequest of the digest
     * computation process
     */
    samApduRequestList.push_back(
        (std::make_shared<DigestInitCmdBuild>(samRevision, verification,
                                              revMode, keyRecordNumber, keyKIF,
                                              keyKVC, poDigestDataCache[0]))
            ->getApduRequest());

    /*
     * Build and append Digest Update commands
     *
     * The first command is at index 1.
     */
    for (int i = 1; i < (int)poDigestDataCache.size(); i++) {
        samApduRequestList.push_back(
            std::make_shared<DigestUpdateCmdBuild>(samRevision, encryption,
                                                   poDigestDataCache[i])
                ->getApduRequest());
    }

    /*
        * Build and append Digest Close command
        */
    samApduRequestList.push_back(
        std::make_shared<DigestCloseCmdBuild>(samRevision,
                                              poRevision == PoRevision::REV3_2
                                                  ? SIGNATURE_LENGTH_REV32
                                                  : SIGNATURE_LENGTH_REV_INF_32)
            ->getApduRequest());

    return std::make_shared<SeRequest>(samApduRequestList);
}

PoTransaction::AnticipatedResponseBuilder::CommandResponse::CommandResponse(
    std::shared_ptr<ApduRequest> apduRequest,
    std::shared_ptr<ApduResponse> apduResponse)
: apduRequest(apduRequest), apduResponse(apduResponse)
{
}

std::shared_ptr<ApduRequest>
PoTransaction::AnticipatedResponseBuilder::CommandResponse::getApduRequest()
{
    return apduRequest;
}

std::shared_ptr<ApduResponse>
PoTransaction::AnticipatedResponseBuilder::CommandResponse::getApduResponse()
{
    return apduResponse;
}

std::unordered_map<
    char,
    std::shared_ptr<PoTransaction::AnticipatedResponseBuilder::CommandResponse>>
    PoTransaction::AnticipatedResponseBuilder::sfiCommandResponseHashMap;

void PoTransaction::AnticipatedResponseBuilder::storeCommandResponse(
    std::vector<std::shared_ptr<PoBuilderParser<
        AbstractPoCommandBuilder<AbstractPoResponseParser>>>>& poBuilderParsers,
    std::vector<std::shared_ptr<ApduRequest>>& apduRequests,
    std::vector<std::shared_ptr<ApduResponse>>& apduResponses,
    bool skipFirstItem)
{
    if (poBuilderParsers.size() > 0) {
        /*
         * Store Read Records' requests and responses for later use to build anticipated
         * responses.
         */
        std::vector<std::shared_ptr<ApduRequest>>::const_iterator
            apduRequestIterator = apduRequests.begin();
        std::vector<std::shared_ptr<ApduResponse>>::const_iterator
            apduResponseIterator = apduResponses.begin();
        if (skipFirstItem) {
            /* case of processAtomicOpening */
            apduRequestIterator++;
            apduResponseIterator++;
        }

        /* Iterate over the poCommandsInsideSession list */
        for (auto poCommand : poBuilderParsers) {
            if (poCommand->getCommandBuilder()) {
                std::shared_ptr<ApduRequest> apduRequest = *apduRequestIterator;
                char sfi                                 = static_cast<char>(
                    (apduRequest->getBytes()[OFFSET_P2] >> 3) & 0x1F);
                sfiCommandResponseHashMap.emplace(
                    std::pair<char,
                              std::shared_ptr<
                                  PoTransaction::AnticipatedResponseBuilder::
                                      CommandResponse>>(
                        sfi, std::make_shared<CommandResponse>(
                                 apduRequest, *apduResponseIterator)));
            }
            apduRequestIterator++;
            apduResponseIterator++;
        }
    }
}

const std::shared_ptr<Logger>
    PoTransaction::AnticipatedResponseBuilder::logger =
        LoggerFactory::getLogger(
            typeid(PoTransaction::AnticipatedResponseBuilder));

std::vector<std::shared_ptr<ApduResponse>>
PoTransaction::AnticipatedResponseBuilder::getResponses(
    std::vector<std::shared_ptr<
        PoBuilderParser<AbstractPoCommandBuilder<AbstractPoResponseParser>>>>&
        poBuilderParsers)
{
    std::vector<std::shared_ptr<ApduResponse>> apduResponses;
    if (poBuilderParsers.size() > 0) {
        for (auto poBuilderParser : poBuilderParsers) {
            if (std::dynamic_pointer_cast<DecreaseCmdBuild>(
                    poBuilderParser->getCommandBuilder()) != nullptr ||
                std::dynamic_pointer_cast<IncreaseCmdBuild>(
                    poBuilderParser->getCommandBuilder()) != nullptr) {
                /* response = NNNNNN9000 */
                std::vector<uint8_t> modCounterApduRequest =
                    poBuilderParser->getCommandBuilder()
                        ->getApduRequest()
                        ->getBytes();

                /* Retrieve SFI from the current Decrease command */
                uint8_t sfi = (modCounterApduRequest[OFFSET_P2] >> 3) & 0x1F;

                /*
                 * Look for the counter value in the stored records. Only the
                 * first occurrence of the SFI is taken into account. We assume
                 * here that the record number is always 1.
                 */
                std::shared_ptr<
                    PoTransaction::AnticipatedResponseBuilder ::CommandResponse>
                    commandResponse = sfiCommandResponseHashMap[sfi];
                if (commandResponse != nullptr) {
                    char counterNumber = modCounterApduRequest[OFFSET_P1];
                    /*
                     * The record containing the counters is structured as
                     * follows:
                     *   AAAAAAABBBBBBCCCCCC...XXXXXX each counter being a
                     *   3-byte unsigned number. Convert the 3-byte block
                     *   indexed by the counter number to an int.
                     */
                    std::vector<uint8_t> resp =
                        commandResponse->getApduResponse()->getBytes();
                    int currentCounterValue = ByteArrayUtil::threeBytesToInt(
                        resp, (counterNumber - 1) * 3);
                    /*
                     * Extract the add or subtract value from the modification
                     * request
                     */
                    int addSubtractValue = ByteArrayUtil::threeBytesToInt(
                        modCounterApduRequest, OFFSET_DATA);
                    /* Build the response */
                    std::vector<uint8_t> response(5);
                    int newCounterValue;

                    if (std::dynamic_pointer_cast<DecreaseCmdBuild>(
                            poBuilderParser->getCommandBuilder()) != nullptr) {
                        newCounterValue =
                            currentCounterValue - addSubtractValue;
                    } else {
                        newCounterValue =
                            currentCounterValue + addSubtractValue;
                    }

                    response[0] = (newCounterValue & 0x00FF0000) >> 16;
                    response[1] = (newCounterValue & 0x0000FF00) >> 8;
                    response[2] = (newCounterValue & 0x000000FF) >> 0;
                    response[3] = 0x90;
                    response[4] = 0x00;

                    apduResponses.push_back(
                        std::make_shared<ApduResponse>(response, nullptr));

                    logger->debug(
                        "Anticipated response. COMMAND = %s, SFI = %d, "
                        "COUNTERVALUE = %d, DECREMENT = %d, NEWVALUE = %d ",
                        (std::dynamic_pointer_cast<DecreaseCmdBuild>(
                             poBuilderParser->getCommandBuilder()) != nullptr)
                            ? "Decrease"
                            : "Increase",
                        sfi, currentCounterValue, addSubtractValue,
                        newCounterValue);
                } else {
                    throw std::make_shared<KeypleCalypsoSecureSessionException>(
                        StringHelper::formatSimple(
                            "Anticipated response. COMMAND = %s. Unable "
                            "to determine anticipated counter value. SFI"
                            "= %s\n",
                            std::dynamic_pointer_cast<DecreaseCmdBuild>(
                                poBuilderParser->getCommandBuilder()) != nullptr
                                ? "Decrease"
                                : "Increase",
                            StringHelper::to_string(sfi)),
                        poBuilderParser->getCommandBuilder()->getApduRequest(),
                        nullptr);
                }
            } else {
                /* Append/Update/Write Record: response = 9000 */
                std::vector<uint8_t> resp = ByteArrayUtil::fromHex("9000");
                apduResponses.push_back(
                    std::make_shared<ApduResponse>(resp, nullptr));
            }
        }
    }

    return apduResponses;
}

bool PoTransaction::processOpening(ModificationMode modificationMode,
                                   SessionAccessLevel accessLevel,
                                   uint8_t openingSfiToSelect,
                                   uint8_t openingRecordNumberToRead)
{
    currentModificationMode                = modificationMode;
    currentAccessLevel                     = accessLevel;
    uint8_t localOpeningRecordNumberToRead = openingRecordNumberToRead;
    bool poProcessSuccess                  = true;

    /*
     * clear the prepared command list if processed flag is still set (no new
     * command prepared)
     */
    if (preparedCommandsProcessed) {
        poBuilderParserList.clear();
        preparedCommandsProcessed = false;
    }

    /* create a sublist of PoBuilderParser to be sent atomically */
    std::vector<std::shared_ptr<
        PoBuilderParser<AbstractPoCommandBuilder<AbstractPoResponseParser>>>>
        poAtomicCommandList;
    for (auto poCommandElement : poBuilderParserList) {
        /*
         * Use a known AppendRecordCmdBuild derived class to verify its magic
         * number. If the magic number is 12345678 then we can consider the
         * opaque class is derived from PoModificationCommand.
         */
        std::shared_ptr<AppendRecordCmdBuild> poModificationCommand =
            Cast::reinterpret_pointer_cast<AppendRecordCmdBuild>(
                poCommandElement->getCommandBuilder());

        if (poModificationCommand->magic !=
            PoModificationCommand::MAGIC_VALUE) {
            /* This command does not affect the PO modifications buffer */
            logger->debug("processOpening - cmd not affecting PO "
                          "modifications buffer\n");
            poAtomicCommandList.push_back(poCommandElement);
        } else {
            /* This command affects the PO modifications buffer */
            logger->debug("processOpening - cmd affecting PO modifications "
                          "buffer\n");
            if (willOverflowBuffer(poModificationCommand)) {
                logger->debug("processOpening - cmd not fitting into PO "
                              "modifications buffer\n");

                if (currentModificationMode == ModificationMode::ATOMIC) {
                    /* FIXME */
                    throw IllegalStateException(
                        "ATOMIC mode error! This command would overflow"
                        " the PO modifications buffer: "
                        "poCommandBuilderElement->toString()");
                }

                std::shared_ptr<SeResponse> seResponseOpening =
                    processAtomicOpening(currentAccessLevel, openingSfiToSelect,
                                         localOpeningRecordNumberToRead,
                                         poAtomicCommandList);

                /*
                 * inhibit record reading for next round, keep file selection
                 * (TODO check this)
                 */
                localOpeningRecordNumberToRead = 0x00;

                if (!createResponseParsers(seResponseOpening,
                                           poBuilderParserList)) {
                    poProcessSuccess = false;
                }

                /*
                 * Closes the session, resets the modifications buffer counters
                 * for the next round (set the contact mode to avoid the
                 * transmission of the ratification)
                 */
                std::vector<std::shared_ptr<PoBuilderParser<
                    AbstractPoCommandBuilder<AbstractPoResponseParser>>>>
                    emptyVector;
                processAtomicClosing(emptyVector, TransmissionMode::CONTACTS,
                                     ChannelControl::KEEP_OPEN);
                resetModificationsBufferCounter();

                /*
                 * Clear the list and add the command that did not fit in the PO
                 * modifications buffer. We also update the usage counter
                 * without checking the result.
                 */
                poAtomicCommandList.clear();
                poAtomicCommandList.push_back(poCommandElement);

                /*
                 * just update modifications buffer usage counter, ignore result
                 * (always false)
                 */
                willOverflowBuffer(poModificationCommand);
            } else {
                /*
                 * The command fits in the PO modifications buffer, just add it
                 * to the list
                 */
                logger->debug("processOpening - cmd fitting into PO "
                              "modifications buffer\n");
                poAtomicCommandList.push_back(poCommandElement);
            }
        }
    }

    std::shared_ptr<SeResponse> seResponseOpening = processAtomicOpening(
        currentAccessLevel, openingSfiToSelect, localOpeningRecordNumberToRead,
        poAtomicCommandList);

    if (!createResponseParsers(seResponseOpening, poAtomicCommandList)) {
        poProcessSuccess = false;
    }

    /* sets the flag indicating that the commands have been executed */
    logger->debug("processOpening - setting preparedCommandsProcessed to "
                  "true\n");
    preparedCommandsProcessed = true;

    return poProcessSuccess;
}

bool PoTransaction::processPoCommands(ChannelControl channelControl)
{

    /* This method should be called only if no session was previously open */
    if (sessionState == SessionState::SESSION_OPEN) {
        throw std::make_shared<IllegalStateException>("A session is open");
    }

    bool poProcessSuccess = true;

    /*
     * PO commands sent outside a Secure Session. No modifications buffer
     * limitation.
     */
    std::shared_ptr<SeResponse> seResponsePoCommands =
        processAtomicPoCommands(poBuilderParserList, channelControl);

    if (!createResponseParsers(seResponsePoCommands, poBuilderParserList)) {
        poProcessSuccess = false;
    }

    /* sets the flag indicating that the commands have been executed */
    logger->debug("processPoCommands - setting preparedCommandsProcessed to"
                  " true\n");
    preparedCommandsProcessed = true;

    return poProcessSuccess;
}

bool PoTransaction::processPoCommandsInSession()
{
    logger->debug("processPoCommandsInSession\n");

    /** This method should be called only if a session was previously open */
    if (sessionState != SessionState::SESSION_OPEN) {
        throw std::make_shared<IllegalStateException>("No open session");
    }

    /*
     * clear the prepared command list if processed flag is still set (no new
     * command prepared)
     */
    logger->debug("processPoCommandsInSession - preparedCommandsProcessed: "
                  "%d\n",
                  preparedCommandsProcessed);
    if (preparedCommandsProcessed) {
        poBuilderParserList.clear();
        logger->debug("processPoCommandsInSession - setting "
                      "preparedCommandsProcessed to false\n");
        preparedCommandsProcessed = false;
    }

    bool poProcessSuccess = true;

    /* A session is open, we have to care about the PO modifications buffer */
    std::vector<std::shared_ptr<
        PoBuilderParser<AbstractPoCommandBuilder<AbstractPoResponseParser>>>>
        poAtomicBuilderParserList;

    for (auto poBuilderParser : this->poBuilderParserList) {
        logger->debug("processPoCommandsInSession - checking poCommandElement"
                      "from poBuilderParserList\n");

        /*
         * Use a known AppendRecordCmdBuild derived class to verify its magic
         * number. If the magic number is 12345678 then we can consider the
         * opaque class is derived from PoModificationCommand.
         */
        std::shared_ptr<AppendRecordCmdBuild> poModificationCommand =
            Cast::reinterpret_pointer_cast<AppendRecordCmdBuild>(
                poBuilderParser->getCommandBuilder());

        if (poModificationCommand->magic !=
            PoModificationCommand::MAGIC_VALUE) {
            /* This command does not affect the PO modifications buffer */
            logger->debug("processPoCommandsInSession - this command does not"
                          "affect po modifications buffer\n");
            poAtomicBuilderParserList.push_back(poBuilderParser);
        } else {
            /* This command affects the PO modifications buffer */
            logger->debug("processPoCommandsInSession - this command does "
                          "affect po modifications buffer\n");
            if (willOverflowBuffer(poModificationCommand)) {
                if (currentModificationMode == ModificationMode::ATOMIC) {
                    throw IllegalStateException(
                        "ATOMIC mode error! This command would overflow"
                        " the PO modifications buffer: "
                        "<fixme>poCommandBuilderElement->toString()");
                }
                /*
                 * The current command would overflow the modifications buffer
                 * in the PO. We send the current commands and update the
                 * parsers. The parsers Iterator is kept all along the process.
                 */
                std::shared_ptr<SeResponse> seResponsePoCommands =
                    processAtomicPoCommands(poAtomicBuilderParserList,
                                            ChannelControl::KEEP_OPEN);
                if (!createResponseParsers(seResponsePoCommands,
                                           poAtomicBuilderParserList)) {
                    poProcessSuccess = false;
                }

                /*
                 * Close the session and reset the modifications buffer counters
                 * for the next round (set the contact mode to avoid the
                 * transmission of the ratification)
                 */
                std::vector<std::shared_ptr<PoBuilderParser<
                    AbstractPoCommandBuilder<AbstractPoResponseParser>>>>
                    empty;
                processAtomicClosing(empty, TransmissionMode::CONTACTS,
                                     ChannelControl::KEEP_OPEN);
                resetModificationsBufferCounter();

                /*
                 * We reopen a new session for the remaining commands to be
                 * sent
                 */
                std::shared_ptr<SeResponse> seResponseOpening =
                    processAtomicOpening(currentAccessLevel, 0x00, 0x00, empty);
                /*
                 * Clear the list and add the command that did not fit in the PO
                 * modifications buffer. We also update the usage counter
                 * without checking the result.
                 */
                poAtomicBuilderParserList.clear();
                poAtomicBuilderParserList.push_back(poBuilderParser);
                /*
                 * Just update modifications buffer usage counter, ignore result
                 * (always false)
                 */
                willOverflowBuffer(poModificationCommand);
            } else {
                /*
                 * The command fits in the PO modifications buffer, just add it
                 * to the list
                 */
                poAtomicBuilderParserList.push_back(poBuilderParser);
            }
        }
    }

    if (!poAtomicBuilderParserList.empty()) {
        std::shared_ptr<SeResponse> seResponsePoCommands =
            processAtomicPoCommands(poAtomicBuilderParserList,
                                    ChannelControl::KEEP_OPEN);
        if (!createResponseParsers(seResponsePoCommands,
                                   poAtomicBuilderParserList)) {
            poProcessSuccess = false;
        }
    }

    /* sets the flag indicating that the commands have been executed */
    logger->debug("processPoCommandsInSession - setting "
                  "preparedCommandsProcessed to true\n");
    preparedCommandsProcessed = true;

    return poProcessSuccess;
}

bool PoTransaction::processClosing(ChannelControl channelControl)
{
    bool poProcessSuccess        = true;
    bool atLeastOneReadCommand   = false;
    bool sessionPreviouslyClosed = false;

    /*
     * clear the prepared command list if processed flag is still set (no
     * new command prepared)
     */
    if (preparedCommandsProcessed) {
        poBuilderParserList.clear();
        logger->debug("processClosing - setting preparedCommandsProcessed to"
                      " false\n");
        preparedCommandsProcessed = false;
    }

    std::vector<std::shared_ptr<PoModificationCommand>>
        poModificationCommandList;
    std::vector<std::shared_ptr<
        PoBuilderParser<AbstractPoCommandBuilder<AbstractPoResponseParser>>>>
        poAtomicBuilderParserList;
    std::shared_ptr<SeResponse> seResponseClosing;

    for (auto poBuilderParser : poBuilderParserList) {
        /*
         * Use a known AppendRecordCmdBuild derived class to verify its magic
         * number. If the magic number is 12345678 then we can consider the
         * opaque class is derived from PoModificationCommand.
         */
        std::shared_ptr<AppendRecordCmdBuild> poModificationCommand =
            Cast::reinterpret_pointer_cast<AppendRecordCmdBuild>(
                poBuilderParser->getCommandBuilder());
        logger->debug("processClosing - poModificationCommand magic number: "
                      "%d\n",
                      poModificationCommand->magic);

        if (poModificationCommand->magic !=
            PoModificationCommand::MAGIC_VALUE) {
            /*
             * This command does not affect the PO modifications buffer. We
             *  will call processPoCommands first
             */
            poAtomicBuilderParserList.push_back(poBuilderParser);
            atLeastOneReadCommand = true;
        } else {
            /* This command affects the PO modifications buffer */
            if (willOverflowBuffer(poModificationCommand)) {
                if (currentModificationMode == ModificationMode::ATOMIC) {
                    throw IllegalStateException(StringHelper::formatSimple(
                        "ATOMIC mode error! This command would "
                        "overflow the PO modifications buffer: %s\n",
                        "<fixme>poBuilderParser->getCommandBuilder()."
                        "toString()"));
                }

                /*
                 * Reopen a session with the same access level if it was
                 * previously closed in this current processClosing
                 */
                if (sessionPreviouslyClosed) {
                    std::vector<std::shared_ptr<PoBuilderParser<
                        AbstractPoCommandBuilder<AbstractPoResponseParser>>>>
                        empty;
                    processAtomicOpening(currentAccessLevel, 0x00, 0x00, empty);
                }

                /*
                 * If at least one non-modifying was prepared, we use
                 * processAtomicPoCommands instead of processAtomicClosing to
                 * send the list
                 */
                if (atLeastOneReadCommand) {
                    std::vector<std::shared_ptr<PoBuilderParser<
                        AbstractPoCommandBuilder<AbstractPoResponseParser>>>>
                        poBuilderParsers;
                    poBuilderParsers.insert(poBuilderParsers.end(),
                                            poAtomicBuilderParserList.begin(),
                                            poAtomicBuilderParserList.end());
                    seResponseClosing = processAtomicPoCommands(
                        poBuilderParsers, ChannelControl::KEEP_OPEN);
                    atLeastOneReadCommand = false;
                } else {
                    /* All commands in the list are 'modifying' */
                    seResponseClosing = processAtomicClosing(
                        poAtomicBuilderParserList, TransmissionMode::CONTACTS,
                        ChannelControl::KEEP_OPEN);
                    resetModificationsBufferCounter();
                    sessionPreviouslyClosed = true;
                }

                if (!createResponseParsers(seResponseClosing,
                                           poAtomicBuilderParserList)) {
                    poProcessSuccess = false;
                }

                /*
                 * Clear the list and add the command that did not fit in the PO
                 * modifications buffer. We also update the usage counter
                 * without checking the result.
                 */
                poAtomicBuilderParserList.clear();
                poAtomicBuilderParserList.push_back(poBuilderParser);
                /*
                 * just update modifications buffer usage counter, ignore
                 * result (always false)
                 */
                willOverflowBuffer(poModificationCommand);
            } else {
                /*
                 * The command fits in the PO modifications buffer, just add it
                 * to the list
                 */
                poAtomicBuilderParserList.push_back(poBuilderParser);
            }
        }
    }

    if (sessionPreviouslyClosed) {
        /*
         * Reopen if needed, to close the session with the requested conditions
         * (CommunicationMode and channelControl)
         */
        std::vector<std::shared_ptr<PoBuilderParser<
            AbstractPoCommandBuilder<AbstractPoResponseParser>>>>
            empty;
        processAtomicOpening(currentAccessLevel, 0x00, 0x00, empty);
    }

    /* Finally, close the session as requested */
    seResponseClosing =
        processAtomicClosing(poAtomicBuilderParserList,
                             calypsoPo->getTransmissionMode(), channelControl);

    /* Update parsers */
    if (!createResponseParsers(seResponseClosing, poAtomicBuilderParserList)) {
        poProcessSuccess = false;
    }

    /* sets the flag indicating that the commands have been executed */
    logger->debug("processClosing - setting preparedCommandsProcessed to "
                  "true\n");
    preparedCommandsProcessed = true;

    return poProcessSuccess;
}

bool PoTransaction::processCancel(ChannelControl channelControl)
{
    /* PO ApduRequest List to hold Close Secure Session command */
    std::vector<std::shared_ptr<ApduRequest>> poApduRequestList;

    /*
     * Build the PO Close Session command (in "abort" mode since no signature
     * is provided).
     */
    std::shared_ptr<CloseSessionCmdBuild> closeCommand =
        std::make_shared<CloseSessionCmdBuild>(calypsoPo->getPoClass());

    poApduRequestList.push_back(closeCommand->getApduRequest());

    /* Transfer PO commands */
    std::shared_ptr<SeRequest> poSeRequest =
        std::make_shared<SeRequest>(poApduRequestList);

    logger->debug("processCancel => POSEREQUEST = %s",
                  poSeRequest->toString().c_str());

    std::shared_ptr<SeResponse> poSeResponse;
    try {
        poSeResponse = poReader->transmit(poSeRequest, channelControl);
    } catch (KeypleReaderException& ex) {
        poSeResponse = ex.getSeResponse();
    }

    logger->debug("processCancel => POSERESPONSE = %s",
                  poSeResponse->toString().c_str());

    /* sets the flag indicating that the commands have been executed */
    logger->debug(
        "processCancel - setting preparedCommandsProcessed to true\n");
    preparedCommandsProcessed = true;

    /*
     * session is now considered closed regardless the previous state or the
     * result of the abort session command sent to the PO.
     */
    sessionState = SessionState::SESSION_CLOSED;

    /* return the successful status of the abort session command */
    return poSeResponse->getApduResponses()[0]->isSuccessful();
}

bool PoTransaction::createResponseParsers(
    std::shared_ptr<SeResponse> seResponse,
    std::vector<std::shared_ptr<
        PoBuilderParser<AbstractPoCommandBuilder<AbstractPoResponseParser>>>>&
        poBuilderParsers)
{
    bool allSuccessfulCommands = true;
    std::vector<std::shared_ptr<PoBuilderParser<
        AbstractPoCommandBuilder<AbstractPoResponseParser>>>>::const_iterator
        commandIterator = poBuilderParsers.begin();

    /* double loop to set apdu responses to corresponding parsers */
    for (auto apduResponse : seResponse->getApduResponses()) {
        if (commandIterator == poBuilderParsers.end()) {
            throw IllegalStateException("Parsers list and responses list "
                                        "mismatch! ");
        }

        std::shared_ptr<
            PoBuilderParser<AbstractPoCommandBuilder<AbstractPoResponseParser>>>
            poBuilderParser = *commandIterator;
        std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
            commandBuilder = poBuilderParser->getCommandBuilder();
        std::shared_ptr<AbstractPoResponseParser> responseParser =
            commandBuilder->createResponseParser(apduResponse);

        poBuilderParser->setResponseParser(responseParser);
        //setApduResponse(apduResponse);
        if (!apduResponse->isSuccessful()) {
            allSuccessfulCommands = false;
        }
    }
    return allSuccessfulCommands;
}

bool PoTransaction::willOverflowBuffer(
    std::shared_ptr<PoModificationCommand> modificationCommand)
{
    bool willOverflow = false;

    if (modificationsCounterIsInBytes) {
        /*
         * Hackish...
         * Cast into a PoModificationCommand derived class to access all
         * required elements.
         *
         * Casting into an AbstractApduCommandBuilder fails and crashes...
         */
        std::shared_ptr<AppendRecordCmdBuild> poModificationCommand =
            std::static_pointer_cast<AppendRecordCmdBuild>(modificationCommand);
        std::shared_ptr<ApduRequest> apduRequest =
            poModificationCommand->getApduRequest();
        std::vector<uint8_t> bytes = apduRequest->getBytes();

        int bufferRequirement = bytes[OFFSET_Lc] + 6;
        if (modificationsCounter - bufferRequirement > 0) {
            modificationsCounter = modificationsCounter - bufferRequirement;
        } else {
            logger->trace("Modifications buffer overflow! BYTESMODE, "
                          "CURRENTCOUNTER = %d, REQUIREMENT = %d\n",
                          modificationsCounter, bufferRequirement);
            willOverflow = true;
        }
    } else {
        if (modificationsCounter > 0) {
            modificationsCounter--;
        } else {
            logger->trace("Modifications buffer overflow! COMMANDSMODE, "
                          "CURRENTCOUNTER = %d, REQUIREMENT = %d\n",
                          modificationsCounter, 1);
            willOverflow = true;
        }
    }

    logger->debug("willOverflowBuffer - buffer will not overflow\n");
    return willOverflow;
}

void PoTransaction::resetModificationsBufferCounter()
{
    logger->trace("Modifications buffer counter reset: PREVIOUSVALUE = %d, "
                  "NEWVALUE = %d\n",
                  modificationsCounter, modificationsCounterMax);

    modificationsCounter = modificationsCounterMax;
}

int PoTransaction::createAndStoreCommandBuilder(
    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
        commandBuilder)
{
    /* reset the list when preparing the first command after last processing */
    if (preparedCommandsProcessed) {
        poBuilderParserList.clear();
        logger->debug("createAndStoreCommandBuilder - setting "
                      "preparedCommandsProcessed to false\n");
        preparedCommandsProcessed = false;
        preparedCommandIndex      = 0;
    }

    std::shared_ptr<
        PoBuilderParser<AbstractPoCommandBuilder<AbstractPoResponseParser>>>
        cmd = std::make_shared<PoBuilderParser<
            AbstractPoCommandBuilder<AbstractPoResponseParser>>>(
            commandBuilder);

    poBuilderParserList.push_back(cmd);

    /* return and post-increment index */
    preparedCommandIndex++;
    return (preparedCommandIndex - 1);
}

int PoTransaction::prepareSelectFileCmd(const std::vector<uint8_t>& path,
                                        const std::string& extraInfo)
{
    (void)extraInfo;

    logger->trace("Select File: PATH = %s\n",
                  ByteArrayUtil::toHex(path).c_str());

    /* Create and keep the PoBuilderParser, return the command index */
    std::shared_ptr<SelectFileCmdBuild> select =
        std::make_shared<SelectFileCmdBuild>(calypsoPo->getPoClass(), path);
    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
        abstract =
            std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>(
                select,
                reinterpret_cast<
                    AbstractPoCommandBuilder<AbstractPoResponseParser>*>(
                    select.get()));

    return createAndStoreCommandBuilder(abstract);
}

int PoTransaction::prepareSelectFileCmd(
    SelectFileCmdBuild::SelectControl selectControl,
    const std::string& extraInfo)
{
    (void)extraInfo;

    logger->trace("Navigate: CONTROL = %d", static_cast<int>(selectControl));

    /* Create and keep the PoBuilderParser, return the command index */
    std::shared_ptr<SelectFileCmdBuild> select =
        std::make_shared<SelectFileCmdBuild>(calypsoPo->getPoClass(),
                                             selectControl);
    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
        abstract =
            std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>(
                select,
                reinterpret_cast<
                    AbstractPoCommandBuilder<AbstractPoResponseParser>*>(
                    select.get()));

    return createAndStoreCommandBuilder(abstract);
}

int PoTransaction::prepareReadRecordsCmdInternal(
    uint8_t sfi, ReadDataStructure readDataStructureEnum,
    uint8_t firstRecordNumber, int expectedLength, const std::string& extraInfo)
{
    /*
     * the readJustOneRecord flag is set to false only in case of multiple read
     * records, in all other cases it is set to true
     */
    bool readJustOneRecord =
        !(readDataStructureEnum == ReadDataStructure::MULTIPLE_RECORD_DATA);

    /*
     * create and keep the PoBuilderParser, return the command index
     */
    std::shared_ptr<ReadRecordsCmdBuild> rr =
        std::make_shared<ReadRecordsCmdBuild>(
            calypsoPo->getPoClass(), sfi, readDataStructureEnum,
            firstRecordNumber, readJustOneRecord,
            static_cast<char>(expectedLength), extraInfo);

    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
        abstract =
            std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>(
                rr, reinterpret_cast<
                        AbstractPoCommandBuilder<AbstractPoResponseParser>*>(
                        rr.get()));

    return createAndStoreCommandBuilder(abstract);
}

int PoTransaction::prepareReadRecordsCmd(
    uint8_t sfi, ReadDataStructure readDataStructureEnum,
    uint8_t firstRecordNumber, int expectedLength, const std::string& extraInfo)
{
    if (expectedLength < 1 || expectedLength > 250) {
        throw std::invalid_argument("Bad length.");
    }

    return prepareReadRecordsCmdInternal(sfi, readDataStructureEnum,
                                         firstRecordNumber, expectedLength,
                                         extraInfo);
}

int PoTransaction::prepareReadRecordsCmd(
    uint8_t sfi, ReadDataStructure readDataStructureEnum,
    uint8_t firstRecordNumber, const std::string& extraInfo)
{
    if (poReader->getTransmissionMode() == TransmissionMode::CONTACTS) {
        throw std::invalid_argument("In contacts mode, the expected length "
                                    "must be specified.");
    }

    return prepareReadRecordsCmdInternal(sfi, readDataStructureEnum,
                                         firstRecordNumber, 0, extraInfo);
}

int PoTransaction::prepareAppendRecordCmd(
    uint8_t sfi, const std::vector<uint8_t>& newRecordData,
    const std::string& extraInfo)
{
    /* Create and keep the PoBuilderParser, return the command index */
    std::shared_ptr<AppendRecordCmdBuild> ar =
        std::make_shared<AppendRecordCmdBuild>(calypsoPo->getPoClass(), sfi,
                                               newRecordData, extraInfo);

    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
        abstract = Cast::reinterpret_pointer_cast<
            AbstractPoCommandBuilder<AbstractPoResponseParser>>(ar);

    return createAndStoreCommandBuilder(abstract);
}

int PoTransaction::prepareUpdateRecordCmd(
    uint8_t sfi, uint8_t recordNumber,
    const std::vector<uint8_t>& newRecordData, const std::string& extraInfo)
{
    /* Create and keep the PoBuilderParser, return the command index */
    std::shared_ptr<UpdateRecordCmdBuild> ur =
        std::make_shared<UpdateRecordCmdBuild>(calypsoPo->getPoClass(), sfi,
                                               recordNumber, newRecordData,
                                               extraInfo);

    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
        abstract =
            std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>(
                ur, reinterpret_cast<
                        AbstractPoCommandBuilder<AbstractPoResponseParser>*>(
                        ur.get()));

    return createAndStoreCommandBuilder(abstract);
}

int PoTransaction::prepareWriteRecordCmd(
    uint8_t sfi, uint8_t recordNumber,
    const std::vector<uint8_t>& overwriteRecordData,
    const std::string& extraInfo)
{
    /* Create and keep the PoBuilderParser, return the command index */
    std::shared_ptr<WriteRecordCmdBuild> wr =
        std::make_shared<WriteRecordCmdBuild>(calypsoPo->getPoClass(), sfi,
                                              recordNumber, overwriteRecordData,
                                              extraInfo);

    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
        abstract =
            std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>(
                wr, reinterpret_cast<
                        AbstractPoCommandBuilder<AbstractPoResponseParser>*>(
                        wr.get()));

    return createAndStoreCommandBuilder(abstract);
}

int PoTransaction::prepareIncreaseCmd(uint8_t sfi, uint8_t counterNumber,
                                      int incValue,
                                      const std::string& extraInfo)
{
    /* Create and keep the PoBuilderParser, return the command index */
    std::shared_ptr<IncreaseCmdBuild> increase =
        std::make_shared<IncreaseCmdBuild>(calypsoPo->getPoClass(), sfi,
                                           counterNumber, incValue, extraInfo);

    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
        abstract =
            std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>(
                increase,
                reinterpret_cast<
                    AbstractPoCommandBuilder<AbstractPoResponseParser>*>(
                    increase.get()));

    return createAndStoreCommandBuilder(abstract);
}

int PoTransaction::prepareDecreaseCmd(uint8_t sfi, uint8_t counterNumber,
                                      int decValue,
                                      const std::string& extraInfo)
{
    /* Create and keep the PoBuilderParser, return the command index */
    std::shared_ptr<DecreaseCmdBuild> decrease =
        std::make_shared<DecreaseCmdBuild>(calypsoPo->getPoClass(), sfi,
                                           counterNumber, decValue, extraInfo);

    std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>
        abstract =
            std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>>(
                decrease,
                reinterpret_cast<
                    AbstractPoCommandBuilder<AbstractPoResponseParser>*>(
                    decrease.get()));

    return createAndStoreCommandBuilder(abstract);
}

std::shared_ptr<AbstractApduResponseParser>
PoTransaction::getResponseParser(int commandIndex)
{
    if (commandIndex >= static_cast<long>(poBuilderParserList.size())) {
        throw std::invalid_argument(StringHelper::formatSimple(
            "Bad command index: index = %d, "
            "number of commands = %d",
            commandIndex, poBuilderParserList.size()));
    }

    return poBuilderParserList[commandIndex]->getResponseParser();
}

}
}
}
