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

#include <vector>
#include <memory>

/* Common */
#include "LoggerFactory.h"

/* Core */
#include "KeypleCoreExport.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::common;

/**
 * Single APDU response wrapper
 */
class KEYPLECORE_API ApduResponse {
public:
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
    ApduResponse(const std::vector<uint8_t>& buffer,
                 std::shared_ptr<std::set<int>> successfulStatusCodes);

    /**
     * Checks if is successful.
     *
     * @return the status of the command transmission.
     */
    bool isSuccessful() const;

    /**
     *
     */
    uint16_t getStatusCode() const;

    /**
     *
     */
    const std::vector<uint8_t>& getBytes() const;

    /**
     * Get the data before the statusCode
     *
     * @return slice of the buffer before the status code
     */
    std::vector<uint8_t> getDataOut() const;

    /**
     *
     */
    bool operator==(const ApduResponse& o) const;

    /**
     *
     */
    bool operator!=(const ApduResponse& o) const;

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os,  const ApduResponse& r);

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os,
                                                   const std::shared_ptr<ApduResponse>& r);

	/**
	 *
	 */
    friend KEYPLECORE_API std::ostream& operator<<(
		std::ostream& os, const std::vector<std::shared_ptr<ApduResponse>>& v);

private:
    /***
     * the success result of the processed APDU command to allow chaining responses in a group of
     * APDUs
     */
    bool mSuccessful;

    /*
     *
     */
    const std::shared_ptr<Logger> mLogger =
        LoggerFactory::getLogger(typeid(ApduResponse));

    /**
     * Apdu response data buffer (including sw1sw2)
     */
    const std::vector<uint8_t> mBytes;
};

}
}
}
}
