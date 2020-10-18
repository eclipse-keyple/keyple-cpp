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

#include "SamCommandProcessor.h"

/* Calypso */
#include "CalypsoDesynchronizedExchangesException.h"
#include "CalypsoPo.h"
#include "CalypsoSamIOException.h"
#include "DigestAuthenticateCmdBuild.h"
#include "DigestAuthenticateRespPars.h"
#include "DigestCloseCmdBuild.h"
#include "DigestCloseRespPars.h"
#include "DigestInitCmdBuild.h"
#include "DigestUpdateCmdBuild.h"
#include "PoSecuritySettings.h"
#include "SamGetChallengeCmdBuild.h"
#include "SamGetChallengeRespPars.h"
#include "SelectDiversifierCmdBuild.h"

/* Common */
#include "Arrays.h"
#include "IllegalStateException.h"
#include "KeypleStd.h"

/* Core */
#include "KeypleReaderIOException.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::command::sam::builder::security;
using namespace keyple::calypso::command::sam::parser::security;
using namespace keyple::calypso::transaction::exception;
using namespace keyple::common;
using namespace keyple::core::seproxy::exception;

const uint8_t SamCommandProcessor::KIF_UNDEFINED = 0xff;
const uint8_t SamCommandProcessor::CHALLENGE_LENGTH_REV_INF_32 = 0x04;
const uint8_t SamCommandProcessor::CHALLENGE_LENGTH_REV32 = 0x08;
const uint8_t SamCommandProcessor::SIGNATURE_LENGTH_REV_INF_32 = 0x04;
const uint8_t SamCommandProcessor::SIGNATURE_LENGTH_REV32 = 0x08;

SamCommandProcessor::SamCommandProcessor(
  const std::shared_ptr<SeResource<CalypsoPo>> poResource,
  const std::shared_ptr<PoSecuritySettings> poSecuritySettings)
