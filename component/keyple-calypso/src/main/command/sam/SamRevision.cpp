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

#include "SamRevision.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

SamRevision SamRevision::C1("C1", InnerEnum::C1, "C1", "C1",
                            static_cast<char>(0x80));
SamRevision SamRevision::S1E("S1E", InnerEnum::S1E, "S1E", "E1",
                             static_cast<char>(0x80));
SamRevision SamRevision::S1D("S1D", InnerEnum::S1D, "S1D", "D?",
                             static_cast<char>(0x94));
SamRevision SamRevision::AUTO("AUTO", InnerEnum::AUTO, "AUTO", "??",
                              static_cast<char>(0x00));
SamRevision SamRevision::NO_REV("NO_REV", InnerEnum::NO_REV, "NO_REV", "??",
                                static_cast<char>(0x00));

std::vector<SamRevision> SamRevision::valueList;

SamRevision::StaticConstructor::StaticConstructor()
{
    valueList.push_back(NO_REV);
    valueList.push_back(C1);
    valueList.push_back(S1E);
    valueList.push_back(S1D);
    valueList.push_back(AUTO);
}

SamRevision::StaticConstructor SamRevision::staticConstructor;
int SamRevision::nextOrdinal = 0;

SamRevision::SamRevision(const std::string& nameValue, InnerEnum innerEnum,
                         const std::string& name,
                         const std::string& applicationTypeMask, char classByte)
: innerEnumValue(innerEnum), nameValue(nameValue), ordinalValue(nextOrdinal++),
  name(name), applicationTypeMask(applicationTypeMask), classByte(classByte)
{
}

SamRevision::SamRevision(const SamRevision& o)
: innerEnumValue(o.innerEnumValue), nameValue(o.nameValue),
  ordinalValue(o.ordinalValue), name(o.name),
  applicationTypeMask(o.applicationTypeMask), classByte(o.classByte)
{
}

std::string SamRevision::getName()
{
    return name;
}

const std::string& SamRevision::getApplicationTypeMask() const
{
    return applicationTypeMask;
}

char SamRevision::getClassByte()
{
    return classByte;
}

bool SamRevision::operator==(const SamRevision& other) const
{
    return this->ordinalValue == other.ordinalValue;
}

bool SamRevision::operator!=(const SamRevision& other) const
{
    return this->ordinalValue != other.ordinalValue;
}

SamRevision& SamRevision::operator=(SamRevision o)
{
    this->innerEnumValue      = o.innerEnumValue;
    this->nameValue           = o.nameValue;
    this->name                = o.name;
    this->ordinalValue        = o.ordinalValue;
    this->applicationTypeMask = o.applicationTypeMask;
    this->classByte           = o.classByte;

    return *this;
}

std::vector<SamRevision> SamRevision::values()
{
    return valueList;
}

int SamRevision::ordinal()
{
    return ordinalValue;
}

std::string SamRevision::toString()
{
    return nameValue;
}

SamRevision SamRevision::valueOf(const std::string& name)
{
    for (auto enumInstance : SamRevision::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    /* Compiler fix */
    return SamRevision("Dummy", InnerEnum::AUTO, "Dummy", "Dummy", 0);
}

std::ostream& operator<<(std::ostream& os, const SamRevision& sr)
{
    if (sr == SamRevision::NO_REV)
        os << "NO_REV";
    else if (sr == SamRevision::C1)
        os << "C1";
    else if (sr == SamRevision::S1E)
        os << "S1E";
	else if (sr == SamRevision::S1D)
        os << "S1D";
	else if (sr == SamRevision::AUTO)
        os << "AUTO";
    return os;
}

}
}
}
}
