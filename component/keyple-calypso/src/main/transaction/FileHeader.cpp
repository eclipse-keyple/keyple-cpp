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

#include "FileHeader.h"

#include <iomanip>

/* Common */
#include "Logger.h"

namespace keyple {
namespace calypso {
namespace transaction {

using FileHeaderBuilder = FileHeader::FileHeaderBuilder;
using FileType = FileHeader::FileType;

/* FILE HEADER -------------------------------------------------------------- */

FileHeader::FileHeader(FileHeaderBuilder* builder)
: mLid(builder->mLid),
  mRecordsNumber(builder->mRecordsNumber),
  mRecordSize(builder->mRecordSize),
  mType(builder->mType),
  mAccessConditions(builder->mAccessConditions),
  mDfStatus(builder->mDfStatus),
  mSharedReference(builder->mSharedReference),
  mShared(builder->mShared) {}

uint16_t FileHeader::getLid() const
{
    return mLid;
}

int FileHeader::getRecordsNumber() const
{
    return mRecordsNumber;
}

int FileHeader::getRecordSize() const
{
    return mRecordSize;
}

FileType FileHeader::getType() const
{
    return mType;
}

const std::vector<uint8_t>& FileHeader::getAccessConditions() const
{
    return mAccessConditions;
}

const std::vector<uint8_t>& FileHeader::getKeyIndexes() const
{
    return mKeyIndexes;
}

uint8_t FileHeader::getDfStatus() const
{
    return mDfStatus;
}

bool FileHeader::isShared() const
{
    return mShared;
}

uint16_t FileHeader::getSharedReference() const
{
    return mSharedReference;
}

std::unique_ptr<FileHeaderBuilder> FileHeader::builder()
{
    return std::unique_ptr<FileHeaderBuilder>(new FileHeaderBuilder());
}

bool FileHeader::operator==(const FileHeader& o) const
{
    return mLid == o.mLid;
}

std::ostream& operator<<(std::ostream& os, const FileHeader& fh)
{
    os << "FILEHEADER : {"
       << "LID = 0x" << std::hex << std::setfill('0') << std::setw(4)
                     << fh.mLid << ", "
       << "RECORDSNUMBER = " << fh.mRecordsNumber << ", "
       << "RECORDSSIZE = " << fh.mRecordSize << ", "
       << "TYPE = " << fh.mType << ", "
       << "ACCCESSCONDITIONS = " << fh.mAccessConditions << ", "
       << "KEYINDEXES = " << fh.mKeyIndexes << ", "
       << "DFSTATUS = 0x" << std::hex << std::setfill('0') << std::setw(2)
                          << fh.mDfStatus << ",  ";

    if (fh.mShared)
       os << "SHAREDREFERENCE = 0x" << std::hex << std::setfill('0')
                                    << std::setw(8) << fh.mSharedReference;
    else
       os << "SHAREDREFERENCE = null";

    os << "}";

    return os;
}

/* FILE HEADER BUILDER ------------------------------------------------------ */

FileHeaderBuilder::FileHeaderBuilder()
: mShared(false) {}

FileHeaderBuilder& FileHeaderBuilder::lid(const uint16_t lid)
{
    mLid = lid;
    return *this;
}

FileHeaderBuilder& FileHeaderBuilder::recordsNumber(const int recordsNumber)
{
    mRecordsNumber = recordsNumber;
    return *this;
}

FileHeaderBuilder& FileHeaderBuilder::recordSize(const int recordSize)
{
    mRecordSize = recordSize;
    return *this;
}

FileHeaderBuilder& FileHeaderBuilder::type(const FileType type)
{
    mType = type;
    return *this;
}

FileHeaderBuilder& FileHeaderBuilder::accessConditions(
    const std::vector<uint8_t>& accessConditions)
{
    mAccessConditions = accessConditions;
    return *this;
}

FileHeaderBuilder& FileHeaderBuilder::keyIndexes(
    const std::vector<uint8_t> keyIndexes)
{
    mKeyIndexes = keyIndexes;
    return *this;
}

FileHeaderBuilder& FileHeaderBuilder::dfStatus(const uint8_t dfStatus)
{
    mDfStatus = dfStatus;
    return *this;
}

FileHeaderBuilder& FileHeaderBuilder::sharedReference(
    const uint16_t sharedReference)
{
    mSharedReference = sharedReference;
    return *this;
}

std::unique_ptr<FileHeader> FileHeaderBuilder::build()
{
    return std::unique_ptr<FileHeader>(new FileHeader(this));
}

/* FILE TYPE ---------------------------------------------------------------- */

std::ostream& operator<<(std::ostream& os, const FileType ft)
{
    switch (ft) {
    case FileType::LINEAR:
        os << "LINEAR";
        break;
    case FileType::BINARY:
        os << "BINARY";
        break;
    case FileType::COUNTERS:
        os << "COUNTERS";
        break;
    case FileType::CYCLIC:
        os << "CYCLIC";
        break;
    case FileType::SIMULATED_COUNTERS:
        os << "SIMULATED COUNTERS";
        break;
    default:
        os << "UNKNOWN";
        break;
    }

    return os;
}

}
}
}