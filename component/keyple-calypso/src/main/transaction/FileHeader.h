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

#include <cstdint>
#include <memory>
#include <ostream>
#include <vector>

/* Calypso */
#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {
namespace transaction {

/**
 * The class {@code FileHeader} contains all metadata of a Calypso EF.
 *
 * @since 0.9
 */
class KEYPLECALYPSO_API FileHeader final {
public:
    /**
     * The EF type enum
     */
    enum class FileType {
        LINEAR,
        BINARY,
        CYCLIC,
        COUNTERS,
        SIMULATED_COUNTERS
    };

    friend KEYPLECALYPSO_API std::ostream& operator<<(std::ostream& os, const FileType ft);

    /**
     * (package-private)<br>
     * Builder pattern
     */
    class KEYPLECALYPSO_API FileHeaderBuilder final {
    public:
        /**
         * Authorize access to private members
         */
        friend FileHeader;

        /**
         * (package-private)<br>
         * Sets the LID.
         *
         * @param lid the LID
         * @return the builder instance
         */
        FileHeaderBuilder& lid(const uint16_t lid) ;

        /**
         * (package-private)<br>
         * Sets the number of records.
         *
         * @param recordsNumber the number of records (should be {@code >=} 1)
         * @return the builder instance
         */
        FileHeaderBuilder& recordsNumber(const int recordsNumber);

        /**
         * (package-private)<br>
         * Sets the size of a record.
         *
         * @param recordSize the size of a record (should be {@code >=} 1)
         * @return the builder instance
         */
        FileHeaderBuilder& recordSize(const int recordSize);

        /**
         * (package-private)<br>
         * Sets the file type.
         *
         * @param type the file type (should be not null)
         * @return the builder instance
         */
        FileHeaderBuilder& type(const FileType type);

        /**
         * (package-private)<br>
         * Sets a reference to the provided access conditions byte array.
         *
         * @param accessConditions the access conditions (should be not null and
         *        4 bytes length)
         * @return the builder instance
         */
        FileHeaderBuilder& accessConditions(const std::vector<uint8_t>& accessConditions);

        /**
         * (package-private)<br>
         * Sets a reference to the provided key indexes byte array.
         *
         * @param keyIndexes the key indexes (should be not null and 4 bytes
         *        length)
         * @return the builder instance
         */
        FileHeaderBuilder& keyIndexes(const std::vector<uint8_t>& keyIndexes);

        /**
         * (package-private)<br>
         * Sets the DF status.
         *
         * @param dfStatus the DF status (byte)
         * @return the builder instance
         */
        FileHeaderBuilder& dfStatus(const uint8_t dfStatus);

        /**
         * (package-private)<br>
         * Sets the shared reference.
         *
         * @param sharedReference the shared reference
         * @return the builder instance
         */
        FileHeaderBuilder& sharedReference(const uint16_t sharedReference);

        /**
         * (package-private)<br>
         * Build a new {@code FileHeader}.
         *
         * @return a new instance
         */
        std::unique_ptr<FileHeader> build();

    private:
        /**
         *
         */
        uint16_t mLid;

        /**
         *
         */
        int mRecordsNumber;

        /**
         *
         */
        int mRecordSize;

        /**
         *
         */
        FileType mType;

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
        uint16_t mSharedReference;

        /**
         * C++ vs. Java: add boolean to avoid shared_ptr on mSharedReference
         */
        bool mShared;

        /**
         * Private constructor
         */
        FileHeaderBuilder();
    };

    /**
     * Gets the associated LID.
     *
     * @return the LID
     * @since 0.9
     */
    uint16_t getLid() const;

    /**
     * Gets the number of records :
     * <ul>
     * <li>For a Counter file, the number of records is always 1.<br>
     * Extra bytes (rest of the division of the file size by 3) aren't accessible.</li>
     * <li>For a Binary file, the number of records is always 1.</li>
     * </ul>
     *
     * @return the number of records
     * @since 0.9
     */
    int getRecordsNumber() const;

    /**
     * Gets the size of a record :
     * <ul>
     * <li>For a Counter file, the record size is the original size of the
     * record #1.<br>
     * Extra bytes (rest of the division of the file size by 3) aren't accessible.</li>
     * <li>For a Binary file, the size of the record is corresponding to the
     * file size.</li>
     * </ul>
     *
     * @return the size of a record
     * @since 0.9
     */
    int getRecordSize() const;

    /**
     * Gets the file type.
     *
     * @return a not null file type
     * @since 0.9
     */
    FileType getType() const;

    /**
     * Gets a reference to the access conditions.
     *
     * @return a not empty byte array reference
     * @since 0.9
     */
    const std::vector<uint8_t>& getAccessConditions() const;

    /**
     * Gets a reference to the keys indexes.
     *
     * @return a not empty byte array reference
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
     * Returns true if EF is a shared file.
     *
     * @return true if the EF is a shared file
     * @since 0.9
     */
    bool isShared() const;

    /**
     * Gets the shared reference of a shared file.
     *
     * @return null if file is not shared
     * @since 0.9
     */
    uint16_t getSharedReference() const;

    /**
     * (package-private)<br>
     * Gets a new builder.
     *
     * @return a new builder instance
     */
    static std::unique_ptr<FileHeaderBuilder> builder();


    /**
     * Comparison is based on field "lid".
     *
     * @param o the object to compare
     * @return the comparison evaluation
     * @since 0.9
     */
    bool operator==(const FileHeader& o) const;

    /**
     *
     */
    friend KEYPLECALYPSO_API std::ostream& operator<<(std::ostream& os, const FileHeader& fh);

private:
    /**
     *
     */
    const uint16_t mLid;

    /**
     *
     */
    const int mRecordsNumber;

    /**
     *
     */
    const int mRecordSize;

    /**
     *
     */
    const FileType mType;

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
    const uint16_t mSharedReference;

    /**
     * C++ vs. Java: add boolean to avoid shared_ptr on mSharedReference
     */
    const bool mShared;

    /**
     * Private constructor
     */
    FileHeader(FileHeaderBuilder* builder);
};

}
}
}
