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

#include <string>
#include <iostream>
#include <memory>

#include "TransmissionMode.h"

/* Core */
#include "KeypleCoreExport.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace protocol {

class KEYPLECORE_API SeProtocol {
public:
    /**
     * Protocol name
     *
     * @return String
     */
    virtual const std::string& getName() const = 0;

    /**
     * Transmission mode: CONTACTS or CONTACTLESS
     *
     * @return a TransmissionMode enum value
     */
    virtual const TransmissionMode& getTransmissionMode() const = 0;

    /**
     *
     */
    friend KEYPLECORE_API std::ostream& operator<<(
        std::ostream& os, const std::shared_ptr<SeProtocol>& se);

};

}
}
}
}

