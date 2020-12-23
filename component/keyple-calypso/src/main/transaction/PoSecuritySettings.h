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


#pragma once

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "AbstractPoResponseParser.h"
#include "CalypsoSam.h"
#include "KeypleCalypsoExport.h"
#include "KeyReference.h"
#include "PoTransaction.h"

namespace keyple {
namespace calypso {
namespace transaction {

using AccessLevel = PoTransaction::SessionSetting::AccessLevel;
using LogRead = PoTransaction::SvSettings::LogRead;
using ModificationMode = PoTransaction::SessionSetting::ModificationMode;
using NegativeBalance = PoTransaction::SvSettings::NegativeBalance;
using RatificationMode = PoTransaction::SessionSetting::RatificationMode;
using PinTransmissionMode = PoTransaction::PinTransmissionMode;

/**
 * A class dedicated to managing the security settings involved in managing secure sessions.
 * <p>
 * The object provides default values when instantiated, they can be modified with the putKeyInfo
 * method.
 * <p>
 * The getKeyInfo method returns the specified setting value.
 */
class KEYPLECALYPSO_API PoSecuritySettings final {
public:
    /**
     *
     */
    static const ModificationMode mDefaultSessionModificationMode;

    /**
     *
     */
    static const RatificationMode mDefaultRatificationMode;

    /**
     *
     */
    static const PinTransmissionMode mDefaultPinTransmissionMode;

    /**
     * Builder pattern
     */
    class KEYPLECALYPSO_API PoSecuritySettingsBuilder final {
    public:
        /**
         *
         */
        friend PoSecuritySettings;

        /**
         *
         */
        ModificationMode mSessionModificationMode =
            PoSecuritySettings::mDefaultSessionModificationMode;

        /**
         *
         */
        RatificationMode mRatificationMode = PoSecuritySettings::mDefaultRatificationMode;

        /**
         *
         */
        PinTransmissionMode mPinTransmissionMode = mDefaultPinTransmissionMode;

        /**
         *
         */
        std::shared_ptr<KeyReference> mPinCipheringKey = mDefaultPinCipheringKey;

        /**
         *
         */
        LogRead mSvGetLogReadMode = mDefaultSvGetLogReadMode;

        /**
         *
         */
        NegativeBalance mSvNegativeBalance = mDefaultSvNegativeBalance;

        /**
         * Constructor
         *
         * @param samResource the SAM resource we'll be working with<br>
         *        Needed in any cases.
         */
        PoSecuritySettingsBuilder(std::shared_ptr<SeResource<CalypsoSam>> samResource);

        /**
         * Set the Session Modification Mode<br>
         * The default value is ATOMIC
         *
         * @param sessionModificationMode the desired Session Modification Mode
         * @return the builder instance
         * @since 0.9
         */
        PoSecuritySettingsBuilder& sessionModificationMode(
            ModificationMode sessionModificationMode);

        /**
         * Set the Ratification Mode<br>
         * The default value is CLOSE_RATIFIED
         *
         * @param ratificationMode the desired Ratification Mode
         * @return the builder instance
         * @since 0.9
         */
        PoSecuritySettingsBuilder& ratificationMode(const RatificationMode ratificationMode);

        /**
         * Set the PIN Transmission Mode<br>
         * The default value is ENCRYPTED
         *
         * @param pinTransmissionMode the desired PIN Transmission Mode
         * @return the builder instance
         * @since 0.9
         */
        PoSecuritySettingsBuilder& pinTransmissionMode(
            const PinTransmissionMode pinTransmissionMode);

        /**
         * Set the default KIF<br>
         *
         * @param sessionAccessLevel the session level
         * @param kif the desired default KIF
         * @return the builder instance
         * @since 0.9
         */
        PoSecuritySettingsBuilder& sessionDefaultKif(const AccessLevel& sessionAccessLevel,
                                                     const uint8_t kif);

        /**
         * Set the default KVC<br>
         *
         * @param sessionAccessLevel the session level
         * @param kvc the desired default KVC
         * @return the builder instance
         * @since 0.9
         */
        PoSecuritySettingsBuilder& sessionDefaultKvc(const AccessLevel& sessionAccessLevel,
                                                     const uint8_t kvc);

        /**
         * Set the default key record number<br>
         *
         * @param sessionAccessLevel the session level
         * @param keyRecordNumber the desired default key record number
         * @return the builder instance
         * @since 0.9
         */
        PoSecuritySettingsBuilder& sessionDefaultKeyRecordNumber(
            const AccessLevel& sessionAccessLevel, const uint8_t keyRecordNumber);

        /**
         * Provides a list of authorized KVC
         *
         * If this method is not called, the list will remain empty and all KVCs will be accepted.
         *
         * @param authorizedKvcList the list of authorized KVCs
         * @return the builder instance
         */
        PoSecuritySettingsBuilder& sessionAuthorizedKvcList(
            const std::vector<uint8_t>& authorizedKvcList);

        /**
         * Provides the KIF/KVC pair of the PIN ciphering key
         *
         * @param kif the KIF of the PIN ciphering key
         * @param kvc the KVC of the PIN ciphering key
         * @return the builder instance
         */
        PoSecuritySettingsBuilder& pinCipheringKey(const uint8_t kif, const uint8_t kvc);

