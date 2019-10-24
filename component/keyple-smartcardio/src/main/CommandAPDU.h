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

/* Common */
#include "Export.h"

namespace keyple {
namespace smartcardio {

class EXPORT CommandAPDU {
public:
    /**
     * Constructor
     *
     * Constructs a CommandAPDU from a byte array containing the complete APDU
     * contents (header and body).
     *
     * Note that the apdu bytes are copied to protect against subsequent
     * modification.
     *
     * @param apdu the complete command APDU
     * @throws NullPointerException if apdu is null
     * @throws IllegalArgumentException if apdu does not contain a valid command APDU
     */
    CommandAPDU(std::vector<char> &apdu) : apdu(apdu)
    {

    }

    /**
     * Destructor
     */
    ~CommandAPDU()
    {

    }

    /**
     * Retrieve APDU data
     */
    std::vector<char> getBytes()
    {
        std::vector<char> copy = apdu;
        return copy;
    }

private:
    /**
     * Current APDU
     */
    std::vector<char> apdu;
};

}
}

