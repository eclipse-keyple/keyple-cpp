#include "Protocol.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace protocol {

Protocol Protocol::ANY("ANY", InnerEnum::ANY, "Any protocol");

std::vector<Protocol> Protocol::valueList;

Protocol::StaticConstructor::StaticConstructor() {
    valueList.push_back(ANY);
}

Protocol::StaticConstructor Protocol::staticConstructor;
int Protocol::nextOrdinal = 0;

                    Protocol::Protocol(const std::string &name, InnerEnum innerEnum, const std::string &name) : nameValue(name), ordinalValue(nextOrdinal++), innerEnumValue(innerEnum) {
                        this->name = name;
                    }

                    std::string Protocol::getName() {
                        return name;
                    }

bool Protocol::operator == (const Protocol &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool Protocol::operator != (const Protocol &other) {
    return this->ordinalValue != other.ordinalValue;
}

std::vector<Protocol> Protocol::values() {
    return valueList;
}

int Protocol::ordinal() {
    return ordinalValue;
}

std::string Protocol::toString() {
    return nameValue;
}

Protocol Protocol::valueOf(const std::string &name) {
    for (auto enumInstance : Protocol::valueList) {
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
