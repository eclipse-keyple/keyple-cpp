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
                    static std::string toHex(const std::vector<char> &byteArray);
                };

            }
        }
    }
}
