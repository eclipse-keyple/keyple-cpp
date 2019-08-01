#include "SeCommonProtocols.h"
#include "TransmissionMode.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace protocol {

using TransmissionMore = org::eclipse::keyple::core::seproxy::protocol::TransmissionMode;

SeCommonProtocols SeCommonProtocols::PROTOCOL_ISO14443_4("PROTOCOL_ISO14443_4", InnerEnum::PROTOCOL_ISO14443_4, "ISO 14443-4", TransmissionMode::CONTACTLESS);
SeCommonProtocols SeCommonProtocols::PROTOCOL_ISO15693("PROTOCOL_ISO15693", InnerEnum::PROTOCOL_ISO15693, "ISO 15693 Type V", TransmissionMode::CONTACTLESS);
SeCommonProtocols SeCommonProtocols::PROTOCOL_ISO14443_3A("PROTOCOL_ISO14443_3A", InnerEnum::PROTOCOL_ISO14443_3A, "ISO 14443-3 Type A", TransmissionMode::CONTACTLESS);
SeCommonProtocols SeCommonProtocols::PROTOCOL_ISO14443_3B("PROTOCOL_ISO14443_3B", InnerEnum::PROTOCOL_ISO14443_3B, "ISO 14443-3 Type B", TransmissionMode::CONTACTLESS);
SeCommonProtocols SeCommonProtocols::PROTOCOL_JIS_6319_4("PROTOCOL_JIS_6319_4", InnerEnum::PROTOCOL_JIS_6319_4, "JIS 6319-4 Felica", TransmissionMode::CONTACTLESS);
SeCommonProtocols SeCommonProtocols::PROTOCOL_NDEF("PROTOCOL_NDEF", InnerEnum::PROTOCOL_NDEF, "NFC NDEF TAG", TransmissionMode::CONTACTLESS);
SeCommonProtocols SeCommonProtocols::PROTOCOL_NDEF_FORMATABLE("PROTOCOL_NDEF_FORMATABLE", InnerEnum::PROTOCOL_NDEF_FORMATABLE, "NFC NDEF FORMATABLE", TransmissionMode::CONTACTLESS);
SeCommonProtocols SeCommonProtocols::PROTOCOL_NFC_BARCODE("PROTOCOL_NFC_BARCODE", InnerEnum::PROTOCOL_NFC_BARCODE, "NFC BARCODE", TransmissionMode::CONTACTLESS);
SeCommonProtocols SeCommonProtocols::PROTOCOL_MIFARE_UL("PROTOCOL_MIFARE_UL", InnerEnum::PROTOCOL_MIFARE_UL, "Mifare Ultra Light", TransmissionMode::CONTACTLESS);
SeCommonProtocols SeCommonProtocols::PROTOCOL_MIFARE_CLASSIC("PROTOCOL_MIFARE_CLASSIC", InnerEnum::PROTOCOL_MIFARE_CLASSIC, "Mifare Classic", TransmissionMode::CONTACTLESS);
SeCommonProtocols SeCommonProtocols::PROTOCOL_MIFARE_DESFIRE("PROTOCOL_MIFARE_DESFIRE", InnerEnum::PROTOCOL_MIFARE_DESFIRE, "Mifare Desfire", TransmissionMode::CONTACTLESS);
SeCommonProtocols SeCommonProtocols::PROTOCOL_B_PRIME("PROTOCOL_B_PRIME", InnerEnum::PROTOCOL_B_PRIME, "Old Calypso B Prime", TransmissionMode::CONTACTLESS);
SeCommonProtocols SeCommonProtocols::PROTOCOL_MEMORY_ST25("PROTOCOL_MEMORY_ST25", InnerEnum::PROTOCOL_MEMORY_ST25, "Memory ST25", TransmissionMode::CONTACTLESS);
SeCommonProtocols SeCommonProtocols::PROTOCOL_ISO7816_3("PROTOCOL_ISO7816_3", InnerEnum::PROTOCOL_ISO7816_3, "ISO 7816-3", TransmissionMode::CONTACTS);
SeCommonProtocols SeCommonProtocols::PROTOCOL_HSP("PROTOCOL_HSP", InnerEnum::PROTOCOL_HSP, "Old Calypso SAM HSP", TransmissionMode::CONTACTS);

std::vector<SeCommonProtocols> SeCommonProtocols::valueList;

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

SeCommonProtocols::SeCommonProtocols(const std::string &nameValue, InnerEnum innerEnum, const std::string &name,
                                     TransmissionMode transmissionMode)
: innerEnumValue(innerEnum), nameValue(nameValue), name(name), transmissionMode(transmissionMode)
{
}

SeCommonProtocols::~SeCommonProtocols()
{
}

std::string SeCommonProtocols::getName()
{
    return name;
}

TransmissionMode SeCommonProtocols::getTransmissionMode() const
{
    return transmissionMode;
}

std::vector<SeCommonProtocols> SeCommonProtocols::values()
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

SeCommonProtocols SeCommonProtocols::valueOf(const std::string &name)
{
    for (auto enumInstance : SeCommonProtocols::valueList) {
        if (enumInstance.nameValue == name) {
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
}
}
