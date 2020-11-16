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

#include "SamCommandProcessor.h"

/* Calypso */
#include "CalypsoDesynchronizedExchangesException.h"
#include "CalypsoPo.h"
#include "CalypsoSamIOException.h"
#include "CardCipherPinCmdBuild.h"
#include "CardCipherPinRespPars.h"
#include "DigestAuthenticateCmdBuild.h"
#include "DigestAuthenticateRespPars.h"
#include "DigestCloseCmdBuild.h"
#include "DigestCloseRespPars.h"
#include "DigestInitCmdBuild.h"
#include "DigestUpdateCmdBuild.h"
#include "GiveRandomCmdBuild.h"
#include "KeyReference.h"
#include "PoSecuritySettings.h"
#include "SamGetChallengeCmdBuild.h"
#include "SamGetChallengeRespPars.h"
#include "SamReadKeyParametersRespPars.h"
#include "SelectDiversifierCmdBuild.h"
#include "SvCheckCmdBuild.h"
#include "SvCheckRespPars.h"
#include "SvDebitCmdBuild.h"
#include "SvPrepareDebitCmdBuild.h"
#include "SvPrepareLoadCmdBuild.h"
#include "SvPrepareOperationRespPars.h"
#include "SvPrepareUndebitCmdBuild.h"

/* Common */
#include "Arrays.h"
#include "IllegalStateException.h"
#include "KeypleStd.h"
#include "System.h"

/* Core */
#include "KeypleReaderIOException.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso;
using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::calypso::transaction::exception;
using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::seproxy::exception;

using AbstractSamCommandResponse = AbstractSamCommandBuilder<AbstractSamResponseParser>;

const uint8_t SamCommandProcessor::KIF_UNDEFINED = 0xff;
const uint8_t SamCommandProcessor::CHALLENGE_LENGTH_REV_INF_32 = 0x04;
const uint8_t SamCommandProcessor::CHALLENGE_LENGTH_REV32 = 0x08;
const uint8_t SamCommandProcessor::SIGNATURE_LENGTH_REV_INF_32 = 0x04;
const uint8_t SamCommandProcessor::SIGNATURE_LENGTH_REV32 = 0x08;

SamCommandProcessor::SamCommandProcessor(
  const std::shared_ptr<SeResource<CalypsoPo>> poResource,
  const std::shared_ptr<PoSecuritySettings> poSecuritySettings)
: mSamResource(poSecuritySettings->getSamResource()),
  mSamReader(std::dynamic_pointer_cast<ProxyReader>(mSamResource->getSeReader())),
  mPoResource(poResource),
  mPoSecuritySettings(poSecuritySettings),
  mWorkKeyKif(0),
  mWorkKeyKVC(0),
  mIsDiversificationDone(false),
  mIsDigestInitDone(false),
  mIsDigesterInitialized(false),
  mSessionEncryption(false),
  mVerificationMode(false) {}

