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
    = std::make_shared<SeCommonProtocols>("PROTOCOL_ISO14443_4",
                                          InnerEnum::PROTOCOL_ISO14443_4,
                                          "ISO 14443-4",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols>  SeCommonProtocols::PROTOCOL_ISO15693
    = std::make_shared<SeCommonProtocols>("PROTOCOL_ISO15693",
                                          InnerEnum::PROTOCOL_ISO15693,
                                          "ISO 15693 Type V",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_ISO14443_3A
    = std::make_shared<SeCommonProtocols>("PROTOCOL_ISO14443_3A",
                                          InnerEnum::PROTOCOL_ISO14443_3A,
                                          "ISO 14443-3 Type A",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_ISO14443_3B
    = std::make_shared<SeCommonProtocols>("PROTOCOL_ISO14443_3B",
                                          InnerEnum::PROTOCOL_ISO14443_3B,
                                          "ISO 14443-3 Type B",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_JIS_6319_4
    = std::make_shared<SeCommonProtocols>("PROTOCOL_JIS_6319_4",
                                          InnerEnum::PROTOCOL_JIS_6319_4,
                                          "JIS 6319-4 Felica",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_NDEF
    = std::make_shared<SeCommonProtocols>("PROTOCOL_NDEF",
                                          InnerEnum::PROTOCOL_NDEF,
                                          "NFC NDEF TAG",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols>
    SeCommonProtocols::PROTOCOL_NDEF_FORMATABLE
    = std::make_shared<SeCommonProtocols>("PROTOCOL_NDEF_FORMATABLE",
                                          InnerEnum::PROTOCOL_NDEF_FORMATABLE,
                                          "NFC NDEF FORMATABLE",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_NFC_BARCODE
    = std::make_shared<SeCommonProtocols>("PROTOCOL_NFC_BARCODE",
                                          InnerEnum::PROTOCOL_NFC_BARCODE,
                                          "NFC BARCODE",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_MIFARE_UL
    = std::make_shared<SeCommonProtocols>("PROTOCOL_MIFARE_UL",
                                          InnerEnum::PROTOCOL_MIFARE_UL,
                                          "Mifare Ultra Light",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols>
    SeCommonProtocols::PROTOCOL_MIFARE_CLASSIC
    = std::make_shared<SeCommonProtocols>("PROTOCOL_MIFARE_CLASSIC",
                                          InnerEnum::PROTOCOL_MIFARE_CLASSIC,
                                          "Mifare Classic",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols>
    SeCommonProtocols::PROTOCOL_MIFARE_DESFIRE
    = std::make_shared<SeCommonProtocols>("PROTOCOL_MIFARE_DESFIRE",
                                          InnerEnum::PROTOCOL_MIFARE_DESFIRE,
                                          "Mifare Desfire",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_B_PRIME
    = std::make_shared<SeCommonProtocols>("PROTOCOL_B_PRIME",
                                          InnerEnum::PROTOCOL_B_PRIME,
                                          "Old Calypso B Prime",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_MEMORY_ST25
    = std::make_shared<SeCommonProtocols>("PROTOCOL_MEMORY_ST25",
                                          InnerEnum::PROTOCOL_MEMORY_ST25,
                                          "Memory ST25",
                                          TransmissionMode::CONTACTLESS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_ISO7816_3
    = std::make_shared<SeCommonProtocols>("PROTOCOL_ISO7816_3",
                                          InnerEnum::PROTOCOL_ISO7816_3,
                                          "ISO 7816-3",
                                          TransmissionMode::CONTACTS);

const std::shared_ptr<SeCommonProtocols> SeCommonProtocols::PROTOCOL_HSP
    = std::make_shared<SeCommonProtocols>("PROTOCOL_HSP",
                                          InnerEnum::PROTOCOL_HSP,
                                          "Old Calypso SAM HSP",
                                          TransmissionMode::CONTACTS);

std::vector<std::shared_ptr<SeCommonProtocols>> SeCommonProtocols::valueList;

SeCommonProtocols::StaticConstructor::StaticConstructor()
{
    valueList.push_back(PROTOCOL_ISO14443_4);
    valueList.push_back(PROTOCOL_ISO15693);
    valueList.push_back(PROTOCOL_ISO14443_3A);
    valueList.push_back(PROTOCOL_ISO14443_3B);
    valueList.push_back(PROTOCOL_JIS_6319_4);
    valueList.push_back(PROTOCOL_NDEF);
    valueList.push_back(PROTOCOL_NDEF_FORMATABLE);
    valueList.push_back(PROTOCOL_NFC_BARCODE);
    valueList.push_back(PROTOCOL_MIFARE_UL);
    valueList.push_back(PROTOCOL_MIFARE_CLASSIC);
    valueList.push_back(PROTOCOL_MIFARE_DESFIRE);
    valueList.push_back(PROTOCOL_B_PRIME);
    valueList.push_back(PROTOCOL_MEMORY_ST25);
    valueList.push_back(PROTOCOL_ISO7816_3);
    valueList.push_back(PROTOCOL_HSP);
}

SeCommonProtocols::StaticConstructor SeCommonProtocols::staticConstructor;

SeCommonProtocols::SeCommonProtocols(const std::string& nameValue,
                                     InnerEnum innerEnum,
                                     const std::string& name,
                                     TransmissionMode transmissionMode)
: innerEnumValue(innerEnum), nameValue(nameValue), name(name),
  transmissionMode(transmissionMode)
{
}

SeCommonProtocols::SeCommonProtocols(const SeCommonProtocols& s)
: SeProtocol(s), innerEnumValue(s.innerEnumValue), nameValue(s.nameValue),
  name(s.name), transmissionMode(s.transmissionMode)
{
}

std::string SeCommonProtocols::getName() const
{
    return name;
}

TransmissionMode SeCommonProtocols::getTransmissionMode() const
{
    return transmissionMode;
}

const std::vector<std::shared_ptr<SeCommonProtocols>>&
    SeCommonProtocols::values()
{
    return valueList;
}

int SeCommonProtocols::ordinal()
{
    return ordinalValue;
}

std::string SeCommonProtocols::toString()
{
    return nameValue;
}

const std::shared_ptr<SeCommonProtocols>
    SeCommonProtocols::valueOf(const std::string& name)
{
    for (auto enumInstance : SeCommonProtocols::valueList) {
        if (enumInstance->nameValue == name) {
            return enumInstance;
        }
    }

    /* Make compiler happy */
    return SeCommonProtocols::valueList.front();
}

}
}
}
}
