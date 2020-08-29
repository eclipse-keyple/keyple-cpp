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
namespace keyple {
namespace calypso {
namespace transaction {

const PoSecuritySettings::ModificationMode mDefaultSessionModificationMode =
    ModificationMode::ATOMIC;
const PoSecuritySettings::RatificationMode mDefaultRatificationMode =
    RatificationMode::CLOSE_RATIFIED;

PoSecuritySettings::PoSecuritySettings(PoSecuritySettingsBuilder* builder)
: mSamResource(builder->samResource),
  mAuthorizedKvcList(builder->authorizedKvcList),
  mDefaultKif(builder->defaultKif), mDefaultKvc(builder->defaultKvc),
  mDefaultKeyRecordNumber(builder->defaultKeyRecordNumber),
  mSessionModificationMode(builder->sessionModificationMode),
  mRatificationMode(builder->ratificationMode) {}

const std::shared_ptr<SeResource<CalypsoSam>>
    PoSecuritySettings::getSamResource() const
{
    return mSamResource;
}

const ModificationMode& PoSecuritySettings::getSessionModificationMode() const
{
    return mSessionModificationMode;
}

const RatificationMode& PoSecuritySettings::getRatificationMode() const
{
    return mRatificationMode;
}

const uint8_t PoSecuritySettings::getSessionDefaultKif(
    const AccessLevel sessionAccessLevel) const
{
    /* C++ vs. Java: throw error when not found */
    std::map<AccessLevel, uint8_t>::const_iterator it;
    if ((it = mDefaultKif.find(sessionAccessLevel) == mDefaultKif.end()))
        throw NoSuchElementException("AccessLevel not found in default KIF");

    return *it;
}

const uint8_t PoSecuritySettings::getSessionDefaultKvc(
    const AccessLevel sessionAccessLevel) const
{
    /* C++ vs. Java: throw error when not found */
    std::map<AccessLevel, uint8_t>::const_iterator it;
    if ((it = mDefaultKvc.find(sessionAccessLevel) == mDefaultKvc.end()))
        throw NoSuchElementException("AccessLevel not found in default KVC");

    return *it;
}

const uint8_t PoSecuritySettings::getSessionDefaultKeyRecordNumber(
    const AccessLevel sessionAccessLevel) const
{
    /* C++ vs. Java: throw error when not found */
    std::map<AccessLevel, uint8_t>::const_iterator it;
    if ((it = mDefaultKeyRecordNumber.find(sessionAccessLevel) ==
        mDefaultKeyRecordNumber.end()))
        throw NoSuchElementException("AccessLevel not found in default KRN");

    return *it;
}

const bool PoSecuritySettings::isSessionKvcAuthorized(const uint8_t kvc) const
{
    return mAuthorizedKvcList.empty() ||
           (mAuthorizedKvcList.find(kvc) != mAuthorizedKvcList.end());
}

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
    const AccessLevel sessionAccessLevel, const uint8_t kif)
{
    mDefaultKif.insert({sessionAccessLevel, kif});
    return *this;
}

PoSecuritySettingsBuilder& PoSecuritySettingsBuilder::sessionDefaultKvc(
    const AccessLevel sessionAccessLevel, const uint8_t kvc)
{
    mDefaultKvc.insert({sessionAccessLevel, kvc});
    return *this;
}

PoSecuritySettingsBuilder&
    PoSecuritySettingsBuilder::sessionDefaultKeyRecordNumber(
        const AccessLevel sessionAccessLevel,
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
