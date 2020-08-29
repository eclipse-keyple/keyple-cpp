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

#include "FileData.h"

/* Common */
#include "Arrays.h"
#include "IndexOutOfBoundsException.h"
#include "NoSuchElementException.h"
#include "stringhelper.h"

/* Core */
#include "ByteArrayUtil.h"
#include "KeypleAssert.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;
using namespace keyple::core::util;

FileData::FileData() {}

const std::map<int, const std::vector<uint8_t>>&
    FileData::getAllRecordsContent() const
{
    return mRecords;
}

const std::vector<uint8_t>& FileData::getContent() const
{
    return getContent(1);
}

const std::vector<uint8_t>& FileData::getContent(const int numRecord) const
{
    std::map<int, const std::vector<uint8_t>>::const_iterator it;

    if ((it = mRecords.find(numRecord) == mRecords.end()))
        throw NoSuchElementException(
                  StringHelper::formatSimple(
                      "Record #%d is not set.", numRecord));

    return *it;
}

const std::vector<uint8_t> FileData::getContent(const int numRecord,
                                                const int dataOffset,
                                                const int dataLength) const
{
    KeypleAssert::getInstance()
        .greaterOrEqual(dataOffset, 0, "dataOffset")
        .greaterOrEqual(dataLength, 1, "dataLength");

    std::map<int, const std::vector<uint8_t>>::const_iterator it;
    if ((it = mRecords.find(numRecord) == mRecords.end()))
        throw NoSuchElementException(
                  StringHelper::formatSimple(
                      "Record #%d is not set.", numRecord));

    const std::vector<uint8_t>& content = *it;
    if (dataOffset >= static_cast<int>(content.size())
        throw IndexOutOfBoundsException(
                  StringHelper::formatSimple(
                      "Offset [%d] >= content length [%d].",
                      dataOffset,
                      content.size());
    }

    const int toIndex = dataOffset + dataLength;
    if (toIndex > static_cast<int>(content.size()) {
        throw IndexOutOfBoundsException(
                  StringHelper::formatSimple(
                      "Offset [%d] + Length [%d] = [%d] > content length [%s].",
                      dataOffset,
                      dataLength,
                      toIndex,
                      content.size());
    }

    return Arrays::copyOfRange(content, dataOffset, toIndex);
}

const int FileData::getContentAsCounterValue(const int numCounter) const
{
    Assert::getInstance().greaterOrEqual(numCounter, 1, "numCounter");

    std::map<int, const std::vector<uint8_t>>::const_iterator it;
    if ((it = mRecords.find(1) == mRecords.end()))
        throw NoSuchElementException("Record #1 is not set.");

    const std::vector<uint8_t> rec1 = *it;
    const int counterIndex = (numCounter - 1) * 3;

    if (counterIndex >= static_cast<int>(rec1.size()) {
        throw NoSuchElementException(
                  StringHelper::formatSimple(
                      "Counter #%d is not set (nb of actual counters = %d).",
                      numCounter,
                      rec1.size() / 3);

    if (counterIndex + 3 > static_cast<int>(rec1.size()))
        throw IndexOutOfBoundsException(
                  StringHelper::formatSimple(
                      "Counter #%d has a truncated value (nb of actual " \
                      "counters = %d).",
                      numCounter,
                      rec1.size() / 3);

    return ByteArrayUtil::threeBytesToInt(rec1, counterIndex);
}

const std::map<int, int> FileData::getAllCountersValue() const
{
    std::map<int, int> result;
    std::map<int, const std::vector<uint8_t>>::const_iterator it;
    if ((it = mRecords.find(1) == mRecords.end()))
        throw NoSuchElementException("Record #1 is not set.");

    const std::vector<uint8_t> rec1 = *it;
    const int length = rec1.size() - (rec1.size() % 3);

    for (int i = 0, c = 1; i < length; i += 3, c++)
        result.insert({c, ByteArrayUtil::threeBytesToInt(rec1, i));

    return result;
}

void FileData::setContent(const int numRecord,
                          const std::vector<uint8_t>& content)
{
    mRecords.insert({numRecord, content});
}

void FileData::setCounter(const int numCounter,
                          const std::vector<uint8_t>& content)
{
    setContent(1, content, (numCounter - 1) * 3);
}

void FileData::setContent(const int numRecord,
                          const std::vector<uint8_t>& content,
                          const int offset)
{
    std::vector<uint8_t> newContent;
    const int newLength = offset + content.size();
    std::map<int, const std::vector<uint8_t>>::const_iterator it;

    std::vector<uint8_t> oldContent;
    if ((it = mRecords.find(numRecord) == mRecords.end()))
        oldContent = mRecords.get(numRecord);

    if (static_cast<int>(oldContent.size()) <= offset)
        System::arraycopy(oldContent, 0, newContent, 0, oldContent.length);
    else if (static_cast<int>(oldContent.size()) < newLength)
        System::arraycopy(oldContent, 0, newContent, 0, offset);
    else
        newContent = oldContent;

    System::arraycopy(content, 0, newContent, offset, content.length);

    mRecords.insert({numRecord, newContent});
}

void FileData::fillContent(const int numRecord,
                           std::vector<uint8_t>& content)
{
    /*
     * /!\ Make sure actualContent *is* the vector from the map and not a copy
     */
    std::vector<uint8_t>& actualContent;
    std::map<int, const std::vector<uint8_t>>::const_iterator it;

    if ((it = mRecords.find(numRecord) == mRecords.end()))
        mRecords.insert({numRecord, content});
    else {
        actualContent = *it;

        if (static_cast<int>(actualContent.size()) <
            static_cast<int>(content.size())) {
            for (int i = 0; i < static_cast<int>(actualContent.size()); i++)
                content[i] |= actualContent[i];
            mRecords.insert({numRecord, content});
        } else {
            for (int i = 0; i < static_cast<int>(content.size()); i++)
                actualContent[i] |= content[i];
        }
    }
}

void FileData::addCyclicContent(const std::vector<uint8_t>& content)
{
    std::vector<int> descendingKeys;

    /* C++: traverse the map in descending order */
    std::map<int, std::vector<uint8_t>>::reverse_iterator it;
    for (it = mRecords.begin(); it != mRecords.end(); it++)
        descendingKeys.push_back(it->first);

    for (const auto& i : descendingKeys)
        mRecords.insert({i + 1, mRecords[i}});

    mRecords.insert({1, content});
}

std::ostream& FileData::operator<<(std::ostream& os,
                                   const FileData& fd) const
{
    os << "FILEDATA : {"
       << "RECORDS = ";

    for (const auto& rec : fd.mRecords)
        os << "{" << rec.fisrt << ", " << rec.second << "}";

    os << "}";

    return os;
}


}
}
}
