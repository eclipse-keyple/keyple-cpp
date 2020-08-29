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
#include "OpenSession10RespPars.h"
#include "OpenSession24RespPars.h"
#include "OpenSession31RespPars.h"
#include "OpenSession32RespPars.h"

/* Common */
#include "ClassNotFoundException.h"
#include "stringhelper.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace security {

using namespace keyple::calypso::command::po;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

std::unordered_map<int, std::shared_ptr<StatusProperties>>
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

std::unordered_map<int, std::shared_ptr<StatusProperties>>
    AbstractOpenSessionRespPars::getStatusTable() const
{
    // At this stage, the status table is the same for everyone
    return STATUS_TABLE;
}

AbstractOpenSessionRespPars::AbstractOpenSessionRespPars(
  std::shared_ptr<ApduResponse> response,
  AbstractOpenSessionCmdBuild<AbstractOpenSessionRespPars>* builder,
  PoRevision revision)
: AbstractPoResponseParser(response, builder)
{
    const std::vector<uint8_t> dataOut = response.getDataOut();
    if (dataOut.length())
        mSecureSession = toSecureSession(dataOut);
}

std::shared_ptr<AbstractOpenSessionRespPars>
AbstractOpenSessionRespPars::create(std::shared_ptr<ApduResponse> response,
                                    PoRevision revision)
{
    switch (revision) {
    case PoRevision::REV1_0:
        return std::make_shared<OpenSession10RespPars>(response, mBuilder);
    case PoRevision::REV2_4:
        return std::make_shared<OpenSession24RespPars>(response, mBuilder);
    case PoRevision::REV3_1:
    case PoRevision::REV3_1_CLAP:
        return std::make_shared<OpenSession31RespPars>(response, mBuilder);
    case PoRevision::REV3_2:
        return std::make_shared<OpenSession32RespPars>(response, mBuilder);
    default:
        throw std::invalid_argument(StringHelper::formatSimple(
            "Unknow revision %d", static_cast<int>(revision)));
    }
}

const std::vector<uint8_t>& AbstractOpenSessionRespPars::getPoChallenge()
{
    return mSecureSession->getChallengeRandomNumber();
}

int AbstractOpenSessionRespPars::getTransactionCounterValue()
{
    return ByteArrayUtil::threeBytesToInt(
               mSecureSession->getChallengeTransactionCounter(), 0);
}

bool AbstractOpenSessionRespPars::wasRatified()
{
    return mSecureSession->isPreviousSessionRatified();
}

bool AbstractOpenSessionRespPars::isManageSecureSessionAuthorized()
{
    return mSecureSession->isManageSecureSessionAuthorized();
}

char AbstractOpenSessionRespPars::getSelectedKif()
{
    return mSecureSession->getKIF();
}

std::shared_ptr<Byte> AbstractOpenSessionRespPars::getSelectedKvc()
{
    return mSecureSession->getKVC();
}

const std::vector<uint8_t>& AbstractOpenSessionRespPars::getRecordDataRead()
{
    return mSecureSession->getOriginalData();
}

AbstractOpenSessionRespPars::SecureSession::SecureSession(
    const std::vector<uint8_t>& challengeTransactionCounter,
    const std::vector<uint8_t>& challengeRandomNumber,
    bool previousSessionRatified, bool manageSecureSessionAuthorized,
    uint8_t kif, std::shared_ptr<Byte> kvc,
    const std::vector<uint8_t>& originalData,
    const std::vector<uint8_t>& secureSessionData)
: challengeTransactionCounter(challengeTransactionCounter),
  challengeRandomNumber(challengeRandomNumber),
  previousSessionRatified(previousSessionRatified),
  manageSecureSessionAuthorized(manageSecureSessionAuthorized), kif(kif),
  kvc(kvc), originalData(originalData), secureSessionData(secureSessionData)
{
}

AbstractOpenSessionRespPars::SecureSession::SecureSession(
    const std::vector<uint8_t>& challengeTransactionCounter,
    const std::vector<uint8_t>& challengeRandomNumber,
    bool previousSessionRatified, bool manageSecureSessionAuthorized,
    std::shared_ptr<Byte> kvc, const std::vector<uint8_t>& originalData,
    const std::vector<uint8_t>& secureSessionData)
: challengeTransactionCounter(challengeTransactionCounter),
  challengeRandomNumber(challengeRandomNumber),
  previousSessionRatified(previousSessionRatified),
  manageSecureSessionAuthorized(manageSecureSessionAuthorized),
  kif(0xff), kvc(kvc != nullptr ? kvc : std::make_shared<Byte>(0xff)),
  originalData(originalData), secureSessionData(secureSessionData)
{
}

AbstractOpenSessionRespPars::SecureSession::SecureSession(
    const std::vector<uint8_t>& challengeTransactionCounter,
    const std::vector<uint8_t>& challengeRandomNumber,
    bool previousSessionRatified, bool manageSecureSessionAuthorized,
    std::shared_ptr<Byte> kvc, const std::vector<uint8_t>& secureSessionData)
: challengeTransactionCounter(challengeTransactionCounter),
  challengeRandomNumber(challengeRandomNumber),
  previousSessionRatified(previousSessionRatified),
  manageSecureSessionAuthorized(manageSecureSessionAuthorized),
  kif(0xff), kvc(kvc != nullptr ? kvc : std::make_shared<Byte>(0xff)),
  secureSessionData(secureSessionData)
{
}

const std::vector<uint8_t>&
AbstractOpenSessionRespPars::SecureSession::getChallengeTransactionCounter()
{
    return challengeTransactionCounter;
}

const std::vector<uint8_t>&
AbstractOpenSessionRespPars::SecureSession::getChallengeRandomNumber()
{
    return challengeRandomNumber;
}

bool AbstractOpenSessionRespPars::SecureSession::isPreviousSessionRatified()
{
    return previousSessionRatified;
}

bool AbstractOpenSessionRespPars::SecureSession::
    isManageSecureSessionAuthorized()
{
    return manageSecureSessionAuthorized;
}

uint8_t AbstractOpenSessionRespPars::SecureSession::getKIF()
{
    return kif;
}

std::shared_ptr<Byte> AbstractOpenSessionRespPars::SecureSession::getKVC()
{
    return kvc;
}

const std::vector<uint8_t>&
AbstractOpenSessionRespPars::SecureSession::getOriginalData()
{
    return originalData;
}

const std::vector<uint8_t>&
AbstractOpenSessionRespPars::SecureSession::getSecureSessionData()
{
    return secureSessionData;
}

}
}
}
}
}
}
