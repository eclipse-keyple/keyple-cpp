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

namespace keyple {
namespace core {
namespace seproxy {
namespace protocol {

using namespace keyple::core::seproxy::protocol;

class EXPORT SeProtocol {
public:
    /**
     *
     */
    const int ordinalValue;

    /**
     *
     */
    SeProtocol() : ordinalValue(nextOrdinal++)
    {
    }

    /**
     *
     */
    SeProtocol(const SeProtocol& other) : ordinalValue(other.ordinalValue)
    {
    }

    /**
     *
     */
    virtual ~SeProtocol()
    {
    }

    /**
     * Protocol name
     * 
     * @return String
     */
    virtual std::string getName() const
    {
        return "";
    }

    /**
     * Transmission mode: CONTACTS or CONTACTLESS
     * 
     * @return a TransmissionMode enum value
     */
    virtual TransmissionMode getTransmissionMode() const
    {
        return TransmissionMode::NO_MODE;
    }

    /**
     *
     */
    virtual bool operator==(const SeProtocol& other) const
    {
        return this->ordinalValue == other.ordinalValue;
    }

    /**
     *
     */
    virtual bool operator!=(const SeProtocol& other) const
    {
        return !(*this == other);
    }

    /**
     *
     */
    virtual bool operator<(const SeProtocol& other) const
    {
        return this->ordinalValue < other.ordinalValue;
    }

    /**
     *
     */
    friend std::ostream& operator<<(std::ostream& os, const SeProtocol& se);

    /**
     * 
     */
    std::string toString() const
    {
        return this->getName();
    }

private:
    /**
     *
     */
    static int nextOrdinal;
};

}
}
}
}
namespace std {

using namespace keyple::core::seproxy::protocol;

template <> struct hash<SeProtocol> {
    size_t operator()(const SeProtocol& obj) const
    {
        (void)obj;

        return hash<int>()(obj.ordinalValue);
    }
};

}
