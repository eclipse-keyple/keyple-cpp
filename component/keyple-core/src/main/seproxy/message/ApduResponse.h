/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
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

#include <set>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
#include <memory>

/* Common */
#include "Object.h"
#include "Serializable.h"
#include "stringhelper.h"
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
class KEYPLECORE_API ApduResponse
: public std::enable_shared_from_this<ApduResponse>, public Object {
public:
    /**
     *
     */
    static constexpr long long serialVersionUID = 6418469841122636812LL;

    /**
     * Create a new ApduResponse from the provided byte array
     *
     * The internal successful status is determined by the current status code
     * and the optional successful status codes list.
     *
     * The list of additional successful status codes is used to set the
     * successful flag if not equal to 0x9000
     *
     * @param buffer apdu response data buffer (including sw1sw2)
     * @param successfulStatusCodes optional list of successful status codes
     *        other than 0x9000
     */
    ApduResponse(std::vector<uint8_t>& buffer,
                 std::shared_ptr<std::set<int>> successfulStatusCodes);

    /**
     *
     */
    virtual ~ApduResponse()
    {
    }

    /**
     * Checks if is successful.
     *
     * @return the status of the command transmission.
     */
    bool isSuccessful() const;

    /**
     *
     */
    int getStatusCode() const;

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
    bool equals(std::shared_ptr<void> o) override;

    /**
     *
     */
    int hashCode() override;

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(std::ostream& os,
                                                   const ApduResponse& r);

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(
        std::ostream& os, const std::shared_ptr<ApduResponse>& r);

	/**
	 *
	 */
    friend KEYPLECORE_API std::ostream& operator<<(
		std::ostream& os, const std::vector<std::shared_ptr<ApduResponse>>& v);

protected:
    /**
     *
     */
    void finalize() override;

private:
    /***
     * the success result of the processed APDU command to allow chaining
     * responses in a group of APDUs
     */
    bool successful;

    /*
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(ApduResponse));

    /**
     * apdu response data buffer (including sw1sw2)
     */
    const std::vector<uint8_t> bytes;
};

}
}
}
}
