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
#include <memory>

/* Common */
#include "Export.h"
#include "stringbuilder.h"
#include "Pattern.h"
#include "Serializable.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::common;

/**
 * Single APDU request wrapper
  */
class EXPORT ApduRequest final
: public std::enable_shared_from_this<ApduRequest>,
  public Serializable {
public:
    /**
     *
     */
    static constexpr long long serialVersionUID = 877369841119873812LL;

    /**
     * the constructor called by a ticketing application in order to build the
     * APDU command requests to push to the ProxyReader.
     *
     * @param buffer Buffer of the APDU request
     * @param case4 the case 4
     * @param successfulStatusCodes the list of status codes to be considered as
     *        successful although different from 9000
     */
    ApduRequest(const std::vector<uint8_t>& buffer, bool case4,
                std::shared_ptr<std::set<int>> successfulStatusCodes);

    /**
     * Alternate constructor with name
     *
     * @param name name to be printed (e.g. in logs)
     * @param buffer data buffer
     * @param case4 case 4 flag (true if case 4)
     * @param successfulStatusCodes the list of status codes to be considered as
     *        successful although different from 9000
     */
    ApduRequest(const std::string& name, const std::vector<uint8_t>& buffer,
                bool case4,
                std::shared_ptr<std::set<int>> successfulStatusCodes);

    /**
     * Alternate constructor without status codes list
     *
     * @param buffer data buffer
     * @param case4 case 4 flag (true if case 4)
     */
    ApduRequest(const std::vector<uint8_t>& buffer, bool case4);

    /**
     * Alternate constructor with name, without status codes list
     *
     * @param name name to be printed (e.g. in logs)
     * @param buffer data buffer
     * @param case4 case 4 flag (true if case 4)
     */
    ApduRequest(const std::string& name, const std::vector<uint8_t>& buffer,
                bool case4);

    /**
     *
     */
    virtual ~ApduRequest()
    {
    }

    /**
     * Checks if is case 4.
     *
     * @return the case4 flag.
     */
    bool isCase4();

    /**
     * Name this APDU request
     *
     * @param name Name of the APDU request
     */
    void setName(const std::string& name);

    /**
     * Get the list of successful status codes for the request
     *
     * @return the list of status codes
     */
    std::shared_ptr<std::set<int>> getSuccessfulStatusCodes();

    /**
     * Get the name of this APDU request
     *
     * @return Name of the APDU request
     */
    std::string getName();

    /**
     * Get the buffer of this APDU
     *
     * @return Name of the APDU request
     */
    std::vector<uint8_t> getBytes();

    /**
     *
     */
    std::string toString() override;

    /**
     *¬
     */
    friend std::ostream& operator<<(std::ostream& os, ApduRequest& se)
    {
        os << se.toString();

        return os;
    }

private:
    /**
     * Buffer of the APDU Request
     */
    std::vector<uint8_t> bytes;

    /**
     * a ‘case 4’ flag in order to explicitly specify, if it’s expected that the APDU command
     * returns data → this flag is required to manage revision 2.4 Calypso Portable Objects and
     * ‘S1Dx’ SAMs that presents a behaviour not compliant with ISO 7816-3 in contacts mode (not
     * returning the 61XYh status).
     */
    const bool case4;

    /**
     * List of status codes that should be considered successful although they are different from
     * 9000
     */
    const std::shared_ptr<std::set<int>> successfulStatusCodes;

    /**
     * Name of the request being sent
     */
    std::string name;
};

}
}
}
}
