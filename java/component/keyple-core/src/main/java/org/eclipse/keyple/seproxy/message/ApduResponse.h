#pragma once

#include <set>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
#include <memory>

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
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace message {



                    /**
                     * Single APDU response wrapper
                     */
                    class ApduResponse final : public std::enable_shared_from_this<ApduResponse> {

                    public:
                        static constexpr long long serialVersionUID = 6418469841122636812LL;

                        /***
                         * the success result of the processed APDU command to allow chaining responses in a group of
                         * APDUs
                         */
                    private:
                        bool successful;


                        /**
                         * apdu response data buffer (including sw1sw2)
                         */
                        std::vector<char> const bytes;


                        /**
                         * Create a new ApduResponse from the provided byte array
                         *
                         * The internal successful status is determined by the current status code and the optional
                         * successful status codes list.
                         *
                         * The list of additional successful status codes is used to set the successful flag if not
                         * equal to 0x9000
                         *
                         * @param buffer apdu response data buffer (including sw1sw2)
                         * @param successfulStatusCodes optional list of successful status codes other than 0x9000
                         */
                    public:
                        ApduResponse(std::vector<char> &buffer, std::shared_ptr<std::set<int>> successfulStatusCodes);

                        /**
                         * Checks if is successful.
                         *
                         * @return the status of the command transmission.
                         */
                        bool isSuccessful();

                        int getStatusCode();

                        std::vector<char> getBytes();

                        /**
                         * Get the data before the statusCode
                         *
                         * @return slice of the buffer before the status code
                         */
                        std::vector<char> getDataOut();

                        std::string toString();

                        bool equals(std::shared_ptr<void> o);

                        int hashCode();
                    };

                }
            }
        }
    }
}
