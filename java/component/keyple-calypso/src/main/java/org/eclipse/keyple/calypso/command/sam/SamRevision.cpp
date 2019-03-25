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

std::vector<SamRevision> SamRevision::valueList;

SamRevision::StaticConstructor::StaticConstructor() {
    valueList.push_back(C1);
    valueList.push_back(S1E);
    valueList.push_back(S1D);
    valueList.push_back(AUTO);
}

SamRevision::StaticConstructor SamRevision::staticConstructor;
int SamRevision::nextOrdinal = 0;

                        SamRevision::SamRevision(const std::string &name, InnerEnum innerEnum, const std::string &name, const std::string &applicationTypeMask, char classByte) : nameValue(name), ordinalValue(nextOrdinal++), innerEnumValue(innerEnum) {
                            this->name = name;
                            this->applicationTypeMask = applicationTypeMask;
                            this->classByte = classByte;
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

bool SamRevision::operator == (const SamRevision &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool SamRevision::operator != (const SamRevision &other) {
    return this->ordinalValue != other.ordinalValue;
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
}
                    }
                }
            }
        }
    }
}