        /**
         * Sets the SV Get log read mode to indicate whether only one or both log files are to be
         * read
         *
         * @param svGetLogReadMode the
         *        keyple::calypso::transaction::PoTransaction::SvSettings::LogRead mode
         * @return the builder instance
         */
        PoSecuritySettingsBuilder& svGetLogReadMode(const LogRead svGetLogReadMode);


        /**
         * Sets the SV negative balance mode to indicate whether negative balances are allowed or
         * not
         *
         * @param svNegativeBalance the
         *        keyple::calypso::transaction::PoTransaction::SvSettings::NegativeBalance mode
         * @return the builder instance
         */
        PoSecuritySettingsBuilder& svNegativeBalance(const NegativeBalance svNegativeBalance);

        /**
         * Build a new {@code PoSecuritySettings}.
         *
         * @return a new instance
         */
        std::shared_ptr<PoSecuritySettings> build();

    private:
        /**
         *
         */
        std::shared_ptr<SeResource<CalypsoSam>> mSamResource;

        /**
         * List of authorized KVCs
         */
        std::vector<uint8_t> mAuthorizedKvcList;

        /**
         * EnumMap associating session levels and corresponding KIFs
         */
        std::map<AccessLevel, uint8_t> mDefaultKif;
        std::map<AccessLevel, uint8_t> mDefaultKvc;
        std::map<AccessLevel, uint8_t> mDefaultKeyRecordNumber;
    };

    /**
     * (package-private)<br>
     *
     * @return the Sam resource
     * @since 0.9
     */
    const std::shared_ptr<SeResource<CalypsoSam>> getSamResource() const;

    /**
     * (package-private)<br>
     *
     * @return the Session Modification Mode
     * @since 0.9
     */
    ModificationMode getSessionModificationMode() const;

    /**
     * (package-private)<br>
     *
     * @return the Ratification Mode
     * @since 0.9
     */
    RatificationMode getRatificationMode() const;

    /**
     * (package-private)<br>
     *
     * @return the PIN Transmission Mode
     * @since 0.9
     */
    PinTransmissionMode getPinTransmissionMode() const;

    /**
     * (package-private)<br>
     *
     * @return the default session KIF
     * @since 0.9
     */
    uint8_t getSessionDefaultKif(const AccessLevel& sessionAccessLevel) const;

    /**
     * (package-private)<br>
     *
     * @return the default session KVC
     * @since 0.9
     */
    uint8_t getSessionDefaultKvc(const AccessLevel& sessionAccessLevel) const;

    /**
     * (package-private)<br>
     *
     * @return the default session key record number
     * @since 0.9
     */
    uint8_t getSessionDefaultKeyRecordNumber(const AccessLevel& sessionAccessLevel) const;

    /**
     * (package-private)<br>
     * Check if the provided kvc value is authorized or not.
     * <p>
     * If no list of authorized kvc is defined (authorizedKvcList null), all kvc are authorized.
     *
     * @param kvc to be tested
     * @return true if the kvc is authorized
     */
    bool isSessionKvcAuthorized(const uint8_t kvc) const;

    /**
     * (package-private)<br>
     *
     * @return the default key reference to be used for PIN encryption
     */
    const std::shared_ptr<KeyReference> getDefaultPinCipheringKey() const;

    /**
     * (package-private)<br>
     *
     * @return how SV logs are read, indicating whether or not all SV logs are needed
     */
    LogRead getSvGetLogReadMode() const;

    /**
     * (package-private)<br>
     *
     * @return an indication of whether negative balances are allowed or not
     */
    NegativeBalance getSvNegativeBalance() const;

    /**
     * Private constructor
     *
     * C++ vs. Java: std::make_shared prevents from using private here
     */
    PoSecuritySettings(PoSecuritySettingsBuilder* builder);

private:
    /**
     *
     */
    std::shared_ptr<SeResource<CalypsoSam>> mSamResource;

    /**
     * List of authorized KVCs
     */
    const std::vector<uint8_t> mAuthorizedKvcList;

    /**
     * EnumMap associating session levels and corresponding KIFs
     */
    const std::map<AccessLevel, uint8_t> mDefaultKif;
    const std::map<AccessLevel, uint8_t> mDefaultKvc;
    const std::map<AccessLevel, uint8_t> mDefaultKeyRecordNumber;

    const ModificationMode mSessionModificationMode = ModificationMode::ATOMIC;
    const RatificationMode mRatificationMode = RatificationMode::CLOSE_RATIFIED;

    const PinTransmissionMode mPinTransmissionMode;
    const std::shared_ptr<KeyReference> mPinCipheringKey;
    const LogRead mSvGetLogReadMode;
    const NegativeBalance mSvNegativeBalance;

    /**
     *
     */
    static const std::shared_ptr<KeyReference> mDefaultPinCipheringKey;

    /**
     *
     */
    static const LogRead mDefaultSvGetLogReadMode;

    /**
     *
     */
    static const NegativeBalance mDefaultSvNegativeBalance;
};

}
}
}
