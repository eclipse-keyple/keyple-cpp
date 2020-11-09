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
#include <map>
#include <ostream>
#include <vector>

/* Calypso */
#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {
namespace transaction {

/**
 * The class {@code FileData} contains all known data content of a Calypso EF.
 *
 * @since 0.9
 */
class KEYPLECALYPSO_API FileData final {
public:
    /**
     * (package-private)<br>
     * Constructor
     */
    FileData();

    /**
     * Copy constructor
     *
     * C++ vs. Java: Copy constructor is prefereable to .clone() method
     */
    FileData(const FileData& o);

    /**
     * Gets a reference to all known records content.
     *
     * @return a not null map eventually empty if there's no content.
     * @since 0.9
     */
    const std::map<int, std::vector<uint8_t>>& getAllRecordsContent() const;

    /**
     * Gets a reference to the known content of record #1.<br>
     * For a Binary file, it means all the bytes of the file.
     *
     * @return a not empty reference to the record content.
     * @throws NoSuchElementException if record #1 is not set.
     * @since 0.9
     */
    const std::vector<uint8_t>& getContent() const;

    /**
     * Gets a reference to the known content of a specific record.
     *
     * @param numRecord the record number
     * @return a not empty reference to the record content.
     * @throws NoSuchElementException if record #numRecord is not set.
     * @since 0.9
     */
    const std::vector<uint8_t>& getContent(const int numRecord) const;

    /**
     * Gets a copy of a known content subset of a specific record from
     * dataOffset to dataLength.
     *
     * @param numRecord the record number
     * @param dataOffset the offset index (should be {@code >=} 0)
     * @param dataLength the data length (should be {@code >=} 1)
     * @return a copy not empty of the record subset content.
     * @throw IllegalArgumentException if dataOffset {@code <} 0 or dataLength
     *        {@code <} 1.
     * @throw NoSuchElementException if record #numRecord is not set.
     * @throw IndexOutOfBoundsException if dataOffset {@code >=} content length
     *        or (dataOffset + dataLength) {@code >} content length.
     * @since 0.9
     */
    const std::vector<uint8_t> getContent(const int numRecord,
                                          const int dataOffset,
                                          const int dataLength) const;


    /**
     * Gets the known value of the counter #numCounter.<br>
     * The counter value is extracted from the 3 next bytes at the index
     * [(numCounter - 1) * 3] of the record #1.<br>
     * e.g. if numCounter == 2, then value is extracted from bytes indexes
     * [3,4,5].
     *
     * @param numCounter the counter number (should be {@code >=} 1)
     * @return the counter value.
     * @throw IllegalArgumentException if numCounter is {@code <} 1.
     * @throw NoSuchElementException if record #1 or numCounter is not set.
     * @throw IndexOutOfBoundsException if numCounter has a truncated value
     *        (when size of record #1 modulo 3 != 0).
     * @since 0.9
     */
    int getContentAsCounterValue(const int numCounter) const;

    /**
     * Gets all known counters value.<br>
     * The counters values are extracted from record #1.<br>
     * If last counter has a truncated value (when size of record #1 modulo 3
     * != 0), then last counter value is not returned.
     *
     * @return a not empty object.
     * @throw NoSuchElementException if record #1 is not set.
     * @since 0.9
     */
    const std::map<int, int> getAllCountersValue() const;

    /**
     * (package-private)<br>
     * Set or replace the entire content of the specified record #numRecord by
     * the provided content.
     *
     * @param numRecord the record number (should be {@code >=} 1)
     * @param content the content (should be not empty)
     */
    void setContent(const int numRecord, const std::vector<uint8_t>& content);

    /**
     * (package-private)<br>
     * Sets a counter value in record #1.
     *
     * @param numCounter the counter number (should be {@code >=} 1)
     * @param content the counter value (should be not null and 3 bytes length)
     */
    void setCounter(const int numCounter, const std::vector<uint8_t>& content);

    /**
     * (package-private)<br>
     * Set or replace the content at the specified offset of record #numRecord
     * by a copy of the provided content.<br>
     * If actual record content is not set or has a size {@code <} offset, then
     * missing data will be padded with 0.
     *
     * @param numRecord the record number (should be {@code >=} 1)
     * @param content the content (should be not empty)
     * @param offset the offset (should be {@code >=} 0)
     */
    void setContent(const int numRecord,
                    const std::vector<uint8_t>& content,
                    const int offset);

    /**
     * (package-private)<br>
     * Fill the content of the specified #numRecord using a binary OR operation
     * with the provided content.<br>
     * If actual record content is not set or has a size {@code <} content size,
     * then missing data will be completed by the provided content.
     *
     * @param numRecord the record number (should be {@code >=} 1)
     * @param content the content (should be not empty)
     */
    void fillContent(const int numRecord, std::vector<uint8_t>& content);

    /**
     * (package-private)<br>
     * Add cyclic content at record #1 by rolling previously all actual records
     * contents (record #1 -> record #2, record #2 -> record #3,...).<br>
     * This is useful for cyclic files.<br>
     * Note that records are infinitely shifted.
     *
     * @param content the content (should be not empty)
     */
    void addCyclicContent(const std::vector<uint8_t>& content);

    /**
     *
     */
    bool operator==(const FileData& o) const;

    /**
     *
     */
    bool operator!=(const FileData& o) const;

    /**
     *
     */
    friend KEYPLECALYPSO_API std::ostream& operator<<(std::ostream& os,
                                                      const FileData& fd);

private:
    /**
     *
     */
    std::map<int, std::vector<uint8_t>> mRecords;
};

}
}
}
