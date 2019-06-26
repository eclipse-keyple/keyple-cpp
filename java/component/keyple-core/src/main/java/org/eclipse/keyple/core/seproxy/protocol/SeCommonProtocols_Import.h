/********************************************************************************
* Copyright (c) 2019 Calypso Networks Association https://www.calypsonet-asso.org/
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

/* Core */
#include "SeProtocol.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace protocol {

using namespace org::eclipse::keyple::core::seproxy::protocol;
//JAVA TO C++ CONVERTER TODO TASK: The Java 'import static' statement cannot be converted to C++:
//                        import static org.eclipse.keyple.core.seproxy.protocol.TransmissionMode.*;

class IMPORT SeCommonProtocols final : public SeProtocol {

    /* ---- contactless standard / NFC compliant ------------- */
public:
    static SeCommonProtocols PROTOCOL_ISO14443_4;

    static SeCommonProtocols PROTOCOL_ISO15693;

    /* ---- contactless proprietary NFC compliant ------------ */
    static SeCommonProtocols PROTOCOL_ISO14443_3A;

    static SeCommonProtocols PROTOCOL_ISO14443_3B;

    static SeCommonProtocols PROTOCOL_JIS_6319_4;

    static SeCommonProtocols PROTOCOL_NDEF;

    static SeCommonProtocols PROTOCOL_NDEF_FORMATABLE;

    static SeCommonProtocols PROTOCOL_NFC_BARCODE;

    static SeCommonProtocols PROTOCOL_MIFARE_UL;

    static SeCommonProtocols PROTOCOL_MIFARE_CLASSIC;

    static SeCommonProtocols PROTOCOL_MIFARE_DESFIRE;

    /* ---- contactless proprietary not NFC compliant -------- */
    static SeCommonProtocols PROTOCOL_B_PRIME;

    static SeCommonProtocols PROTOCOL_MEMORY_ST25;

    /* ---- contacts ISO standard ---------------------------- */
    static SeCommonProtocols PROTOCOL_ISO7816_3;

    /* ---- contacts proprietary ---------------- */
    static SeCommonProtocols PROTOCOL_HSP;

private:
    static std::vector<SeCommonProtocols> valueList;

    class StaticConstructor {
    public:
        StaticConstructor();
    };

    static StaticConstructor staticConstructor;

public:
    enum class InnerEnum {
        PROTOCOL_ISO14443_4,
        PROTOCOL_ISO15693,
        PROTOCOL_ISO14443_3A,
        PROTOCOL_ISO14443_3B,
        PROTOCOL_JIS_6319_4,
        PROTOCOL_NDEF,
        PROTOCOL_NDEF_FORMATABLE,
        PROTOCOL_NFC_BARCODE,
        PROTOCOL_MIFARE_UL,
        PROTOCOL_MIFARE_CLASSIC,
        PROTOCOL_MIFARE_DESFIRE,
        PROTOCOL_B_PRIME,
        PROTOCOL_MEMORY_ST25,
        PROTOCOL_ISO7816_3,
        PROTOCOL_HSP
    };

    const InnerEnum innerEnumValue;
private:
    const std::string nameValue;
public:
    const int ordinalValue;
private:
    static int nextOrdinal;

private:
    const std::string name;
    const TransmissionMode transmissionMode;

    /**
        * Constructor
        */
public:
    SeCommonProtocols(const std::string &nameValue, InnerEnum innerEnum, const std::string &name, TransmissionMode transmissionMode);

    std::string getName() override;

    TransmissionMode getTransmissionMode() override;

public:
    bool operator==(const SeCommonProtocols &other) const;

    bool operator!=(const SeCommonProtocols &other);

    static std::vector<SeCommonProtocols> values();

    int ordinal();

    std::string toString();

    static SeCommonProtocols valueOf(const std::string &name);
};

}
}
}
}
}
}

namespace std {

using SeCommonProtocols = org::eclipse::keyple::core::seproxy::protocol::SeCommonProtocols;

template<> struct hash<SeCommonProtocols>
{
    size_t operator()(const SeCommonProtocols& obj) const
    {
        return hash<int>()(obj.ordinalValue);
    }
};

}