const std::vector<uint8_t> SamCommandProcessor::getSessionTerminalChallenge()
{
    std::vector<std::shared_ptr<ApduRequest>> apduRequests;

    /* Diversify only if this has not already been done. */
    if (!mIsDiversificationDone) {
        /*
         * Build the SAM Select Diversifier command to provide the SAM with the
         * PO S/N
         */
        std::shared_ptr<AbstractApduCommandBuilder> selectDiversifier =
            std::make_shared<SelectDiversifierCmdBuild>(
               mSamResource->getMatchingSe()->getSamRevision(),
               mPoResource->getMatchingSe()->getApplicationSerialNumberBytes());

        apduRequests.push_back(selectDiversifier->getApduRequest());

        /* Note that the diversification has been made */
        mIsDiversificationDone = true;
    }

    /* Build the SAM Get Challenge command */
    const uint8_t challengeLength =
        mPoResource->getMatchingSe()->isConfidentialSessionModeSupported() ?
            CHALLENGE_LENGTH_REV32 : CHALLENGE_LENGTH_REV_INF_32;

    std::shared_ptr<AbstractSamCommandResponse>
        getChallengeCmdBuild =
            std::reinterpret_pointer_cast<
                AbstractSamCommandResponse>(
                    std::make_shared<SamGetChallengeCmdBuild>(
                        mSamResource->getMatchingSe()->getSamRevision(),
                        challengeLength));

    apduRequests.push_back(getChallengeCmdBuild->getApduRequest());

    /*
     * Transmit the SeRequest to the SAM and get back the SeResponse (list of
     * ApduResponse)
     */
    std::shared_ptr<SeResponse> samSeResponse;

    try {
        samSeResponse =
            mSamReader->transmitSeRequest(
                std::make_shared<SeRequest>(apduRequests),
                ChannelControl::KEEP_OPEN);
    } catch (const KeypleReaderIOException& e) {
        throw CalypsoSamIOException(
                  "SAM IO Exception while getting terminal challenge.", e);
    }

    const std::vector<std::shared_ptr<ApduResponse>> samApduResponses =
        samSeResponse->getApduResponses();

    std::vector<uint8_t> sessionTerminalChallenge;

    const int numberOfSamCmd = apduRequests.size();
    if (static_cast<int>(samApduResponses.size()) == numberOfSamCmd) {
        auto getChallengeRespPars =
            std::dynamic_pointer_cast<SamGetChallengeRespPars>(
                getChallengeCmdBuild->createResponseParser(
                    samApduResponses[numberOfSamCmd - 1]));

        getChallengeRespPars->checkStatus();

        sessionTerminalChallenge = getChallengeRespPars->getChallenge();

        mLogger->debug("identification: TERMINALCHALLENGE = %\n",
                       sessionTerminalChallenge);

    } else {
        std::stringstream ss;
        ss << "The number of commands/responses does not "
           << "match: cmd=" << numberOfSamCmd << ", resp="
           << samApduResponses.size();
        throw CalypsoDesynchronizedExchangesException(ss.str());
    }

    return sessionTerminalChallenge;
}

uint8_t SamCommandProcessor::determineWorkKif(const uint8_t poKif,
                                              const AccessLevel& accessLevel)
    const
{
    uint8_t kif;

    if (poKif == KIF_UNDEFINED)
        kif = mPoSecuritySettings->getSessionDefaultKif(accessLevel);
    else
        kif = poKif;

    return kif;
}

void SamCommandProcessor::initializeDigester(
    const AccessLevel& accessLevel,
    const bool sessionEncryption,
    const bool verificationMode,
    const uint8_t poKif,
    const uint8_t poKVC,
    const std::vector<uint8_t> digestData)
{
    mSessionEncryption = sessionEncryption;
    mVerificationMode = verificationMode;
    /* TODO check in which case this key number is needed */
    // mWorkKeyRecordNumber = mPoSecuritySettings->getSessionDefaultKeyRecordNumber(accessLevel);
    mWorkKeyKif = determineWorkKif(poKif, accessLevel);
    /* TODO handle Rev 1.0 case where KVC is not available */
    mWorkKeyKVC = poKVC;

    mLogger->debug("initialize: POREVISION = %, SAMREVISION = %, SESSIONENCRYPTION = %, " \
                   "VERIFICATIONMODE = %\n",
                   mPoResource->getMatchingSe()->getRevision(),
                   mSamResource->getMatchingSe()->getSamRevision(),
                   sessionEncryption,
                   verificationMode);

    mLogger->debug("initialize: VERIFICATIONMODE = %, REV32MODE = %, KEYRECNUMBER = %\n",
                   verificationMode,
                   mPoResource->getMatchingSe() ->isConfidentialSessionModeSupported(),
                   mWorkKeyRecordNumber);

    mLogger->debug("initialize: KIF = %, KVC = %, DIGESTDATA = %\n", poKif, poKVC, digestData);

    /* Clear data cache */
    mPoDigestDataCache.clear();

    /* Build Digest Init command as first ApduRequest of the digest computation process */
    mPoDigestDataCache.push_back(digestData);

    mIsDigestInitDone = false;
    mIsDigesterInitialized = true;
}

void SamCommandProcessor::pushPoExchangeData(
    const std::shared_ptr<ApduRequest> request,
    const std::shared_ptr<ApduResponse> response)
{
    mLogger->trace("pushPoExchangeData: REQUEST = %\n", request);

    /*
     * Add an ApduRequest to the digest computation: if the request is of case4
     * type, Le must be excluded from the digest computation. In this cas, we
     * remove here the last byte of the command buffer.
     */
    if (request->isCase4())
        mPoDigestDataCache.push_back(
            Arrays::copyOfRange(request->getBytes(),
                                0,
                                request->getBytes().size() - 1));
    else
        mPoDigestDataCache.push_back(request->getBytes());

    mLogger->trace("pushPoExchangeData: RESPONSE = %\n", response);

    /* Add an ApduResponse to the digest computation */
    mPoDigestDataCache.push_back(response->getBytes());
}

