
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

#include "SvDebitLogRecord.h"

/* Core */
#include "ByteArrayUtil.h"

/* Common */
#include "System.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;
using namespace keyple::core::util;

SvDebitLogRecord::SvDebitLogRecord(const std::vector<uint8_t>& poResponse, const int offset)
: mOffset(offset), mPoResponse(poResponse) {}

int SvDebitLogRecord::getAmount() const
{
	return ByteArrayUtil::twoBytesSignedToInt(mPoResponse, mOffset);
}
int SvDebitLogRecord::getBalance() const
{
	return ByteArrayUtil::threeBytesSignedToInt(mPoResponse, mOffset + 14);
}

int SvDebitLogRecord::getDebitTime() const
{
    return ByteArrayUtil::twoBytesToInt(getDebitTimeBytes(), 0);
}

const std::vector<uint8_t> SvDebitLogRecord::getDebitTimeBytes() const
{
    std::vector<uint8_t> time(2);
    time[0] = mPoResponse[mOffset + 4];
    time[1] = mPoResponse[mOffset + 5];

    return time;
}

int SvDebitLogRecord::getDebitDate() const
{
    return ByteArrayUtil::twoBytesToInt(getDebitDateBytes(), 0);
}

const std::vector<uint8_t> SvDebitLogRecord::getDebitDateBytes() const
{
    std::vector<uint8_t> date(2);
    date[0] = mPoResponse[mOffset + 2];
    date[1] = mPoResponse[mOffset + 3];

    return date;
}

uint8_t SvDebitLogRecord::getKvc() const
{
    return mPoResponse[mOffset + 6];
}

uint32_t SvDebitLogRecord::getSamId() const
{
    return ByteArrayUtil::fourBytesToInt(getSamIdBytes(), 0);
}

const std::vector<uint8_t> SvDebitLogRecord::getSamIdBytes() const
{
    std::vector<uint8_t> samId(4);
    System::arraycopy(mPoResponse, mOffset + 7, samId, 0, 4);

    return samId;
}
int SvDebitLogRecord::getSvTNum() const
{
    return ByteArrayUtil::twoBytesToInt(getSvTNumBytes(), 0);
}

const std::vector<uint8_t> SvDebitLogRecord::getSvTNumBytes() const
{
    std::vector<uint8_t> tnNum(2);
    tnNum[0] = mPoResponse[mOffset + 17];
    tnNum[1] = mPoResponse[mOffset + 18];

    return tnNum;
}

int SvDebitLogRecord::getSamTNum() const
{
    return ByteArrayUtil::threeBytesToInt(getSamTNumBytes(), 0);
}

const std::vector<uint8_t> SvDebitLogRecord::getSamTNumBytes() const
{
    std::vector<uint8_t> samTNum(3);
    System::arraycopy(mPoResponse, mOffset + 11, samTNum, 0, 3);

    return samTNum;
}

const std::string SvDebitLogRecord::toString() const
{
    std::stringstream ss;
    ss << "{"
       <<     "\"SvDebitLogRecord\":{"
       <<         "\"amount\":" << getAmount() << ", "
       <<         "\"balance\":" << getBalance() << ", "
       <<         "\"debitDate\":" << getDebitDate() << ", "
       <<         "\"debitTime\":" << getDebitDate() << ", "
       <<         "\"kvc\":" << getKvc() << ", "
       <<         "\"samId\":" + ByteArrayUtil::toHex(getSamIdBytes()) << ", "
       <<         "\"svTransactionNumber\":" << getSvTNum() << ", "
       <<         "\"svSamTransactionNumber\":" << getSamTNum()
       <<      "}"
       << "}";

    return ss.str();
}

}
}
}
