/********************************************************************************
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information regarding copyright
 * ownership.
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/

#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

#include "stringbuilder.h"

/* Common */
#include "Export.h"
#include "Pattern.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace util {



                /**
                 * Utils around byte arrays
                 */
                class EXPORT ByteArrayUtils : public std::enable_shared_from_this<ByteArrayUtils> {
                    /* byte to hex string conversion table */
                private:
                    static std::vector<std::string> const byteToHex;

                    /**
                     * Chars we will ignore when loading a sample HEX string. It allows to copy/paste the specs APDU
                     */
                    static const Pattern HEX_IGNORED_CHARS;

                    /**
                     * Create a byte array from an hexa string. This method allows spaces and "h".
                     *
                     * @param hex Hexa string
                     * @return byte array
                     */
                public:
                    static std::vector<char> fromHex(const std::string &hex);

                    /**
                     * Represents the byte array in a hexadecimal string.
                     *
                     * @param byteArray byte array to represent to hex
                     * @return Hex representation of the byte array
                     */
                    static std::string toHex(std::vector<char> &byteArray);

                    /**
                     * Convert three bytes from a byte array into an integer.
                     * <p>
                     * The three bytes are expected to be in the MSB first order (aka network order).
                     * <p>
                     * Throw an exception if the buffer is null or not long enough to contain all 3 bytes.
                     * 
                     * @param bytes byte array
                     * @param offset offset from which the 3 bytes are
                     * @return the resulting int
                     * @throws IllegalArgumentException if the buffer has a bad length
                     */
                    static int threeBytesToInt(std::vector<char> &bytes, int offset);
                };

            }
        }
    }
}