void SamCommandProcessor::pushPoExchangeDataList(
    const std::vector<std::shared_ptr<ApduRequest>>& requests,
    const std::vector<std::shared_ptr<ApduResponse>>& responses,
    const int startIndex)
{
    for (int i = startIndex; i < static_cast<int>(requests.size()); i++)
        /* Add requests and responses to the digest processor */
        pushPoExchangeData(requests[i], responses[i]);
}

const std::vector<std::shared_ptr<AbstractSamCommandResponse>>
    SamCommandProcessor::getPendingSamCommands(const bool addDigestClose)
{
    /* TODO optimization with the use of Digest Update Multiple whenever possible */
    std::vector<std::shared_ptr<AbstractSamCommandResponse>> samCommands;

    /* Sanity checks */
    if (mPoDigestDataCache.empty()) {
        mLogger->debug("getSamDigestRequest: no data in cache\n");
        throw IllegalStateException("Digest data cache is empty.");
    }

    if (!mIsDigestInitDone && mPoDigestDataCache.size() % 2 == 0) {
        /* The number of buffers should be 2*n + 1 */
        mLogger->debug("getSamDigestRequest: wrong number of buffer in cache NBR = %\n.",
                       mPoDigestDataCache.size());
        throw IllegalStateException("Digest data cache is inconsistent.");
    }

    if (!mIsDigestInitDone) {
        /*
         * Build and append Digest Init command as first ApduRequest of the digest computation
         * process. The Digest Init command comes from the Open Secure Session response from the PO.
         * Once added to the ApduRequest list, the data is remove from the cache to keep only
         * couples of PO request/response
         */
        auto init = std::make_shared<DigestInitCmdBuild>(
                        mSamResource->getMatchingSe()->getSamRevision(),
                        mVerificationMode,
                        mPoResource->getMatchingSe()->isConfidentialSessionModeSupported(),
                        mWorkKeyRecordNumber,
                        mWorkKeyKif,
                        mWorkKeyKVC,
                        mPoDigestDataCache[0]);
        auto cmd = std::reinterpret_pointer_cast<AbstractSamCommandResponse>(init);
        samCommands.push_back(cmd);

        mPoDigestDataCache.erase(mPoDigestDataCache.begin());

        /* Note that the digest init has been made */
        mIsDigestInitDone = true;
    }

    /* Build and append Digest Update commands */
    for (int i = 0; i < static_cast<int>(mPoDigestDataCache.size()); i++) {
        auto update = std::make_shared<DigestUpdateCmdBuild>(
                          mSamResource->getMatchingSe()->getSamRevision(),
                          mSessionEncryption,
                          mPoDigestDataCache[i]);
        auto cmd = std::reinterpret_pointer_cast<AbstractSamCommandResponse>(update);
        samCommands.push_back(cmd);
    }

    /* clears cached commands once they have been processed */
    mPoDigestDataCache.clear();

    if (addDigestClose) {
        /* Build and append Digest Close command */
        auto close = std::make_shared<DigestCloseCmdBuild>(
                        mSamResource->getMatchingSe()->getSamRevision(),
                        mPoResource->getMatchingSe()->getRevision() == PoRevision::REV3_2 ?
                            SIGNATURE_LENGTH_REV32 : SIGNATURE_LENGTH_REV_INF_32);
        auto cmd = std::reinterpret_pointer_cast<AbstractSamCommandResponse>(close);
        samCommands.push_back(cmd);
    }

    return samCommands;
}

