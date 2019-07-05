#include "SamRevision.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace command {
namespace sam {

SamRevision SamRevision::C1("C1", InnerEnum::C1, "C1", "C1", static_cast<char>(0x80));
SamRevision SamRevision::S1E("S1E", InnerEnum::S1E, "S1E", "E1", static_cast<char>(0x80));
SamRevision SamRevision::S1D("S1D", InnerEnum::S1D, "S1D", "D?", static_cast<char>(0x94));
SamRevision SamRevision::AUTO("AUTO", InnerEnum::AUTO, "AUTO", "??", static_cast<char>(0x00));
SamRevision SamRevision::NO_REV("NO_REV", InnerEnum::NO_REV, "NO_REV", "??", static_cast<char>(0x00));

std::vector<SamRevision> SamRevision::valueList;

SamRevision::StaticConstructor::StaticConstructor() {
    valueList.push_back(NO_REV);
    valueList.push_back(C1);
    valueList.push_back(S1E);
    valueList.push_back(S1D);
    valueList.push_back(AUTO);
}

SamRevision::StaticConstructor SamRevision::staticConstructor;
int SamRevision::nextOrdinal = 0;

SamRevision::SamRevision(const std::string &nameValue, InnerEnum innerEnum, const std::string &name, const std::string &applicationTypeMask, char classByte)
    : innerEnumValue(innerEnum), nameValue(nameValue), ordinalValue(nextOrdinal++), name(name), applicationTypeMask(applicationTypeMask), classByte(classByte)
{
}

std::string SamRevision::getName() {
    return name;
}

std::string SamRevision::getApplicationTypeMask() {
    return applicationTypeMask;
}

char SamRevision::getClassByte() {
    return classByte;
}

bool SamRevision::operator==(const SamRevision &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool SamRevision::operator!=(const SamRevision &other) {
    return this->ordinalValue != other.ordinalValue;
}

SamRevision& SamRevision::operator=(const SamRevision& other)
{
    this->name = other.name;
    this->nameValue = other.nameValue;
    this->ordinalValue = other.ordinalValue;
    this->classByte = other.classByte;
    this->applicationTypeMask = other.applicationTypeMask;
    this->innerEnumValue = other.innerEnumValue;

    return *this;
}

std::vector<SamRevision> SamRevision::values() {
    return valueList;
}

int SamRevision::ordinal() {
    return ordinalValue;
}

std::string SamRevision::toString() {
    return nameValue;
}

SamRevision SamRevision::valueOf(const std::string &name) {
    for (auto enumInstance : SamRevision::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    /* Compiler fix */
    return SamRevision("Dummy", InnerEnum::AUTO, "Dummy", "Dummy", 0);
}

}
}
}
}
}
}
