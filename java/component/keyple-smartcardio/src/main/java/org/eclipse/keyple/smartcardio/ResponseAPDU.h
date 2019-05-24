/*
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* All rights reserved. This program and the accompanying materials are made available under the
* terms of the Eclipse Public License version 2.0 which accompanies this distribution, and is
* available at https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*/

#pragma once

#include <cstdint>
#include <vector>

namespace org {
namespace eclipse {
namespace keyple {
namespace smartcardio {

/*
 *
 */
class ResponseAPDU {
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
}
}