const std::vector<uint8_t> SamCommandProcessor::getTerminalSignature()
{
    /*
     * All remaining SAM digest operations will now run at once.
     * Get the SAM Digest request including Digest Close from the cache manager
     */
    std::vector<std::shared_ptr<AbstractSamCommandResponse>> samCommands =
        getPendingSamCommands(true);

    auto samSeRequest = std::make_shared<SeRequest>(getApduRequests(samCommands));

    /* Transmit SeRequest and get SeResponse */
    std::shared_ptr<SeResponse> samSeResponse;

    try {
        samSeResponse = mSamReader->transmitSeRequest(samSeRequest, ChannelControl::KEEP_OPEN);
    } catch (const KeypleReaderIOException& e) {
        throw CalypsoSamIOException("SAM IO Exception while transmitting digest data.", e);
    }

    const std::vector<std::shared_ptr<ApduResponse>>& samApduResponses =
        samSeResponse->getApduResponses();

    if (samApduResponses.size() != samCommands.size()) {
        std::stringstream ss;
        ss << "The number of commands/responses does not match: cmd=" << samCommands.size()
           << ", resp=" << samApduResponses.size();
        throw CalypsoDesynchronizedExchangesException(ss.str());
    }

    /* Check all responses status */
    for (int i = 0; i < static_cast<int>(samApduResponses.size()); i++)
        samCommands[i]->createResponseParser(samApduResponses[i])->checkStatus();

    /* Get Terminal Signature from the latest response */
    std::shared_ptr<AbstractSamResponseParser> response =
        samCommands[samCommands.size() - 1]->createResponseParser(
            samApduResponses[samCommands.size() - 1]);
    auto digestCloseRespPars = std::reinterpret_pointer_cast<DigestCloseRespPars>(response);

    const std::vector<uint8_t>& sessionTerminalSignature = digestCloseRespPars->getSignature();

    mLogger->debug("SIGNATURE = %\n", sessionTerminalSignature);

    return sessionTerminalSignature;
}

void SamCommandProcessor::authenticatePoSignature(
    const std::vector<uint8_t>& poSignatureLo)
{
    /*
     * Check the PO signature part with the SAM
     * Build and send SAM Digest Authenticate command
     */
    auto digestAuthenticateCmdBuild =
        std::make_shared<DigestAuthenticateCmdBuild>(mSamResource->getMatchingSe()->getSamRevision(),
                                                     poSignatureLo);

    std::vector<std::shared_ptr<ApduRequest>> samApduRequests;
    samApduRequests.push_back(digestAuthenticateCmdBuild->getApduRequest());

    auto samSeRequest = std::make_shared<SeRequest>(samApduRequests);
    std::shared_ptr<SeResponse> samSeResponse;

    try {
        samSeResponse = mSamReader->transmitSeRequest(samSeRequest, ChannelControl::KEEP_OPEN);
    } catch (const KeypleReaderIOException& e) {
        throw CalypsoSamIOException("SAM IO Exception while transmitting digest authentication " \
                                    "data.",
                                    e);
    }

    /* Get transaction result parsing the response */
    const std::vector<std::shared_ptr<ApduResponse>>& samApduResponses =
        samSeResponse->getApduResponses();

    if (samApduResponses.empty())
        throw CalypsoDesynchronizedExchangesException("No response to Digest Auth. command.");

    std::shared_ptr<DigestAuthenticateRespPars> digestAuthenticateRespPars =
        digestAuthenticateCmdBuild->createResponseParser(samApduResponses[0]);

    digestAuthenticateRespPars->checkStatus();
}

const std::vector<std::shared_ptr<ApduRequest>> SamCommandProcessor::getApduRequests(
    const std::vector<std::shared_ptr<AbstractSamCommandResponse>>& samCommands)
{
    std::vector<std::shared_ptr<ApduRequest>> apduRequests;

    if (!samCommands.empty()) {
        for (const auto& commandBuilder : samCommands)
            apduRequests.push_back(commandBuilder->getApduRequest());
    }

    return apduRequests;
}