: mSamResource(poSecuritySettings->getSamResource()),
  mSamReader(
      std::dynamic_pointer_cast<ProxyReader>(mSamResource->getSeReader())),
  mPoResource(poResource),
  mPoSecuritySettings(poSecuritySettings) {}

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

    std::shared_ptr<AbstractSamCommandBuilder<AbstractSamResponseParser>>
        getChallengeCmdBuild =
            reinterpret_pointer_cast<
                AbstractSamCommandBuilder<AbstractSamResponseParser>>(
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
    mWorkKeyRecordNumber =
        mPoSecuritySettings->getSessionDefaultKeyRecordNumber(accessLevel);
    mWorkKeyKif = determineWorkKif(poKif, accessLevel);
    /* TODO handle Rev 1.0 case where KVC is not available */
    mWorkKeyKVC = poKVC;

    mLogger->debug("initialize: POREVISION = %, SAMREVISION = %, " \
                   "SESSIONENCRYPTION = %, VERIFICATIONMODE = %\n",
                   mPoResource->getMatchingSe()->getRevision(),
                   mSamResource->getMatchingSe()->getSamRevision(),
                   sessionEncryption,
                   verificationMode);

    mLogger->debug("initialize: VERIFICATIONMODE = %, REV32MODE = % " \
                   ", KEYRECNUMBER = %\n",
                   verificationMode,
                   mPoResource->getMatchingSe()
                       ->isConfidentialSessionModeSupported(),
                   mWorkKeyRecordNumber);

    mLogger->debug("initialize: KIF = %, KVC = %, DIGESTDATA = %\n",
                   poKif,
                   poKVC,
                   digestData);

    /* Clear data cache */
    mPoDigestDataCache.clear();

    /*
     * Build Digest Init command as first ApduRequest of the digest computation
     * process
     */
    mPoDigestDataCache.push_back(digestData);

    mIsDigestInitDone = false;
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

const std::vector<std::shared_ptr<AbstractSamCommandBuilder<
    AbstractSamResponseParser>>> SamCommandProcessor::getPendingSamCommands(
        const bool addDigestClose)
{
    /*
     * TODO optimization with the use of Digest Update Multiple whenever
     * possible.
     */
    std::vector<std::shared_ptr<AbstractSamCommandBuilder<
        AbstractSamResponseParser>>> samCommands;

    /* Sanity checks */
    if (mPoDigestDataCache.empty()) {
        mLogger->debug("getSamDigestRequest: no data in cache.");
        throw IllegalStateException("Digest data cache is empty.");
    }

    if (!mIsDigestInitDone && mPoDigestDataCache.size() % 2 == 0) {
        /* The number of buffers should be 2*n + 1 */
        mLogger->debug(
            "getSamDigestRequest: wrong number of buffer in cache NBR = %\n.",
            mPoDigestDataCache.size());
        throw IllegalStateException("Digest data cache is inconsistent.");
    }

    if (!mIsDigestInitDone) {
        /*
         * Build and append Digest Init command as first ApduRequest of the
         * digest computation process. The Digest Init command comes from the
         * Open Secure Session response from the PO. Once added to the
         * ApduRequest list, the data is remove from the cache to keep only
         * couples of PO request/response
         */
        samCommands.push_back(
            reinterpret_pointer_cast<
                AbstractSamCommandBuilder<AbstractSamResponseParser>>(
                    std::make_shared<DigestInitCmdBuild>(
                        mSamResource->getMatchingSe()->getSamRevision(),
                        mVerificationMode,
                        mPoResource->getMatchingSe()
                            ->isConfidentialSessionModeSupported(),
                        mWorkKeyRecordNumber,
                        mWorkKeyKif,
                        mWorkKeyKVC,
                        mPoDigestDataCache[0])));

        mPoDigestDataCache.erase(mPoDigestDataCache.begin());

        /* Note that the digest init has been made */
        mIsDigestInitDone = true;
    }

    /* Build and append Digest Update commands */
    for (int i = 0; i < static_cast<int>(mPoDigestDataCache.size()); i++)
        samCommands.push_back(
            reinterpret_pointer_cast<
                AbstractSamCommandBuilder<AbstractSamResponseParser>>(
                    std::make_shared<DigestUpdateCmdBuild>(
                        mSamResource->getMatchingSe()->getSamRevision(),
                        mSessionEncryption,
                        mPoDigestDataCache[i])));

    /* clears cached commands once they have been processed */
    mPoDigestDataCache.clear();

    if (addDigestClose)
        /* Build and append Digest Close command */
        samCommands.push_back(
            reinterpret_pointer_cast<
                AbstractSamCommandBuilder<AbstractSamResponseParser>>(
                    std::make_shared<DigestCloseCmdBuild>(
                        mSamResource->getMatchingSe()->getSamRevision(),
                        mPoResource->getMatchingSe()->getRevision() ==
                            PoRevision::REV3_2 ?
                            SIGNATURE_LENGTH_REV32 :
                            SIGNATURE_LENGTH_REV_INF_32)));


    return samCommands;
}

const std::vector<uint8_t> SamCommandProcessor::getTerminalSignature()
{
    /*
     * All remaining SAM digest operations will now run at once.
     * Get the SAM Digest request including Digest Close from the cache manager
     */
    std::vector<std::shared_ptr<AbstractSamCommandBuilder<
        AbstractSamResponseParser>>> samCommands = getPendingSamCommands(true);

    std::shared_ptr<SeRequest> samSeRequest =
        std::make_shared<SeRequest>(getApduRequests(samCommands));

    /* Transmit SeRequest and get SeResponse */
    std::shared_ptr<SeResponse> samSeResponse;

    try {
        samSeResponse = mSamReader->transmitSeRequest(
                            samSeRequest, ChannelControl::KEEP_OPEN);
    } catch (const KeypleReaderIOException& e) {
        throw CalypsoSamIOException(
                  "SAM IO Exception while transmitting digest data.", e);
    }

    const std::vector<std::shared_ptr<ApduResponse>>& samApduResponses =
        samSeResponse->getApduResponses();

    if (samApduResponses.size() != samCommands.size()) {
        std::stringstream ss;
        ss << "The number of commands/responses does not "
           << "match: cmd=" << samCommands.size() << ", resp="
           << samApduResponses.size();
        throw CalypsoDesynchronizedExchangesException(ss.str());
    }

    /* Check all responses status */
    for (int i = 0; i < static_cast<int>(samApduResponses.size()); i++)
        samCommands[i]->createResponseParser(samApduResponses[i])
            ->checkStatus();

    /* Get Terminal Signature from the latest response */
    std::shared_ptr<DigestCloseRespPars> digestCloseRespPars =
        std::dynamic_pointer_cast<DigestCloseRespPars>(
            samCommands[samCommands.size() - 1]->createResponseParser(
                samApduResponses[samCommands.size() - 1]));

    const std::vector<uint8_t>& sessionTerminalSignature =
        digestCloseRespPars->getSignature();

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
    std::shared_ptr<DigestAuthenticateCmdBuild> digestAuthenticateCmdBuild =
        std::make_shared<DigestAuthenticateCmdBuild>(
            mSamResource->getMatchingSe()->getSamRevision(), poSignatureLo);

    std::vector<std::shared_ptr<ApduRequest>> samApduRequests;
    samApduRequests.push_back(digestAuthenticateCmdBuild->getApduRequest());

    std::shared_ptr<SeRequest> samSeRequest =
        std::make_shared<SeRequest>(samApduRequests);
    std::shared_ptr<SeResponse> samSeResponse;

    try {
        samSeResponse = mSamReader->transmitSeRequest(
                            samSeRequest, ChannelControl::KEEP_OPEN);
    } catch (const KeypleReaderIOException& e) {
        throw CalypsoSamIOException("SAM IO Exception while transmitting " \
                                    "digest authentication data.",
                                    e);
    }

    /* Get transaction result parsing the response */
    const std::vector<std::shared_ptr<ApduResponse>>& samApduResponses =
        samSeResponse->getApduResponses();

    if (samApduResponses.empty())
        throw CalypsoDesynchronizedExchangesException(
                  "No response to Digest Authenticate command.");

    std::shared_ptr<DigestAuthenticateRespPars> digestAuthenticateRespPars =
        digestAuthenticateCmdBuild->createResponseParser(samApduResponses[0]);

    digestAuthenticateRespPars->checkStatus();
}

const std::vector<std::shared_ptr<ApduRequest>>
    SamCommandProcessor::getApduRequests(
        const std::vector<std::shared_ptr<AbstractSamCommandBuilder<
            AbstractSamResponseParser>>>& samCommands)
{
    std::vector<std::shared_ptr<ApduRequest>> apduRequests;

    if (!samCommands.empty()) {
        for (const auto& commandBuilder : samCommands)
            apduRequests.push_back(commandBuilder->getApduRequest());
    }

    return apduRequests;
}

}
}
}
