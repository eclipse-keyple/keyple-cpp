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

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_ISO14443_4
    = std::make_shared<SeCommonProtocols>("ISO 14443-4",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols>  SeCommonProtocols::PROTOCOL_ISO15693
    = std::make_shared<SeCommonProtocols>("ISO 15693 Type V",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_ISO14443_3A
    = std::make_shared<SeCommonProtocols>("ISO 14443-3 Type A",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_ISO14443_3B
    = std::make_shared<SeCommonProtocols>("ISO 14443-3 Type B",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_JIS_6319_4
    = std::make_shared<SeCommonProtocols>("JIS 6319-4 Felica",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_NDEF
    = std::make_shared<SeCommonProtocols>("NFC NDEF TAG",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols>
    SeCommonProtocols::PROTOCOL_NDEF_FORMATABLE
    = std::make_shared<SeCommonProtocols>("NFC NDEF FORMATABLE",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_NFC_BARCODE
    = std::make_shared<SeCommonProtocols>("NFC BARCODE",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_MIFARE_UL
    = std::make_shared<SeCommonProtocols>("Mifare Ultra Light",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols>
    SeCommonProtocols::PROTOCOL_MIFARE_CLASSIC
    = std::make_shared<SeCommonProtocols>("Mifare Classic",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols>
    SeCommonProtocols::PROTOCOL_MIFARE_DESFIRE
    = std::make_shared<SeCommonProtocols>("Mifare Desfire",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_B_PRIME
    = std::make_shared<SeCommonProtocols>("Old Calypso B Prime",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_MEMORY_ST25
    = std::make_shared<SeCommonProtocols>("Memory ST25",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_ISO7816_3
    = std::make_shared<SeCommonProtocols>("ISO 7816-3",
                                          TransmissionMode::CONTACTS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_HSP
    = std::make_shared<SeCommonProtocols>("Old Calypso SAM HSP",
                                          TransmissionMode::CONTACTS);

const std::vector<std::shared_ptr<SeCommonProtocols>>
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

SeCommonProtocols::SeCommonProtocols(
  const std::string& name, TransmissionMode transmissionMode)
: name(name), transmissionMode(transmissionMode)
{
}

const std::string& SeCommonProtocols::getName() const
{
    return name;
}

TransmissionMode SeCommonProtocols::getTransmissionMode() const
{
    return transmissionMode;
}

}
}
}
}
