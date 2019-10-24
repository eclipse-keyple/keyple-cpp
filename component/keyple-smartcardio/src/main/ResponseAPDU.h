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

/*
 *
 */
class EXPORT ResponseAPDU {
public:
    /**
     * Constructor
     *
     * Constructs a ResponseAPDU from a byte array containing the
     * complete APDU contents (conditional body and trailed).
     *
     * @param apdu the complete response APDU
     * @throws NullPointerException if apdu is null
     * @throws IllegalArgumentException if apdu.length is less than 2
     */
    ResponseAPDU(std::vector<char> &apdu) : apdu(apdu)
    {

    }

    /**
     * Destructor
     */
    ~ResponseAPDU()
    {

    }

    /**
     * Returns a copy of the bytes in this APDU.
     *
     * &return a copy of the bytes in this APDU.
     */
    std::vector<char> &getBytes()
    {
        return apdu;
    }

private:
    /*
     * Current APDU
     */
    std::vector<char> apdu;

};

}
}

