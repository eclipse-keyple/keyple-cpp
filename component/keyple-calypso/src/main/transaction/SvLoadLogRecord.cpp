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

#include "SvLoadLogRecord.h"

/* Core */
#include "ByteArrayUtil.h"

/* Common */
#include "stringhelper.h"
#include "System.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;
using namespace keyple::core::util;

SvLoadLogRecord::SvLoadLogRecord(const std::vector<uint8_t>& poResponse, const int offset)
: mOffset(offset), mPoResponse(poResponse) {}

int SvLoadLogRecord::getAmount() const
{
    return ByteArrayUtil::threeBytesSignedToInt(mPoResponse, mOffset + 8);
}

int SvLoadLogRecord::getBalance() const
{
    return ByteArrayUtil::threeBytesSignedToInt(mPoResponse, mOffset + 5);
}

int SvLoadLogRecord::getLoadTime() const
{
    return ByteArrayUtil::twoBytesToInt(getLoadTimeBytes(), 0);
}

const std::vector<uint8_t> SvLoadLogRecord::getLoadTimeBytes() const
{
    std::vector<uint8_t> time(2);
    time[0] = mPoResponse[mOffset + 11];
    time[1] = mPoResponse[mOffset + 12];

    return time;
}

int SvLoadLogRecord::getLoadDate() const
{
    return ByteArrayUtil::twoBytesToInt(getLoadDateBytes(), 0);
}

const std::vector<uint8_t> SvLoadLogRecord::getLoadDateBytes() const
{
    std::vector<uint8_t> date(2);
    date[0] = mPoResponse[mOffset + 0];
    date[1] = mPoResponse[mOffset + 1];

    return date;
}

std::string SvLoadLogRecord::getFreeByte() const
{
    const char bytes[3] = {
        static_cast<char>(getFreeByteBytes().data()[0]),
        static_cast<char>(getFreeByteBytes().data()[1]),
        '\0'
    };

    return std::string(bytes);
}

const std::vector<uint8_t> SvLoadLogRecord::getFreeByteBytes() const
{
    std::vector<uint8_t> free(2);
    free[0] = mPoResponse[mOffset + 2];
    free[1] = mPoResponse[mOffset + 4];

    return free;
}

uint8_t SvLoadLogRecord::getKvc() const
{
    return mPoResponse[mOffset + 3];
}

uint32_t SvLoadLogRecord::getSamId() const
{
    return ByteArrayUtil::fourBytesToInt(getSamIdBytes(), 0);
}

const std::vector<uint8_t> SvLoadLogRecord::getSamIdBytes() const
{
    std::vector<uint8_t> samId(4);
    System::arraycopy(mPoResponse, mOffset + 13, samId, 0, 4);

    return samId;
}

int SvLoadLogRecord::getSvTNum() const
{
    return ByteArrayUtil::twoBytesToInt(getSvTNumBytes(), 0);
}

const std::vector<uint8_t> SvLoadLogRecord::getSvTNumBytes() const
{
    std::vector<uint8_t> tnNum(2);
    tnNum[0] = mPoResponse[mOffset + 20];
    tnNum[1] = mPoResponse[mOffset + 21];

    return tnNum;
}

int SvLoadLogRecord::getSamTNum() const
{
    return ByteArrayUtil::threeBytesToInt(getSamTNumBytes(), 0);
}

const std::vector<uint8_t> SvLoadLogRecord::getSamTNumBytes() const
{
    std::vector<uint8_t> samTNum(3);
    System::arraycopy(mPoResponse, mOffset + 17, samTNum, 0, 3);

    return samTNum;
}

const std::string SvLoadLogRecord::toString() const
{
    std::stringstream ss;
    ss << "{"
       <<     "\"SvLoadLogRecord\":{"
       <<         "\"amount\":" << getAmount() << ", "
       <<         "\"balance\":" << getBalance() << ", "
       <<         "\"debitDate\":" << getLoadDate() << ", "
       <<         "\"debitTime\":" << getLoadTime() << ", "
       <<         "\"freeBytes\":" << ByteArrayUtil::toHex(getFreeByteBytes()) << ", "
       <<         "\"kvc\":" << getKvc() << ", "
       <<         "\"samId\":" << ByteArrayUtil::toHex(getSamIdBytes()) << ", "
       <<         "\"svTransactionNumber\":" << getSvTNum() << ", "
       <<         "\"svSamTransactionNumber\":" << getSamTNum()
       <<     "}"
       << "}";

    return ss.str();
}

}
}
}
