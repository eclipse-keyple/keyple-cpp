/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

#include "AbstractOpenSessionRespPars.h"

#include <typeinfo>

/* Calypso */
#include "AbstractPoResponseParser.h"
#include "AbstractApduResponseParser.h"
#include "ApduResponse.h"
#include "CalypsoPoAccessForbiddenException.h"
#include "CalypsoPoDataAccessException.h"
#include "CalypsoPoIllegalParameterException.h"
#include "CalypsoPoSecurityContextException.h"
#include "CalypsoPoTerminatedException.h"
#include "OpenSession10RespPars.h"
#include "OpenSession24RespPars.h"
#include "OpenSession31RespPars.h"
#include "OpenSession32RespPars.h"

/* Common */
#include "ClassNotFoundException.h"
#include "IllegalAccessException.h"
#include "stringhelper.h"

/* Core */
#include "ByteArrayUtil.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::po::exception;
using namespace keyple::common;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::util;

using StatusProperties = AbstractApduResponseParser::StatusProperties;
using SecureSession = AbstractOpenSessionRespPars::SecureSession;

/* ABSTRACT OPEN SESSION RESP PARS ------------------------------------------ */

const std::map<int, std::shared_ptr<StatusProperties>>
    AbstractOpenSessionRespPars::STATUS_TABLE = {
    {
        0x6700,
        std::make_shared<StatusProperties>(
            "Lc value not supported.",
            typeid(CalypsoPoIllegalParameterException))
    }, {
        0x6900,
        std::make_shared<StatusProperties>(
            "Transaction Counter is 0",
            typeid(CalypsoPoTerminatedException))
    }, {
        0x6981,
        std::make_shared<StatusProperties>(
            "Command forbidden (read requested and current EF is a " \
            "Binary file).",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6982,
        std::make_shared<StatusProperties>(
            "Security conditions not fulfilled (PIN code not presented," \
            " AES key forbidding the compatibility mode, encryption "  \
            "required).",
            typeid(CalypsoPoSecurityContextException))
    }, {
        0x6985,
        std::make_shared<StatusProperties>(
            "Access forbidden (Never access mode, Session already " \
            "opened).",
            typeid(CalypsoPoAccessForbiddenException))
    }, {
        0x6986,
        std::make_shared<StatusProperties>(
            "Command not allowed (read requested and no current EF).",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6A81,
        std::make_shared<StatusProperties>(
            "Wrong key index.",
            typeid(CalypsoPoIllegalParameterException))
    }, {
        0x6A82,
        std::make_shared<StatusProperties>(
            "File not found.",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6A83,
        std::make_shared<StatusProperties>(
            "Record not found (record index is above NumRec).",
            typeid(CalypsoPoDataAccessException))
    }, {
        0x6B00,
        std::make_shared<StatusProperties>(
            "P1 or P2 value not supported (key index incorrect, wrong " \
            "P2).",
            typeid(CalypsoPoIllegalParameterException))
    }, {
        0x61FF,
        std::make_shared<StatusProperties>(
            "Correct execution (ISO7816 T=0).",
            typeid(ClassNotFoundException))
    }
};

const std::map<int, std::shared_ptr<StatusProperties>>&
    AbstractOpenSessionRespPars::getStatusTable() const
{
    // At this stage, the status table is the same for everyone
    return STATUS_TABLE;
}

AbstractOpenSessionRespPars::AbstractOpenSessionRespPars(
  std::shared_ptr<ApduResponse> response,
  AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>* builder,
  PoRevision revision)
: AbstractPoResponseParser(
    response,
    dynamic_cast<AbstractPoCommandBuilder<AbstractPoResponseParser>*>(builder))
{
    (void)revision;

    const std::vector<uint8_t> dataOut = response->getDataOut();
    if (dataOut.size())
        mSecureSession = toSecureSession(dataOut);
}

std::shared_ptr<SecureSession> AbstractOpenSessionRespPars::toSecureSession(
    const std::vector<uint8_t>& apduResponseData)
{
    (void)apduResponseData;

    throw IllegalAccessException(
              "This function should be overwritten by derived class");
}

std::shared_ptr<AbstractOpenSessionRespPars>
AbstractOpenSessionRespPars::create(std::shared_ptr<ApduResponse> response,
                                    PoRevision revision)
{
    switch (revision) {
    case PoRevision::REV1_0:
        return std::make_shared<OpenSession10RespPars>(
                   response,
                   dynamic_cast<OpenSession10CmdBuild*>(mBuilder));
    case PoRevision::REV2_4:
        return std::make_shared<OpenSession24RespPars>(
                   response,
                   dynamic_cast<OpenSession24CmdBuild*>(mBuilder));
    case PoRevision::REV3_1:
    case PoRevision::REV3_1_CLAP:
        return std::make_shared<OpenSession31RespPars>(
                   response,
                   dynamic_cast<OpenSession31CmdBuild*>(mBuilder));
    case PoRevision::REV3_2:
        return std::make_shared<OpenSession32RespPars>(
                   response,
                   dynamic_cast<OpenSession32CmdBuild*>(mBuilder));
    default:
        throw std::invalid_argument(StringHelper::formatSimple(
            "Unknow revision %d", static_cast<int>(revision)));
    }
}

const std::vector<uint8_t>& AbstractOpenSessionRespPars::getPoChallenge() const
{
    return mSecureSession->getChallengeRandomNumber();
}

int AbstractOpenSessionRespPars::getTransactionCounterValue() const
{
    return ByteArrayUtil::threeBytesToInt(
               mSecureSession->getChallengeTransactionCounter(), 0);
}

bool AbstractOpenSessionRespPars::wasRatified() const
{
    return mSecureSession->isPreviousSessionRatified();
}

bool AbstractOpenSessionRespPars::isManageSecureSessionAuthorized() const
{
    return mSecureSession->isManageSecureSessionAuthorized();
}

uint8_t AbstractOpenSessionRespPars::getSelectedKif() const
{
    return mSecureSession->getKIF();
}

std::uint8_t AbstractOpenSessionRespPars::getSelectedKvc() const
{
    return mSecureSession->getKVC();
}

const std::vector<uint8_t>& AbstractOpenSessionRespPars::getRecordDataRead()
   const
{
    return mSecureSession->getOriginalData();
}

/* SECURE SESSION ----------------------------------------------------------- */

SecureSession::SecureSession(
  const std::vector<uint8_t>& challengeTransactionCounter,
  const std::vector<uint8_t>& challengeRandomNumber,
  const bool previousSessionRatified,
  const bool manageSecureSessionAuthorized,
  const uint8_t kif,
  const uint8_t kvc,
  const std::vector<uint8_t>& originalData,
  const std::vector<uint8_t>& secureSessionData)
: mChallengeTransactionCounter(challengeTransactionCounter),
  mChallengeRandomNumber(challengeRandomNumber),
  mPreviousSessionRatified(previousSessionRatified),
  mManageSecureSessionAuthorized(manageSecureSessionAuthorized),
  mKif(kif),
  mKvc(kvc),
  mOriginalData(originalData),
  mSecureSessionData(secureSessionData) {}

SecureSession::SecureSession(
  const std::vector<uint8_t>& challengeTransactionCounter,
  const std::vector<uint8_t>& challengeRandomNumber,
  const bool previousSessionRatified,
  const bool manageSecureSessionAuthorized,
  const uint8_t kvc,
  const std::vector<uint8_t>& originalData,
  const std::vector<uint8_t>& secureSessionData)
: SecureSession(challengeTransactionCounter,
                challengeRandomNumber,
                previousSessionRatified,
                manageSecureSessionAuthorized,
                0xff,
                kvc,
                originalData,
                secureSessionData) {}

SecureSession::SecureSession(
  const std::vector<uint8_t>& challengeTransactionCounter,
  const std::vector<uint8_t>& challengeRandomNumber,
  const bool previousSessionRatified,
  const bool manageSecureSessionAuthorized,
  const std::vector<uint8_t>& originalData,
  const std::vector<uint8_t>& secureSessionData)
: SecureSession(challengeTransactionCounter,
                challengeRandomNumber,
                previousSessionRatified,
                manageSecureSessionAuthorized,
                0xff,
                0xff,
                originalData,
                secureSessionData) {}

const std::vector<uint8_t>& SecureSession::getChallengeTransactionCounter()
    const
{
    return mChallengeTransactionCounter;
}

const std::vector<uint8_t>& SecureSession::getChallengeRandomNumber() const
{
    return mChallengeRandomNumber;
}

bool SecureSession::isPreviousSessionRatified()
    const
{
    return mPreviousSessionRatified;
}

bool SecureSession::isManageSecureSessionAuthorized() const
{
    return mManageSecureSessionAuthorized;
}

uint8_t SecureSession::getKIF() const
{
    return mKif;
}
uint8_t SecureSession::getKVC() const
{
    return mKvc;
}

const std::vector<uint8_t>& SecureSession::getOriginalData() const
{
    return mOriginalData;
}

const std::vector<uint8_t>& SecureSession::getSecureSessionData() const
{
    return mSecureSessionData;
}

}
}
}
}
}
}
