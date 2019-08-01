#include "PoClass.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {

PoClass PoClass::LEGACY("LEGACY", InnerEnum::LEGACY, static_cast<char>(0x94));
PoClass PoClass::ISO("ISO", InnerEnum::ISO, static_cast<char>(0x00));

std::vector<PoClass> PoClass::valueList;

PoClass::StaticConstructor::StaticConstructor() {
    valueList.push_back(LEGACY);
    valueList.push_back(ISO);
}

PoClass::StaticConstructor PoClass::staticConstructor;
int PoClass::nextOrdinal = 0;

char PoClass::getValue() {
    return cla;
}

PoClass::PoClass(const std::string &name, InnerEnum innerEnum, char cla) : innerEnumValue(innerEnum), nameValue(name), ordinalValue(nextOrdinal++), cla(cla)
{

}

bool PoClass::operator == (const PoClass &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool PoClass::operator != (const PoClass &other) {
    return this->ordinalValue != other.ordinalValue;
}

std::vector<PoClass> PoClass::values() {
    return valueList;
}

int PoClass::ordinal() {
    return ordinalValue;
}

std::string PoClass::toString() {
    return nameValue;
}

PoClass PoClass::valueOf(const std::string &name) {
    for (auto enumInstance : PoClass::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    /* Compiler fix */
    return PoClass("Dummy", InnerEnum::ISO, 0);
}
}
}
}
}
}
