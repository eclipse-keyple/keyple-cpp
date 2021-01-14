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
#include <string>
#include <vector>

#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {
namespace transaction {

/**
 * The class {@code SvLoadLogRecord} contains the data of a Stored Value load log.
 *
 * @since 0.9
 */
class KEYPLECALYPSO_API SvLoadLogRecord {
public:
    /**
     * Constructor
     *
     * @param poResponse the Sv Get or Read Record (SV Debit log file) response data
     * @param offset the load log offset in the response (may change from a PO to another)
     */
    SvLoadLogRecord(const std::vector<uint8_t>& poResponse, const int offset);

    /**
     * Gets the load amount
     *
     * @return the amount value
     * @since 0.9
     */
    int getAmount() const;

    /**
     * Gets the SV balance
     *
     * @return the balance value
     * @since 0.9
     */
    int getBalance() const;

    /**
     * Gets the load time
     *
     * @return the time value as an int
     * @since 0.9
     */
    int getLoadTime() const;

    /**
     * Gets the load time
     *
     * @return the time value as a 2-byte byte array
     * @since 0.9
     */
    const std::vector<uint8_t> getLoadTimeBytes() const;

    /**
     * Gets the load date
     *
     * @return the date value as an int
     * @since 0.9
     */
    int getLoadDate() const;

    /**
     * Gets the load date
     *
     * @return the date value as a 2-byte byte array
     * @since 0.9
     */
    const std::vector<uint8_t> getLoadDateBytes() const;

    /**
     * Gets the free bytes as a String
     *
     * @return the free value as an Ascii string
     * @since 0.9
     */
    std::string getFreeByte() const;

    /**
     * Gets the free bytes
     *
     * @return the free value as a 2-byte byte array
     * @since 0.9
     */
    const std::vector<uint8_t> getFreeByteBytes() const;

    /**
     * Gets the KVC of the load key (as given in the last SV Reload)
     *
     * @return the KVC value
     * @since 0.9
     */
    uint8_t getKvc() const;
    /**
     * Gets the SAM ID
     *
     * @return the SAM ID value as an int
     * @since 0.9
     */
    uint32_t getSamId() const;

    /**
     * Gets the SAM ID
     *
     * @return the SAM ID value as a 4-byte byte array
     * @since 0.9
     */
    const std::vector<uint8_t> getSamIdBytes() const;

    /**
     * Gets the SV transaction number
     *
     * @return the SV transaction number value as an int
     * @since 0.9
     */
    int getSvTNum() const;

    /**
     * Gets the SV transaction number
     *
     * @return the SV transaction number value as a 2-byte byte array
     * @since 0.9
     */
    const std::vector<uint8_t> getSvTNumBytes() const;

    /**
     * Gets the SAM transaction number
     *
     * @return the SAM transaction number value as an int
     * @since 0.9
     */
    int getSamTNum() const;

    /**
     * Gets the SAM transaction number
     *
     * @return the SAM transaction number value as a 3-byte byte array
     * @since 0.9
     */
    const std::vector<uint8_t> getSamTNumBytes() const;


    /**
     * @return a JSON formatted string with the debit log content
     */
    const std::string toString() const;

private:
    /**
     *
     */
    const int mOffset;

    /**
     *
     */
    const std::vector<uint8_t> mPoResponse;
};

}
}
}
