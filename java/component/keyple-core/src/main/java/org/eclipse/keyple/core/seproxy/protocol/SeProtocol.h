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
#include <iostream>

#include "TransmissionMode.h"

/* Common */
#include "Export.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace protocol {

using TransmissionMode = org::eclipse::keyple::core::seproxy::protocol::TransmissionMode;

class EXPORT SeProtocol {
public:
    /**
     * Protocol name
     * 
     * @return String
     */

    virtual std::string getName() = 0;

    /**
     * Transmission mode: CONTACTS or CONTACTLESS
     * 
     * @return a TransmissionMode enum value
     */
    virtual TransmissionMode getTransmissionMode() = 0;
    
    /**
     *
     */
    friend std::ostream &operator<<(std::ostream &os, SeProtocol &se)
    {
        os << "name: " << se.getName();

        return os;
    }
};

}
}
}
}
}
}
