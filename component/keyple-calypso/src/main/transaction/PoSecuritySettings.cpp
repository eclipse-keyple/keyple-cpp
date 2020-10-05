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

#include "PoSecuritySettings.h"

/* Common */
#include "IllegalStateException.h"
#include "NoSuchElementException.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;

using PoSecuritySettingsBuilder = PoSecuritySettings::PoSecuritySettingsBuilder;

/* PO SECURITY SETTINGS ----------------------------------------------------- */

const ModificationMode PoSecuritySettings::mDefaultSessionModificationMode =
    ModificationMode::ATOMIC;
const RatificationMode PoSecuritySettings::mDefaultRatificationMode =
    RatificationMode::CLOSE_RATIFIED;

PoSecuritySettings::PoSecuritySettings(PoSecuritySettingsBuilder* builder)
: mSamResource(builder->mSamResource),
  mAuthorizedKvcList(builder->mAuthorizedKvcList),
  mDefaultKif(builder->mDefaultKif),
  mDefaultKvc(builder->mDefaultKvc),
  mDefaultKeyRecordNumber(builder->mDefaultKeyRecordNumber),
  mSessionModificationMode(builder->mSessionModificationMode),
  mRatificationMode(builder->mRatificationMode) {}

const std::shared_ptr<SeResource<CalypsoSam>>
    PoSecuritySettings::getSamResource() const
{
    return mSamResource;
}

ModificationMode PoSecuritySettings::getSessionModificationMode() const
{
    return mSessionModificationMode;
}

RatificationMode PoSecuritySettings::getRatificationMode() const
{
    return mRatificationMode;
}

uint8_t PoSecuritySettings::getSessionDefaultKif(
    const AccessLevel& sessionAccessLevel) const
{
    /* C++ vs. Java: throw error when not found */
    const auto it = mDefaultKif.find(sessionAccessLevel);
    if (it == mDefaultKif.end())
        throw NoSuchElementException("AccessLevel not found in default KIF");

    return it->second;
}

uint8_t PoSecuritySettings::getSessionDefaultKvc(
    const AccessLevel& sessionAccessLevel) const
{
    /* C++ vs. Java: throw error when not found */
    const auto it = mDefaultKvc.find(sessionAccessLevel);
    if (it == mDefaultKvc.end())
        throw NoSuchElementException("AccessLevel not found in default KVC");

    return it->second;
}

uint8_t PoSecuritySettings::getSessionDefaultKeyRecordNumber(
    const AccessLevel& sessionAccessLevel) const
{
    /* C++ vs. Java: throw error when not found */
    const auto it = mDefaultKeyRecordNumber.find(sessionAccessLevel);
    if (it == mDefaultKeyRecordNumber.end())
        throw NoSuchElementException("AccessLevel not found in default KRN");

    return it->second;
}

bool PoSecuritySettings::isSessionKvcAuthorized(const uint8_t kvc) const
{
    return mAuthorizedKvcList.empty() ||
           (std::find(mAuthorizedKvcList.begin(), mAuthorizedKvcList.end(), kvc)
               != mAuthorizedKvcList.end());
}

/* PO SECURITY SETTINGS BUILDER --------------------------------------------- */

PoSecuritySettingsBuilder::PoSecuritySettingsBuilder(
    std::shared_ptr<SeResource<CalypsoSam>> samResource)
{
    if (!samResource)
        throw IllegalStateException("SAM resource cannot be null.");

    mSamResource = samResource;
}

PoSecuritySettingsBuilder& PoSecuritySettingsBuilder::sessionModificationMode(
    const ModificationMode sessionModificationMode)
{
    mSessionModificationMode = sessionModificationMode;
    return *this;
}

PoSecuritySettingsBuilder& PoSecuritySettingsBuilder::ratificationMode(
    const RatificationMode ratificationMode)
{
    mRatificationMode = ratificationMode;
    return *this;
}

PoSecuritySettingsBuilder& PoSecuritySettingsBuilder::sessionDefaultKif(
    const AccessLevel& sessionAccessLevel, const uint8_t kif)
{
    mDefaultKif.insert({sessionAccessLevel, kif});
    return *this;
}

PoSecuritySettingsBuilder& PoSecuritySettingsBuilder::sessionDefaultKvc(
    const AccessLevel& sessionAccessLevel, const uint8_t kvc)
{
    mDefaultKvc.insert({sessionAccessLevel, kvc});
    return *this;
}

PoSecuritySettingsBuilder&
    PoSecuritySettingsBuilder::sessionDefaultKeyRecordNumber(
        const AccessLevel& sessionAccessLevel,
        const uint8_t keyRecordNumber)
{
    mDefaultKeyRecordNumber.insert({sessionAccessLevel, keyRecordNumber});
    return *this;
}

PoSecuritySettingsBuilder& PoSecuritySettingsBuilder::sessionAuthorizedKvcList(
    const std::vector<uint8_t>& authorizedKvcList)
{
    mAuthorizedKvcList = authorizedKvcList;
    return *this;
}

std::unique_ptr<PoSecuritySettings> PoSecuritySettingsBuilder::build()
{
    return std::unique_ptr<PoSecuritySettings>(new PoSecuritySettings(this));
}

}
}
}
