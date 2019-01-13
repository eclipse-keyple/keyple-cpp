#include "ContactlessProtocols.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace protocol {

ContactlessProtocols ContactlessProtocols::PROTOCOL_ISO14443_4("PROTOCOL_ISO14443_4", InnerEnum::PROTOCOL_ISO14443_4, "ISO 14443-4");
ContactlessProtocols ContactlessProtocols::PROTOCOL_ISO14443_3A("PROTOCOL_ISO14443_3A", InnerEnum::PROTOCOL_ISO14443_3A, "ISO 14443-3 Type A");
ContactlessProtocols ContactlessProtocols::PROTOCOL_ISO14443_3B("PROTOCOL_ISO14443_3B", InnerEnum::PROTOCOL_ISO14443_3B, "ISO 14443-3 Type B");
ContactlessProtocols ContactlessProtocols::PROTOCOL_B_PRIME("PROTOCOL_B_PRIME", InnerEnum::PROTOCOL_B_PRIME, "Old Calypso B prime");
ContactlessProtocols ContactlessProtocols::PROTOCOL_MIFARE_UL("PROTOCOL_MIFARE_UL", InnerEnum::PROTOCOL_MIFARE_UL, "Mifare Ultra Light");
ContactlessProtocols ContactlessProtocols::PROTOCOL_MIFARE_CLASSIC("PROTOCOL_MIFARE_CLASSIC", InnerEnum::PROTOCOL_MIFARE_CLASSIC, "Mifare Classic");
ContactlessProtocols ContactlessProtocols::PROTOCOL_MIFARE_DESFIRE("PROTOCOL_MIFARE_DESFIRE", InnerEnum::PROTOCOL_MIFARE_DESFIRE, "Mifare DESFire");
ContactlessProtocols ContactlessProtocols::PROTOCOL_MEMORY_ST25("PROTOCOL_MEMORY_ST25", InnerEnum::PROTOCOL_MEMORY_ST25, "Memory ST25");

std::vector<ContactlessProtocols> ContactlessProtocols::valueList;

ContactlessProtocols::StaticConstructor::StaticConstructor() {
    valueList.push_back(PROTOCOL_ISO14443_4);
    valueList.push_back(PROTOCOL_ISO14443_3A);
    valueList.push_back(PROTOCOL_ISO14443_3B);
    valueList.push_back(PROTOCOL_B_PRIME);
    valueList.push_back(PROTOCOL_MIFARE_UL);
    valueList.push_back(PROTOCOL_MIFARE_CLASSIC);
    valueList.push_back(PROTOCOL_MIFARE_DESFIRE);
    valueList.push_back(PROTOCOL_MEMORY_ST25);
}

ContactlessProtocols::StaticConstructor ContactlessProtocols::staticConstructor;
int ContactlessProtocols::nextOrdinal = 0;

                    ContactlessProtocols::ContactlessProtocols(const std::string &nameValue, InnerEnum innerEnum, const std::string &name) : nameValue(nameValue), ordinalValue(nextOrdinal++), innerEnumValue(innerEnum), name(name) {
                        this->name = name;
                    }

                    std::string ContactlessProtocols::getName() {
                        return name;
                    }

bool ContactlessProtocols::operator == (const ContactlessProtocols &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool ContactlessProtocols::operator != (const ContactlessProtocols &other) {
    return this->ordinalValue != other.ordinalValue;
}

std::vector<ContactlessProtocols> ContactlessProtocols::values() {
    return valueList;
}

int ContactlessProtocols::ordinal() {
    return ordinalValue;
}

std::string ContactlessProtocols::toString() {
    return nameValue;
}

ContactlessProtocols ContactlessProtocols::valueOf(const std::string &name) {
    for (auto enumInstance : ContactlessProtocols::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }
}
                }
            }
        }
    }
}