const std::vector<uint8_t> SamCommandProcessor::getCipheredPinData(
    const std::vector<uint8_t>& poChallenge,
    const std::vector<uint8_t>& currentPin,
    const std::vector<uint8_t> newPin)
{
    std::vector<std::shared_ptr<AbstractSamCommandResponse>> samCommands;
    std::shared_ptr<KeyReference> pinCipheringKey;

    if (mWorkKeyKif != 0) {
        /* The current work key has been set (a secure session is open) */
        pinCipheringKey = std::make_shared<KeyReference>(mWorkKeyKif, mWorkKeyKVC);
    } else {
        /* No current work key is available (outside secure session) */
        pinCipheringKey = mPoSecuritySettings->getDefaultPinCipheringKey();
    }

    if (!mIsDiversificationDone) {
        /* Build the SAM Select Diversifier command to provide the SAM with the PO S/N */
        auto select = std::make_shared<SelectDiversifierCmdBuild>(
                          mSamResource->getMatchingSe()->getSamRevision(),
                          mPoResource->getMatchingSe()->getApplicationSerialNumberBytes());
        auto cmd = std::reinterpret_pointer_cast<AbstractSamCommandResponse>(select);
        samCommands.push_back(cmd);
        mIsDiversificationDone = true;
    }

    if (mIsDigesterInitialized) {
        /* Get the pending SAM ApduRequest and add it to the current ApduRequest list */
        const std::vector<std::shared_ptr<AbstractSamCommandResponse>> pending =
            getPendingSamCommands(false);
        samCommands.insert(std::end(samCommands), std::begin(pending), std::end(pending));
    }

    auto random = std::make_shared<GiveRandomCmdBuild>(
                      mSamResource->getMatchingSe()->getSamRevision(),
                      poChallenge);
    auto cmd = std::reinterpret_pointer_cast<AbstractSamCommandResponse>(random);
    samCommands.push_back(cmd);


    int cardCipherPinCmdIndex = samCommands.size();

    auto cardCipherPinCmdBuild = std::make_shared<CardCipherPinCmdBuild>(
                                     mSamResource->getMatchingSe()->getSamRevision(),
                                     pinCipheringKey,
                                     currentPin,
                                     newPin);
    auto cipher = std::make_shared<CardCipherPinCmdBuild>(
                      mSamResource->getMatchingSe()->getSamRevision(),
                      pinCipheringKey,
                      currentPin,
                      newPin);
    cmd = std::reinterpret_pointer_cast<AbstractSamCommandResponse>(cipher);
    samCommands.push_back(cmd);

    /* Build a SAM SeRequest */
    auto samSeRequest = std::make_shared<SeRequest>(getApduRequests(samCommands));

    /* Execute the command */
    std::shared_ptr<SeResponse> samSeResponse =
        mSamReader->transmitSeRequest(samSeRequest, ChannelControl::KEEP_OPEN);

    std::shared_ptr<ApduResponse> cardCipherPinResponse =
        samSeResponse->getApduResponses()[cardCipherPinCmdIndex];

    /* Create a parser */
    std::shared_ptr<CardCipherPinRespPars> cardCipherPinRespPars =
            cardCipherPinCmdBuild->createResponseParser(cardCipherPinResponse);

    cardCipherPinRespPars->checkStatus();

    return cardCipherPinRespPars->getCipheredData();
}

const std::vector<uint8_t> SamCommandProcessor::getSvComplementaryData(
    std::shared_ptr<AbstractSamCommandResponse> svPrepareCmdBuild)
{
    std::vector<std::shared_ptr<AbstractSamCommandResponse>> samCommands;

    if (!mIsDiversificationDone) {
        /* Build the SAM Select Diversifier command to provide the SAM with the PO S/N */
        auto select = std::make_shared<SelectDiversifierCmdBuild>(
                          mSamResource->getMatchingSe()->getSamRevision(),
                          mPoResource->getMatchingSe()->getApplicationSerialNumberBytes());
        auto cmd = std::reinterpret_pointer_cast<AbstractSamCommandResponse>(select);
        samCommands.push_back(cmd);
        mIsDiversificationDone = true;
    }

    if (mIsDigesterInitialized) {
        /* Get the pending SAM ApduRequest and add it to the current ApduRequest list */
        const std::vector<std::shared_ptr<AbstractSamCommandResponse>> pending =
            getPendingSamCommands(false);
        samCommands.insert(std::end(samCommands), std::begin(pending), std::end(pending));
    }

    const int svPrepareOperationCmdIndex = samCommands.size();

    samCommands.push_back(svPrepareCmdBuild);

    /* Build a SAM SeRequest */
    auto samSeRequest = std::make_shared<SeRequest>(getApduRequests(samCommands));

    /* Execute the command */
    std::shared_ptr<SeResponse> samSeResponse =
        mSamReader->transmitSeRequest(samSeRequest, ChannelControl::KEEP_OPEN);

    std::shared_ptr<ApduResponse> svPrepareResponse =
        samSeResponse->getApduResponses()[svPrepareOperationCmdIndex];

    /* Create a parser */
    auto svPrepareOperationRespPars =
        std::dynamic_pointer_cast<SvPrepareOperationRespPars>(
            svPrepareCmdBuild->createResponseParser(svPrepareResponse));

    svPrepareOperationRespPars->checkStatus();

    const std::vector<uint8_t>& samId = mSamResource->getMatchingSe()->getSerialNumber();
    const std::vector<uint8_t>& prepareOperationData =
        svPrepareOperationRespPars->getApduResponse()->getDataOut();

    std::vector<uint8_t> operationComplementaryData(samId.size() + prepareOperationData.size());
    System::arraycopy(samId, 0, operationComplementaryData, 0, samId.size());
    System::arraycopy(prepareOperationData,
                      0,
                      operationComplementaryData,
                      samId.size(),
                      prepareOperationData.size());

    return operationComplementaryData;
}

