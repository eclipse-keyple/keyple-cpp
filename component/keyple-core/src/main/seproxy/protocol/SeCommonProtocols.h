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
#include <vector>

/* Common */
#include "Export.h"

/* Core */
#include "SeProtocol_Import.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace protocol {

using namespace keyple::core::seproxy::protocol;

class EXPORT SeCommonProtocols final : public SeProtocol {
public:
    /* ---- contactless standard / NFC compliant ------------- */
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

    /**
     *
     */
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

    /**
     *
     */
    const InnerEnum innerEnumValue;

    /**
     * Constructor
     */
    SeCommonProtocols(const std::string& nameValue, InnerEnum innerEnum,
                      const std::string& name,
                      TransmissionMode transmissionMode);

    /**
     *
     */
    SeCommonProtocols(const SeCommonProtocols& s);

    /**
     *
     */
    virtual ~SeCommonProtocols();

    /**
     *
     */
    //SeCommonProtocols& operator=(const SeCommonProtocols& s);

    /**
     *
     */
    std::string getName() const override;

    /**
     *
     */
    TransmissionMode getTransmissionMode() const override;

    /**
     *
     */
    static std::vector<SeCommonProtocols> values();

    /**
     *
     */
    int ordinal();

    /**
     *
     */
    std::string toString();

    /**
     *
     */
    static SeCommonProtocols valueOf(const std::string& name);

private:
    /**
     *
     */
    static std::vector<SeCommonProtocols> valueList;

    /**
     *
     */
    class StaticConstructor {
    public:
        /**
         *
         */
        StaticConstructor();
    };

    /**
     *
     */
    static StaticConstructor staticConstructor;

    /**
     *
     */
    const std::string nameValue;

    /**
     *
     */
    const std::string name;

    /**
     *
     */
    const TransmissionMode transmissionMode;
};

}
}
}
}

namespace std {

using namespace keyple::core::seproxy::protocol;

template <> struct hash<SeCommonProtocols> {
    size_t operator()(const SeCommonProtocols& obj) const
    {
        return hash<int>()(obj.ordinalValue);
    }
};

}
