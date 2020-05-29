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

#include <memory>
#include <string>
#include <vector>

/* Core */
#include "KeypleCoreExport.h"
#include "SeProtocol.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace protocol {

using namespace keyple::core::seproxy::protocol;

class KEYPLECORE_API SeCommonProtocols final : public SeProtocol {
public:
    /* ---- contactless standard / NFC compliant ------------- */
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_ISO14443_4;
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_ISO15693;

    /* ---- contactless proprietary NFC compliant ------------ */
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_ISO14443_3A;
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_ISO14443_3B;
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_JIS_6319_4;
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_NDEF;
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_NDEF_FORMATABLE;
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_NFC_BARCODE;
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_MIFARE_UL;
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_MIFARE_CLASSIC;
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_MIFARE_DESFIRE;

    /* ---- contactless proprietary not NFC compliant -------- */
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_B_PRIME;
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_MEMORY_ST25;

    /* ---- contacts ISO standard ---------------------------- */
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_ISO7816_3;

    /* ---- contacts proprietary ---------------- */
    static const std::shared_ptr<SeCommonProtocols> PROTOCOL_HSP;


    /**
     * Constructor
     */
    SeCommonProtocols(const std::string& name,
                      TransmissionMode transmissionMode);

    /**
     *
     */
    const std::string& getName() const override;

    /**
     *
     */
    TransmissionMode getTransmissionMode() const override;

    /**
     *
     */
    static const std::vector<std::shared_ptr<SeCommonProtocols>> values;

private:
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
