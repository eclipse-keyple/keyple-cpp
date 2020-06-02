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

#include "SeCommonProtocols.h"
#include "TransmissionMode.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace protocol {

using namespace keyple::core::seproxy::protocol;

using SeCommonProtocol = SeCommonProtocols::SeCommonProtocol;

const std::shared_ptr<SeCommonProtocol> SeCommonProtocols::PROTOCOL_ISO14443_4
    = std::make_shared<SeCommonProtocol>("ISO 14443-4",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocol>  SeCommonProtocols::PROTOCOL_ISO15693
    = std::make_shared<SeCommonProtocol>("ISO 15693 Type V",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocol> SeCommonProtocols::PROTOCOL_ISO14443_3A
    = std::make_shared<SeCommonProtocol>("ISO 14443-3 Type A",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocol> SeCommonProtocols::PROTOCOL_ISO14443_3B
    = std::make_shared<SeCommonProtocol>("ISO 14443-3 Type B",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocol> SeCommonProtocols::PROTOCOL_JIS_6319_4
    = std::make_shared<SeCommonProtocol>("JIS 6319-4 Felica",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocol> SeCommonProtocols::PROTOCOL_NDEF
    = std::make_shared<SeCommonProtocol>("NFC NDEF TAG",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocol>
    SeCommonProtocols::PROTOCOL_NDEF_FORMATABLE
    = std::make_shared<SeCommonProtocol>("NFC NDEF FORMATABLE",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocol> SeCommonProtocols::PROTOCOL_NFC_BARCODE
    = std::make_shared<SeCommonProtocol>("NFC BARCODE",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocol> SeCommonProtocols::PROTOCOL_MIFARE_UL
    = std::make_shared<SeCommonProtocol>("Mifare Ultra Light",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocol>
    SeCommonProtocols::PROTOCOL_MIFARE_CLASSIC
    = std::make_shared<SeCommonProtocol>("Mifare Classic",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocol>
    SeCommonProtocols::PROTOCOL_MIFARE_DESFIRE
    = std::make_shared<SeCommonProtocol>("Mifare Desfire",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocol> SeCommonProtocols::PROTOCOL_B_PRIME
    = std::make_shared<SeCommonProtocol>("Old Calypso B Prime",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocol> SeCommonProtocols::PROTOCOL_MEMORY_ST25
    = std::make_shared<SeCommonProtocol>("Memory ST25",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocol> SeCommonProtocols::PROTOCOL_ISO7816_3
    = std::make_shared<SeCommonProtocol>("ISO 7816-3",
                                          TransmissionMode::CONTACTS);

const std::shared_ptr<SeCommonProtocol> SeCommonProtocols::PROTOCOL_HSP
    = std::make_shared<SeCommonProtocol>("Old Calypso SAM HSP",
                                          TransmissionMode::CONTACTS);

const std::vector<std::shared_ptr<SeCommonProtocol>>
    SeCommonProtocols::values = {
    SeCommonProtocols::PROTOCOL_ISO14443_4,
    SeCommonProtocols::PROTOCOL_ISO15693,
    SeCommonProtocols::PROTOCOL_ISO14443_3A,
    SeCommonProtocols::PROTOCOL_ISO14443_3B,
    SeCommonProtocols::PROTOCOL_JIS_6319_4,
    SeCommonProtocols::PROTOCOL_NDEF,
    SeCommonProtocols::PROTOCOL_NDEF_FORMATABLE,
    SeCommonProtocols::PROTOCOL_NFC_BARCODE,
    SeCommonProtocols::PROTOCOL_MIFARE_UL,
    SeCommonProtocols::PROTOCOL_MIFARE_CLASSIC,
    SeCommonProtocols::PROTOCOL_MIFARE_DESFIRE,
    SeCommonProtocols::PROTOCOL_B_PRIME,
    SeCommonProtocols::PROTOCOL_MEMORY_ST25,
    SeCommonProtocols::PROTOCOL_ISO7816_3,
    SeCommonProtocols::PROTOCOL_HSP
};

SeCommonProtocol::SeCommonProtocol(
  const std::string& name, const TransmissionMode& transmissionMode)
: name(name), transmissionMode(transmissionMode)
{
}

const std::string& SeCommonProtocol::getName() const
{
    return name;
}

const TransmissionMode& SeCommonProtocol::getTransmissionMode() const
{
    return transmissionMode;
}

}
}
}
}
