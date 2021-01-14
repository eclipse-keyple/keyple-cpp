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

#include "SvGetRespPars.h"

/* Calypso */
#include "CalypsoPoSecurityContextException.h"
#include "CalypsoPoSessionBufferOverflowException.h"
#include "CalypsoSamCounterOverflowException.h"
#include "CalypsoSamAccessForbiddenException.h"
#include "CalypsoPoIllegalParameterException.h"
#include "CalypsoPoSecurityDataException.h"

/* Common */
#include "ClassNotFoundException.h"
#include "IllegalStateException.h"
#include "System.h"

/* Core */
#include "ByteArrayUtil.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {
namespace parser {
namespace storedvalue {

using namespace keyple::calypso::command::po::exception;
using namespace keyple::calypso::command::sam::exception;
using namespace keyple::common;
using namespace keyple::common::exception;
using namespace keyple::core::util;

const std::map<int, std::shared_ptr<StatusProperties>> SvGetRespPars::STATUS_TABLE = {
    {
        0x6982,
        std::make_shared<StatusProperties>(
            "Security conditions not fulfilled.",
            typeid(CalypsoPoSecurityContextException)),
    }, {
        0x6985,
        std::make_shared<StatusProperties>(
            "Preconditions not satisfied (a store value operation was already done in the current" \
            " session).",
            typeid(CalypsoSamAccessForbiddenException)),
    }, {
        0x6A81,
        std::make_shared<StatusProperties>(
            "Incorrect P1 or P2.",
            typeid(CalypsoPoIllegalParameterException)),
    }, {
        0x6A86,
        std::make_shared<StatusProperties>(
            "Le inconsistent with P2.",
            typeid(CalypsoPoIllegalParameterException)),
    }, {
        0x6D00,
        std::make_shared<StatusProperties>(
            "SV function not present.",
            typeid(CalypsoPoIllegalParameterException)),
    }, {
        0x9000,
        std::make_shared<StatusProperties>("Success")
    }
};

SvGetRespPars::SvGetRespPars(const std::vector<uint8_t>& svCommandHeader,
                             std::shared_ptr<ApduResponse> response,
                             SvGetCmdBuild* builder)
: AbstractPoResponseParser(
      response,
      reinterpret_cast<AbstractPoCommandBuilder<AbstractPoResponseParser>*>(builder)),
  mSvCommandHeader(svCommandHeader)
{
    const std::vector<uint8_t>& poResponse = response->getDataOut();

    switch (poResponse.size()) {
    case 0x21: /* Compatibility mode, Reload */
    case 0x1E: /* Compatibility mode, Debit or Undebit */
        mChallengeOut = std::vector<uint8_t>(2);
        mPreviousSignatureLo = std::vector<uint8_t>(3);
        mCurrentKVC = poResponse[0];
        mTransactionNumber = ByteArrayUtil::twoBytesToInt(poResponse, 1);
        System::arraycopy(poResponse, 3, mPreviousSignatureLo, 0, 3);
        mChallengeOut[0] = poResponse[6];
        mChallengeOut[1] = poResponse[7];
        mBalance = ByteArrayUtil::threeBytesSignedToInt(poResponse, 8);
        if (poResponse.size() == 0x21) {
            /* Reload */
            mLoadLog = std::make_shared<SvLoadLogRecord>(poResponse, 11);
            mDebitLog = nullptr;
        } else {
            /* Debit */
            mLoadLog = nullptr;
            mDebitLog = std::make_shared<SvDebitLogRecord>(poResponse, 11);
        }
        break;
    case 0x3D: /* Revision 3.2 mode */
        mChallengeOut = std::vector<uint8_t>(8);
        mPreviousSignatureLo = std::vector<uint8_t>(6);
        System::arraycopy(poResponse, 0, mChallengeOut, 0, 8);
        mCurrentKVC = poResponse[8];
        mTransactionNumber = ByteArrayUtil::twoBytesToInt(poResponse, 9);
        System::arraycopy(poResponse, 11, mPreviousSignatureLo, 0, 6);
        mBalance = ByteArrayUtil::threeBytesSignedToInt(poResponse, 17);
        mLoadLog = std::make_shared<SvLoadLogRecord>(poResponse, 20);
        mDebitLog = std::make_shared<SvDebitLogRecord>(poResponse, 42);
        break;
    default:
        throw IllegalStateException("Incorrect data length in response to SVGet");
    }
}

const std::vector<uint8_t>& SvGetRespPars::getSvGetCommandHeader() const
{
    return mSvCommandHeader;
}

uint8_t SvGetRespPars::getCurrentKVC() const
{
    return mCurrentKVC;
}

int SvGetRespPars::getTransactionNumber() const
{
    return mTransactionNumber;
}

const std::vector<uint8_t>& SvGetRespPars::getPreviousSignatureLo() const
{
    return mPreviousSignatureLo;
}

const std::vector<uint8_t>& SvGetRespPars::getChallengeOut() const
{
    return mChallengeOut;
}

int SvGetRespPars::getBalance() const
{
    return mBalance;
}

const std::shared_ptr<SvLoadLogRecord> SvGetRespPars::getLoadLog() const
{
    return mLoadLog;
}

const std::shared_ptr<SvDebitLogRecord> SvGetRespPars::getDebitLog() const
{
    return mDebitLog;
}

const std::map<int, std::shared_ptr<StatusProperties>>& SvGetRespPars::getStatusTable() const
{
    return STATUS_TABLE;
}

}
}
}
}
}
}
