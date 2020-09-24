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

#pragma once

#include "PoTransaction.h"

#include <cstdint>
#include <map>
#include <vector>

namespace keyple {
namespace calypso {
namespace transaction {

using AccessLevel = PoTransaction::SessionSetting::AccessLevel;

/**
 * The class {@code DirectoryHeader} contains all metadata of a Calypso DF.
 *
 * @since 0.9
 */
class DirectoryHeader final {
public:
    /**
     * (package-private)<br>
     * Builder pattern
     */
    class DirectoryHeaderBuilder final {
    public:
        /**
         * Friend declaration to give access to private members
         */
        friend DirectoryHeader;

        /**
         * (package-private)<br>
         * Sets the LID.
         *
         * @param lid the LID
         * @return the builder instance
         */
        DirectoryHeaderBuilder& lid(const uint16_t lid);

        /**
         * (package-private)<br>
         * Sets a reference to the provided access conditions byte array.
         *
         * @param accessConditions the access conditions (should be not null and
         *        4 bytes length)
         * @return the builder instance
         */
        DirectoryHeaderBuilder& accessConditions(
            const std::vector<uint8_t>& accessConditions);

        /**
         * (package-private)<br>
         * Sets a reference to the provided key indexes byte array.
         *
         * @param keyIndexes the key indexes (should be not null and 4 bytes
         *        length)
         * @return the builder instance
         */
        DirectoryHeaderBuilder& keyIndexes(
            const std::vector<uint8_t>& keyIndexes);

        /**
         * (package-private)<br>
         * Sets the DF status.
         *
         * @param dfStatus the DF status (byte)
         * @return the builder instance
         */
        DirectoryHeaderBuilder& dfStatus(const uint8_t dfStatus);

        /**
         * (package-private)<br>
         * Add a KIF.
         *
         * @param level the KIF session access level (should be not null)
         * @param kif the KIF value
         * @return the builder instance
         */
        DirectoryHeaderBuilder& kif(const AccessLevel& level,
                                    const uint8_t kif);

        /**
         * (package-private)<br>
         * Add a KVC.
         *
         * @param level the KVC session access level (should be not null)
         * @param kvc the KVC value
         * @return the builder instance
         */
        DirectoryHeaderBuilder& kvc(const AccessLevel& level,
                                    const uint8_t kvc);

        /**
         * (package-private)<br>
         * Build a new {@code DirectoryHeader}.
         *
         * @return a new instance
         */
        std::unique_ptr<DirectoryHeader> build();

    protected:
        /**
         * Private constructor
         */
        DirectoryHeaderBuilder();

    private:
        /**
         *
         */
        uint16_t mLid;

        /**
         *
         */
        std::vector<uint8_t> mAccessConditions;

        /**
         *
         */
        std::vector<uint8_t> mKeyIndexes;

        /**
         *
         */
        uint8_t mDfStatus;

        /**
         *
         */
        std::map<AccessLevel, uint8_t> mKif;

        /**
         *
         */
        std::map<AccessLevel, uint8_t> mKvc;
    };

    /**
     * Gets the associated LID.
     *
     * @return the LID
     * @since 0.9
     */
    uint8_t getLid() const;

    /**
     * Gets a reference to access conditions.
     *
     * @return a not empty byte array
     * @since 0.9
     */
    const std::vector<uint8_t>& getAccessConditions() const;

    /**
     * Gets a reference to keys indexes.
     *
     * @return a not empty byte array
     * @since 0.9
     */
    const std::vector<uint8_t>& getKeyIndexes() const;

    /**
     * Gets the DF status.
     *
     * @return the DF status byte
     * @since 0.9
     */
    uint8_t getDfStatus() const;

    /**
     * Gets the KIF associated to the provided session access level.
     *
     * @param level the session access level (should be not null)
     * @return a not null value
     * @throw IllegalArgumentException if level is null.
     * @throw NoSuchElementException if KIF is not found.
     * @since 0.9
     */
    uint8_t getKif(const AccessLevel& level) const;

    /**
     * Gets the KVC associated to the provided session access level.
     *
     * @param level the session access level (should be not null)
     * @return a not null value
     * @throw IllegalArgumentException if level is null.
     * @throw NoSuchElementException if KVC is not found.
     * @since 0.9
     */
    uint8_t getKvc(const AccessLevel& level) const;

    /**
     * (package-private)<br>
     * Gets a new builder.
     *
     * @return a new builder instance
     */
    static std::unique_ptr<DirectoryHeaderBuilder> builder();

    /**
     * Comparison is based on field "lid".
     *
     * @param o the object to compare
     * @return the comparison evaluation
     * @since 0.9
     */
    bool operator==(const DirectoryHeader& o) const;

    /**
     *
     */
    friend KEYPLECALYPSO_API std::ostream& operator<<(
        std::ostream& os, const DirectoryHeader& dh);

private:
    /**
     *
     */
    const uint8_t mLid;

    /**
     *
     */
    const std::vector<uint8_t> mAccessConditions;

    /**
     *
     */
    const std::vector<uint8_t> mKeyIndexes;

    /**
     *
     */
    const uint8_t mDfStatus;

    /**
     *
     */
    const std::map<AccessLevel, uint8_t> mKif;

    /**
     *
     */
    const std::map<AccessLevel, uint8_t> mKvc;

    /**
     * Private constructor
     */
    DirectoryHeader(const DirectoryHeaderBuilder* builder);
};

}
}
}
