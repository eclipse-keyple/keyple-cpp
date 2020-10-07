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
#include "IndexOutOfBoundException.h"
#include "Logger.h"
#include "NoSuchElementException.h"
#include "System.h"

/* Core */
#include "ByteArrayUtil.h"
#include "KeypleAssert.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;
using namespace keyple::core::util;

FileData::FileData() {}

const std::map<int, std::vector<uint8_t>>&
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
    const auto it = mRecords.find(numRecord);
    if (it == mRecords.end())
        throw NoSuchElementException(
                  "Record #" + std::to_string(numRecord) + " is not set.");

    return it->second;
}

const std::vector<uint8_t> FileData::getContent(const int numRecord,
                                                const int dataOffset,
                                                const int dataLength) const
{
    KeypleAssert::getInstance()
        .greaterOrEqual(dataOffset, 0, "dataOffset")
        .greaterOrEqual(dataLength, 1, "dataLength");

    const auto it= mRecords.find(numRecord);
    if (it == mRecords.end())
        throw NoSuchElementException(
                  "Record #" + std::to_string(numRecord) + " is not set.");

    const std::vector<uint8_t>& content = it->second;
    if (dataOffset >= static_cast<int>(content.size()))
        throw IndexOutOfBoundException(
                  "Offset [" + std::to_string(dataOffset) + "] >= " +
                  "content length [" + std::to_string(content.size()) + "].");

    const int toIndex = dataOffset + dataLength;
    if (toIndex > static_cast<int>(content.size()))
        throw IndexOutOfBoundException(
                  "Offset [" + std::to_string(dataOffset) + "] + " +
                  "Length [" + std::to_string(dataLength) + "] = " +
                  "[" + std::to_string(toIndex) + "] > " +
                  "content length [" + std::to_string(content.size()) + "].");

    return Arrays::copyOfRange(content, dataOffset, toIndex);
}

int FileData::getContentAsCounterValue(const int numCounter) const
{
    KeypleAssert::getInstance().greaterOrEqual(numCounter, 1, "numCounter");

    const auto it= mRecords.find(1);
    if (it == mRecords.end())
        throw NoSuchElementException("Record #1 is not set.");

    const std::vector<uint8_t> rec1 = it->second;
    const int counterIndex = (numCounter - 1) * 3;

    if (counterIndex >= static_cast<int>(rec1.size()))
        throw NoSuchElementException(
                  "Counter #" + std::to_string(numCounter) +
                  " is not set (nb of actual counters = " +
                  std::to_string(rec1.size() / 3) + ").");

    if (counterIndex + 3 > static_cast<int>(rec1.size()))
        throw IndexOutOfBoundException(
                  "Counter #" + std::to_string(numCounter) +
                  "has a truncated value (nb of actual counters = " +
                  std::to_string(rec1.size() / 3) + ").");

    return ByteArrayUtil::threeBytesToInt(rec1, counterIndex);
}

const std::map<int, int> FileData::getAllCountersValue() const
{
    std::map<int, int> result;

    const auto it = mRecords.find(1);
    if (it == mRecords.end())
        throw NoSuchElementException("Record #1 is not set.");

    const std::vector<uint8_t> rec1 = it->second;
    const int length = rec1.size() - (rec1.size() % 3);

    for (int i = 0, c = 1; i < length; i += 3, c++)
        result.insert({c, ByteArrayUtil::threeBytesToInt(rec1, i)});

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
    std::vector<uint8_t> oldContent;
    const int newLength = offset + content.size();

    const auto it = mRecords.find(numRecord);
    if (it != mRecords.end())
        oldContent = it->second;

    if (static_cast<int>(oldContent.size()) <= offset)
        System::arraycopy(oldContent, 0, newContent, 0, oldContent.size());
    else if (static_cast<int>(oldContent.size()) < newLength)
        System::arraycopy(oldContent, 0, newContent, 0, offset);
    else
        newContent = oldContent;

    System::arraycopy(content, 0, newContent, offset, content.size());

    mRecords.insert({numRecord, newContent});
}

void FileData::fillContent(const int numRecord,
                           std::vector<uint8_t>& content)
{
    /*
     * /!\ Make sure 'actualContent' *is* the vector from the map and not a copy
     */
    const auto it = mRecords.find(numRecord);
    if (it == mRecords.end()) {
        mRecords.insert({numRecord, content});
    } else {
        std::vector<uint8_t>& actualContent = it->second;

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
    for (auto it = mRecords.rbegin(); it != mRecords.rend(); it++)
        descendingKeys.push_back(it->first);

    for (const auto& i : descendingKeys)
        mRecords.insert({i + 1, mRecords[i]});

    mRecords.insert({1, content});
}

std::ostream& operator<<(std::ostream& os, const FileData& fd)
{
    os << "FILEDATA : {"
       << "RECORDS = ";

    for (const auto& rec : fd.mRecords)
        os << "{" << rec.first << ", " << rec.second << "}";

    os << "}";

    return os;
}


}
}
}
