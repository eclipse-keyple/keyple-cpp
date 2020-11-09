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

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

/* Common */
#include "stringbuilder.h"
#include "Pattern.h"

/* Core */
#include "KeypleCoreExport.h"

namespace keyple {
namespace core {
namespace util {

using namespace keyple::common;

/**
 * Utils around byte arrays
 */
class KEYPLECORE_API ByteArrayUtil {
public:
    /**
     * Create a byte array from an hexa string. This method allows spaces and "h".
     *
     * @param hex Hexa string
     * @return byte array
     */
    static std::vector<uint8_t> fromHex(const std::string& hex);

    /**
     * Represents the byte array in a hexadecimal string.
     *
     * @param byteArray byte array to represent to hex
     * @return Hex representation of the byte array
     */
    static std::string toHex(const std::vector<char>& byteArray);

    /**
     * Represents the byte array in a hexadecimal string.
     *
     * @param byteArray byte array to represent to hex
     * @return Hex representation of the byte array
     */
    static std::string toHex(const std::vector<uint8_t>& byteArray);

    /**
     * Convert two bytes (unsigned) from a byte array into an integer.
     * <p>
     * The two bytes are expected to be in the MSB first order (aka network order).
     * <p>
     * Throw an exception if the buffer is null or not long enough to contain 2 bytes.
     *
     * @param bytes byte array
     * @param offset offset from which the 2 bytes are
     * @return the resulting int
     * @throw IllegalArgumentException if the buffer has a bad length
     */
    static int twoBytesToInt(const std::vector<uint8_t>& bytes, const int offset);

    /**
     * Convert two bytes (signed) from a byte array into an integer.
     * <p>
     * The two bytes are expected to be in the MSB first order (aka network order).
     * <p>
     * The number is also considered as signed. That is, if the MSB (first left bit) is 1, then the
     * number is negative and the conversion is done accordingly with the usual binary arithmetic.
     * <p>
     * Throw an exception if the buffer is null or not long enough to contain 2 bytes.
     *
     * @param bytes byte array
     * @param offset offset from which the 2 bytes are
     * @return the resulting int
     * @throw IllegalArgumentException if the buffer has a bad length
     */
    static int twoBytesSignedToInt(const std::vector<uint8_t>& bytes, const int offset);

    /**
     * Convert three bytes from a byte array into an integer.
     * <p>
     * The three bytes are expected to be in the MSB first order (aka network
     * order).
     * <p>
     * Throw an exception if the buffer is null or not long enough to contain all 3 bytes.
     *
     * @param bytes byte array
     * @param offset offset from which the 3 bytes are
     * @return the resulting int
     * @throw IllegalArgumentException if the buffer has a bad length
     */
    static int threeBytesToInt(const std::vector<uint8_t>& bytes, const int offset);

    /**
     * Convert three bytes (signed) from a byte array into an integer.
     * <p>
     * The three bytes are expected to be in the MSB first order (aka network order).
     * <p>
     * The number is also considered as signed. That is, if the MSB (first left bit) is 1, then the
     * number is negative and the conversion is done accordingly with the usual binary arithmetic.
     * <p>
     * Throw an exception if the buffer is null or not long enough to contain all 3 bytes.
     *
     * @param bytes byte array containing a 3-byte signed number
     * @param offset offset from which the 3 bytes are
     * @return the resulting int
     * @throw IllegalArgumentException if the buffer has a bad length
     */
    static int threeBytesSignedToInt(const std::vector<uint8_t>& bytes, const int offset);

    /**
     * Convert four bytes from a byte array into an integer.
     * <p>
     * The four bytes are expected to be in the MSB first order (aka network order).
     * <p>
     * Throw an exception if the buffer is null or not long enough to contain 4 bytes.
     *
     * @param bytes byte array
     * @param offset offset from which the 4 bytes are
     * @return the resulting int
     * @throw IllegalArgumentException if the buffer has a bad length
     */
    static int fourBytesToInt(const std::vector<uint8_t>& bytes, const int offset);

private:
    /**
     * Byte to hex string conversion table
     */
    static std::vector<std::string> const byteToHex;

    /**
     * Chars we will ignore when loading a sample HEX string. It allows to
     * copy/paste the specs APDU
     */
    static Pattern* HEX_IGNORED_CHARS;
};

}
}
}
