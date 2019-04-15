#include "CustomProtocols.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic {
                    namespace common {

                        using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;

CustomProtocols CustomProtocols::CUSTOM_PROTOCOL_B_PRIME("CUSTOM_PROTOCOL_B_PRIME", InnerEnum::CUSTOM_PROTOCOL_B_PRIME, "Custom Old Calypso B prime");
CustomProtocols CustomProtocols::CUSTOM_PROTOCOL_MIFARE_DESFIRE("CUSTOM_PROTOCOL_MIFARE_DESFIRE", InnerEnum::CUSTOM_PROTOCOL_MIFARE_DESFIRE, "Custom Mifare DESFire");

std::vector<CustomProtocols> CustomProtocols::valueList;

CustomProtocols::StaticConstructor::StaticConstructor() {
    valueList.push_back(CUSTOM_PROTOCOL_B_PRIME);
    valueList.push_back(CUSTOM_PROTOCOL_MIFARE_DESFIRE);
}

CustomProtocols::StaticConstructor CustomProtocols::staticConstructor;
int CustomProtocols::nextOrdinal = 0;

                        CustomProtocols::CustomProtocols(const std::string &nameValue, InnerEnum innerEnum, const std::string &name) : innerEnumValue(innerEnum), nameValue(nameValue), ordinalValue(nextOrdinal++) {
                            this->name = name;
                        }

                        std::string CustomProtocols::getName() {
                            return name;
                        }

bool CustomProtocols::operator == (const CustomProtocols &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool CustomProtocols::operator != (const CustomProtocols &other) {
    return this->ordinalValue != other.ordinalValue;
}

std::vector<CustomProtocols> CustomProtocols::values() {
    return valueList;
}

int CustomProtocols::ordinal() {
    return ordinalValue;
}

std::string CustomProtocols::toString() {
    return nameValue;
}

CustomProtocols CustomProtocols::valueOf(const std::string &name) {
    for (auto enumInstance : CustomProtocols::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    /* Make compiler happy */
    return valueList.front();
}

                    }
                }
            }
        }
    }
}
