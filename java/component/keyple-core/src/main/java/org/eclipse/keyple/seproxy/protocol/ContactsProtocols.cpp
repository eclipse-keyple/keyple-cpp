#include "ContactsProtocols.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace protocol {

ContactsProtocols ContactsProtocols::PROTOCOL_ISO7816_3("PROTOCOL_ISO7816_3", InnerEnum::PROTOCOL_ISO7816_3, "ISO 7816-3");

std::vector<ContactsProtocols> ContactsProtocols::valueList;

ContactsProtocols::StaticConstructor::StaticConstructor() {
    valueList.push_back(PROTOCOL_ISO7816_3);
}

ContactsProtocols::StaticConstructor ContactsProtocols::staticConstructor;
int ContactsProtocols::nextOrdinal = 0;

                    ContactsProtocols::ContactsProtocols(const std::string &name, InnerEnum innerEnum, const std::string &name) : nameValue(name), ordinalValue(nextOrdinal++), innerEnumValue(innerEnum) {
                        this->name = name;
                    }

                    std::string ContactsProtocols::getName() {
                        return name;
                    }

bool ContactsProtocols::operator == (const ContactsProtocols &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool ContactsProtocols::operator != (const ContactsProtocols &other) {
    return this->ordinalValue != other.ordinalValue;
}

std::vector<ContactsProtocols> ContactsProtocols::values() {
    return valueList;
}

int ContactsProtocols::ordinal() {
    return ordinalValue;
}

std::string ContactsProtocols::toString() {
    return nameValue;
}

ContactsProtocols ContactsProtocols::valueOf(const std::string &name) {
    for (auto enumInstance : ContactsProtocols::valueList) {
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