const std::vector<uint8_t> SamCommandProcessor::getSvReloadComplementaryData(
    std::shared_ptr<SvReloadCmdBuild> svReloadCmdBuild,
    const std::vector<uint8_t>& svGetHeader,
    const std::vector<uint8_t>& svGetData)
{
    /* Get the complementary data from the SAM */
    auto svPrepareLoadCmdBuild = std::make_shared<SvPrepareLoadCmdBuild>(
                                     mSamResource->getMatchingSe()->getSamRevision(),
                                     svGetHeader,
                                     svGetData,
                                     svReloadCmdBuild->getSvReloadData());
    auto cmd = std::reinterpret_pointer_cast<AbstractSamCommandResponse>(svPrepareLoadCmdBuild);

    return getSvComplementaryData(cmd);
}

const std::vector<uint8_t> SamCommandProcessor::getSvDebitComplementaryData(
    std::shared_ptr<SvDebitCmdBuild> svDebitCmdBuild,
    const std::vector<uint8_t>& svGetHeader,
    const std::vector<uint8_t>& svGetData)
{
    /* Get the complementary data from the SAM */
    auto svPrepareDebitCmdBuild = std::make_shared<SvPrepareDebitCmdBuild>(
                                      mSamResource->getMatchingSe()->getSamRevision(),
                                      svGetHeader,
                                      svGetData,
                                      svDebitCmdBuild->getSvDebitData());
    auto cmd = std::reinterpret_pointer_cast<AbstractSamCommandResponse>(svPrepareDebitCmdBuild);

    return getSvComplementaryData(cmd);
}

const std::vector<uint8_t> SamCommandProcessor::getSvUndebitComplementaryData(
    std::shared_ptr<SvUndebitCmdBuild> svUndebitCmdBuild,
    const std::vector<uint8_t>& svGetHeader,
    const std::vector<uint8_t>& svGetData)
{
    /* Get the complementary data from the SAM */
    auto svPrepareUndebitCmdBuild = std::make_shared<SvPrepareUndebitCmdBuild>(
                                        mSamResource->getMatchingSe()->getSamRevision(),
                                        svGetHeader,
                                        svGetData,
                                        svUndebitCmdBuild->getSvUndebitData());
    auto cmd = std::reinterpret_pointer_cast<AbstractSamCommandResponse>(svPrepareUndebitCmdBuild);

    return getSvComplementaryData(cmd);
}

void SamCommandProcessor::checkSvStatus(const std::vector<uint8_t>& svOperationResponseData)
{
    std::vector<std::shared_ptr<AbstractSamCommandResponse>> samCommands;

    auto svCheckCmdBuilder = std::make_shared<SvCheckCmdBuild>(
                                 mSamResource->getMatchingSe()->getSamRevision(),
                                 svOperationResponseData);
    auto cmd = std::reinterpret_pointer_cast<AbstractSamCommandResponse>(svCheckCmdBuilder);
    samCommands.push_back(cmd);

    /* Build a SAM SeRequest */
    auto samSeRequest = std::make_shared<SeRequest>(getApduRequests(samCommands));

    /* Execute the command */
    std::shared_ptr<SeResponse> samSeResponse =
        mSamReader->transmitSeRequest(samSeRequest, ChannelControl::KEEP_OPEN);

    std::shared_ptr<ApduResponse> svCheckResponse = samSeResponse->getApduResponses()[0];

    /* Create a parser */
    std::shared_ptr<SvCheckRespPars> svCheckRespPars =
        svCheckCmdBuilder->createResponseParser(svCheckResponse);

    svCheckRespPars->checkStatus();
}


}
}
}
