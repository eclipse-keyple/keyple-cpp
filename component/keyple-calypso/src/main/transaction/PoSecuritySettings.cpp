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

#include "PoSecuritySettings.h"

/* Common */
#include "IllegalStateException.h"
#include "NoSuchElementException.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;
using namespace keyple::common::exception;

using PoSecuritySettingsBuilder = PoSecuritySettings::PoSecuritySettingsBuilder;

/* PO SECURITY SETTINGS ----------------------------------------------------- */

const ModificationMode PoSecuritySettings::mDefaultSessionModificationMode =
    ModificationMode::ATOMIC;
const RatificationMode PoSecuritySettings::mDefaultRatificationMode =
    RatificationMode::CLOSE_RATIFIED;
const PinTransmissionMode PoSecuritySettings::mDefaultPinTransmissionMode =
    PinTransmissionMode::ENCRYPTED;
const std::shared_ptr<KeyReference> PoSecuritySettings::mDefaultPinCipheringKey =
    std::make_shared<KeyReference>(static_cast<uint8_t>(0), static_cast<uint8_t>(0));
const LogRead PoSecuritySettings::mDefaultSvGetLogReadMode = LogRead::SINGLE;
const NegativeBalance PoSecuritySettings::mDefaultSvNegativeBalance = NegativeBalance::FORBIDDEN;

PoSecuritySettings::PoSecuritySettings(PoSecuritySettingsBuilder* builder)
: mSamResource(builder->mSamResource),
  mAuthorizedKvcList(builder->mAuthorizedKvcList),
  mDefaultKif(builder->mDefaultKif),
  mDefaultKvc(builder->mDefaultKvc),
  mDefaultKeyRecordNumber(builder->mDefaultKeyRecordNumber),
  mSessionModificationMode(builder->mSessionModificationMode),
  mRatificationMode(builder->mRatificationMode),
  mPinTransmissionMode(builder->mPinTransmissionMode),
  mPinCipheringKey(builder->mPinCipheringKey),
  mSvGetLogReadMode(builder->mSvGetLogReadMode),
  mSvNegativeBalance(builder->mSvNegativeBalance) {}

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

PinTransmissionMode PoSecuritySettings::getPinTransmissionMode() const
{
    return mPinTransmissionMode;
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

const std::shared_ptr<KeyReference> PoSecuritySettings::getDefaultPinCipheringKey() const
{
    return mPinCipheringKey;
}

LogRead PoSecuritySettings::getSvGetLogReadMode() const
{
    return mSvGetLogReadMode;
}

NegativeBalance PoSecuritySettings::getSvNegativeBalance() const
{
    return mSvNegativeBalance;
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

PoSecuritySettingsBuilder& PoSecuritySettingsBuilder::pinTransmissionMode(
    const PinTransmissionMode pinTransmissionMode)
{
    mPinTransmissionMode = pinTransmissionMode;
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

PoSecuritySettingsBuilder& PoSecuritySettingsBuilder::pinCipheringKey(const uint8_t kif,
                                                                      const uint8_t kvc)
{
    mPinCipheringKey = std::make_shared<KeyReference>(kif, kvc);
    return *this;
}

PoSecuritySettingsBuilder& PoSecuritySettingsBuilder::svGetLogReadMode(
    const LogRead svGetLogReadMode)
{
    mSvGetLogReadMode = svGetLogReadMode;
    return *this;
}

PoSecuritySettingsBuilder& PoSecuritySettingsBuilder::svNegativeBalance(
    const NegativeBalance svNegativeBalance)
{
    mSvNegativeBalance = svNegativeBalance;
    return *this;
}

std::shared_ptr<PoSecuritySettings> PoSecuritySettingsBuilder::build()
{
    return std::make_shared<PoSecuritySettings>(this);
}

}
}
}
