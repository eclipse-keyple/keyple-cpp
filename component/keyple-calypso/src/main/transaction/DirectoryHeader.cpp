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

#include "DirectorHeader.h"

/* Core */
#include "KeypleAssert.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::core::util;

DirectoryHeaderBuilder::DirectoryHeaderBuilder() {}

DirectoryHeaderBuilder& DirectoryHeaderBuilder::lid(const uint16_t lid)
{
    mLid = lid;
    return *this;
}

DirectoryHeaderBuilder& DirectoryHeaderBuilder::accessConditions(
    const std::vector<uint8_t>& accessConditions)
{
    mAccessConditions = accessConditions;
    return *this;
}

DirectoryHeaderBuilder& DirectoryHeaderBuilder::keyIndexes(
    const std::vector<uint8_t>& keyIndexes)
{
    mKeyIndexes = keyIndexes;
    return *this;
}

DirectoryHeaderBuilder& DirectoryHeaderBuilder::dfStatus(const uint8_t dfStatus)
{
    mDfStatus = dfStatus;
    return *this;
}

DirectoryHeaderBuilder& DirectoryHeaderBuilder::kif(const AccessLevel level,
                                                    const uint8_t kif)
{
    mKif.insert({level, kif});
    return *this;
}

DirectoryHeaderBuilder& DirectoryHeaderBuilder::kvc(const AccessLevel level,
                                                    const uint8_t kvc)
{
    mKvc.insert({level, kvc});
    return *this;
}

std::unique_ptr<DirectoryHeader> DirectoryHeaderBuilder::build()
{
    return std::unique_ptr<DirectoryHeader>(new DirectoryHeader(this));
}

const uint8_t DirectoryHeader::getLid() const
{
    return mLid;
}

const std::vector<uint8_t>& DirectoryHeader::getAccessConditions() const
{
    return mAccessConditions;
}

const std::vector<uint8_t>& DirectoryHeader::getKeyIndexes() const
{
    return mKeyIndexes;
}

const uint8_t DirectoryHeader::getDfStatus() const
{
    return mDfStatus;
}

const uint8_t DirectoryHeader::getKif(const AccessLevel level) const
{
    std::map<AccessLevel, uint8_t>::const_iterator it;

    if ((it = mKif.find(level)) == mKif.end())
        throw NoSuchElementException(
                  StringHelper::formatSimple(
                      "KIF not found for session access level [%d].", level));

    return *it;
}

const uint8_t DirectoryHeader::getKvc(const AccessLevel level) const
{
    std::map<AccessLevel, uint8_t>::const_iterator it;

    if ((it = mKvc.find(level)) == mKvc.end())
        throw NoSuchElementException(
                  StringHelper::formatSimple(
                      "KVC not found for session access level [%d].", level);

    return *it;
}

std::unique_ptr<DirectoryHeaderBuilder> DirectoryHeader::builder() {
    return std::unique_ptr<DirectoryHeaderBuilder>(
               new DirectoryHeaderBuilder());
}

bool DirectoryHeader::operator==(const DirectoryHeader& o) const
{
    return mLid == o.mLid;
}

std::ostream& DirectoryHeader::operator<<(
    std::ostream& os, const DirectoryHeader& dh) const override
{
    os << "OSTREAM: {"
       << "LID = 0x" << std::hex << setfill('0') << setw(2) << dh.mLid << ", "
       << "ACCESSCONDITIONS = " << dh.mAccessConditions << ", "
       << "KEYINDEXES = " << dh.mKeyIndexes << ", "
       << "DFSTATUS = 0x" << std::hex << setfill('0') << setw(2)
           << oh.mDfStatus << ", "
       << "KIF (todo)" << ", "
       << "KVC (todo)"
       << "}";

    return os;
}

DirectoryHeader::DirectoryHeader(const DirectoryHeaderBuilder* builder)
: mLid(builder->mLid), mAccessConditions(builder->accessConditions),
  mKeyIndexes(builder->mKeyIndexes), mDfStatus(builder->dfStatus),
  mKif(builder->kif), mKvc(builder->kvc) {}

}
}
