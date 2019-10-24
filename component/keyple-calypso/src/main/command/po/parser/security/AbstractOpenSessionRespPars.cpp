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

#include "AbstractPoResponseParser.h"
#include "AbstractOpenSessionRespPars.h"
#include "AbstractApduResponseParser_Import.h"
#include "ApduResponse.h"
#include "OpenSession10RespPars.h"
#include "OpenSession24RespPars.h"
#include "OpenSession31RespPars.h"
#include "OpenSession32RespPars.h"

/* Common */
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

std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> AbstractOpenSessionRespPars::STATUS_TABLE;

AbstractOpenSessionRespPars::StaticConstructor::StaticConstructor() {
    std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> m(AbstractApduResponseParser::STATUS_TABLE);
    m.emplace(0x6700, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Lc value not supported."));
    m.emplace(0x6900, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Transaction Counter is 0"));
    m.emplace(0x6981, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Command forbidden (read requested and current EF is a Binary file)."));
    m.emplace(0x6982, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, std::string("Security conditions not fulfilled (PIN code not presented, AES key forbidding the ") + "compatibility mode, encryption required)."));
    m.emplace(0x6985, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Access forbidden (Never access mode, Session already opened)."));
    m.emplace(0x6986, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Command not allowed (read requested and no current EF)."));
    m.emplace(0x6A81, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Wrong key index."));
    m.emplace(0x6A82, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "File not found."));
    m.emplace(0x6A83, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "Record not found (record index is above NumRec)."));
    m.emplace(0x6B00, std::make_shared<AbstractApduResponseParser::StatusProperties>(false, "P1 or P2 value not supported (key index incorrect, wrong P2)."));
    STATUS_TABLE = m;
}

AbstractOpenSessionRespPars::StaticConstructor AbstractOpenSessionRespPars::staticConstructor;

std::unordered_map<int, std::shared_ptr<AbstractApduResponseParser::StatusProperties>> AbstractOpenSessionRespPars::getStatusTable()
{
    // At this stage, the status table is the same for everyone
    return STATUS_TABLE;
}

AbstractOpenSessionRespPars::AbstractOpenSessionRespPars(std::shared_ptr<ApduResponse> response, PoRevision revision)
: AbstractPoResponseParser(response) //, revision(revision)
{
    /* Alex: this member is (was) declared in the class but not used. Commenting it until it is necessary. */
    (void)revision;
}

std::shared_ptr<AbstractOpenSessionRespPars>
AbstractOpenSessionRespPars::create(std::shared_ptr<ApduResponse> response, PoRevision revision)
{
    switch (revision) {
        case PoRevision::REV1_0:
            return std::make_shared<OpenSession10RespPars>(response);
        case PoRevision::REV2_4:
            return std::make_shared<OpenSession24RespPars>(response);
        case PoRevision::REV3_1:
        case PoRevision::REV3_1_CLAP:
            return std::make_shared<OpenSession31RespPars>(response);
        case PoRevision::REV3_2:
            return std::make_shared<OpenSession32RespPars>(response);
        default:
            throw std::invalid_argument(StringHelper::formatSimple("Unknow revision %d", static_cast<int>(revision)));
    }
}

std::vector<char> AbstractOpenSessionRespPars::getPoChallenge()
{
    return secureSession->getChallengeRandomNumber();
}

int AbstractOpenSessionRespPars::getTransactionCounterValue()
{
    std::vector<char> counter = secureSession->getChallengeTransactionCounter();
    return counter[0] << 24 | counter[1] << 16 | counter[2] << 8 | counter[3];
}

bool AbstractOpenSessionRespPars::wasRatified() {
    return secureSession->isPreviousSessionRatified();
}

bool AbstractOpenSessionRespPars::isManageSecureSessionAuthorized()
{
    return secureSession->isManageSecureSessionAuthorized();
}

char AbstractOpenSessionRespPars::getSelectedKif()
{
    return secureSession->getKIF();
}

std::shared_ptr<Byte> AbstractOpenSessionRespPars::getSelectedKvc()
{
    return secureSession->getKVC();
}

std::vector<char> AbstractOpenSessionRespPars::getRecordDataRead()
{
    return secureSession->getOriginalData();
}

AbstractOpenSessionRespPars::SecureSession::SecureSession(
                                                    std::vector<char> &challengeTransactionCounter,
                                                    std::vector<char> &challengeRandomNumber,
                                                    bool previousSessionRatified,
                                                    bool manageSecureSessionAuthorized, char kif,
                                                    std::shared_ptr<Byte> kvc,
                                                    std::vector<char> &originalData,
                                                    std::vector<char> &secureSessionData)
: challengeTransactionCounter(challengeTransactionCounter),
  challengeRandomNumber(challengeRandomNumber),
  previousSessionRatified(previousSessionRatified),
  manageSecureSessionAuthorized(manageSecureSessionAuthorized),
  kif(kif), kvc(kvc), originalData(originalData), secureSessionData(secureSessionData)
{
}

AbstractOpenSessionRespPars::SecureSession::SecureSession(
                                                    std::vector<char> &challengeTransactionCounter,
                                                    std::vector<char> &challengeRandomNumber,
                                                    bool previousSessionRatified,
                                                    bool manageSecureSessionAuthorized,
                                                    std::shared_ptr<Byte> kvc,
                                                    std::vector<char> &originalData,
                                                    std::vector<char> &secureSessionData)
: challengeTransactionCounter(challengeTransactionCounter),
  challengeRandomNumber(challengeRandomNumber), previousSessionRatified(previousSessionRatified),
  manageSecureSessionAuthorized(manageSecureSessionAuthorized), kif(static_cast<char>(0xFF)),
  kvc(kvc), originalData(originalData), secureSessionData(secureSessionData)
{
}

std::vector<char> AbstractOpenSessionRespPars::SecureSession::getChallengeTransactionCounter()
{
    return challengeTransactionCounter;
}

std::vector<char> AbstractOpenSessionRespPars::SecureSession::getChallengeRandomNumber()
{
    return challengeRandomNumber;
}

bool AbstractOpenSessionRespPars::SecureSession::isPreviousSessionRatified()
{
    return previousSessionRatified;
}

bool AbstractOpenSessionRespPars::SecureSession::isManageSecureSessionAuthorized()
{
    return manageSecureSessionAuthorized;
}

char AbstractOpenSessionRespPars::SecureSession::getKIF()
{
    return kif;
}

std::shared_ptr<Byte> AbstractOpenSessionRespPars::SecureSession::getKVC()
{
    return kvc;
}

std::vector<char> AbstractOpenSessionRespPars::SecureSession::getOriginalData()
{
    return originalData;
}

std::vector<char> AbstractOpenSessionRespPars::SecureSession::getSecureSessionData()
{
    return secureSessionData;
}

}
}
}
}
}
}
