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
#include <vector>

/* Common */
#include "Export.h"

/* Core*/
#include "SeProtocol.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace protocol {

class EXPORT ContactlessProtocols final : public SeProtocol {

    /** Contactless standard */
public:
    static ContactlessProtocols PROTOCOL_ISO14443_4; // equivalent IsoDep in Android NFC

    /** Contactless proprietary solutions */
    static ContactlessProtocols PROTOCOL_ISO14443_3A;
    static ContactlessProtocols PROTOCOL_ISO14443_3B;
    static ContactlessProtocols PROTOCOL_B_PRIME;

    /** Mifare Ultralight and Ultralight C */
    static ContactlessProtocols PROTOCOL_MIFARE_UL;

    /** Mifare mini, 1K, 2K, 4K */
    static ContactlessProtocols PROTOCOL_MIFARE_CLASSIC;
    static ContactlessProtocols PROTOCOL_MIFARE_DESFIRE;
    static ContactlessProtocols PROTOCOL_MEMORY_ST25;

private:
    static std::vector<ContactlessProtocols> valueList;

    class StaticConstructor {
    public:
        StaticConstructor();
    };

    static StaticConstructor staticConstructor;

public:
    enum class InnerEnum {
        PROTOCOL_ISO14443_4,
        PROTOCOL_ISO14443_3A,
        PROTOCOL_ISO14443_3B,
        PROTOCOL_B_PRIME,
        PROTOCOL_MIFARE_UL,
        PROTOCOL_MIFARE_CLASSIC,
        PROTOCOL_MIFARE_DESFIRE,
        PROTOCOL_MEMORY_ST25
    };

    const InnerEnum innerEnumValue;
private:
    const std::string nameValue;
    const int ordinalValue;
    static int nextOrdinal;

    /** The protocol name. */
private:
    std::string name;

public:
    ContactlessProtocols(const std::string &nameValue, InnerEnum innerEnum, const std::string &name);

    virtual std::string getName();

public:
    bool operator == (const ContactlessProtocols &other);

    bool operator != (const ContactlessProtocols &other);

    static std::vector<ContactlessProtocols> values();

    int ordinal();

    std::string toString();

    static ContactlessProtocols valueOf(const std::string &name);
};

}
}
}
}
}
}
