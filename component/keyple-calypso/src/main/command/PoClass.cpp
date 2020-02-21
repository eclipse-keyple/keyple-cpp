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

#include "PoClass.h"

namespace keyple {
namespace calypso {
namespace command {

PoClass PoClass::LEGACY("LEGACY", InnerEnum::LEGACY, static_cast<char>(0x94));
PoClass PoClass::ISO("ISO", InnerEnum::ISO, static_cast<char>(0x00));

std::vector<PoClass> PoClass::valueList;

PoClass::StaticConstructor::StaticConstructor()
{
    valueList.push_back(LEGACY);
    valueList.push_back(ISO);
}

PoClass::StaticConstructor PoClass::staticConstructor;
int PoClass::nextOrdinal = 0;

char PoClass::getValue()
{
    return cla;
}

PoClass::PoClass(const std::string& name, InnerEnum innerEnum, char cla)
: innerEnumValue(innerEnum), nameValue(name), ordinalValue(nextOrdinal++),
  cla(cla)
{
}

PoClass::PoClass(const PoClass& o)
: innerEnumValue(o.innerEnumValue), nameValue(o.nameValue),
  ordinalValue(o.ordinalValue), cla(o.cla)
{
}

bool PoClass::operator==(const PoClass& o)
{
    return this->ordinalValue == o.ordinalValue;
}

bool PoClass::operator!=(const PoClass& o)
{
    return this->ordinalValue != o.ordinalValue;
}

PoClass& PoClass::operator=(const PoClass o)
{
    this->innerEnumValue = o.innerEnumValue;
    this->nameValue      = o.nameValue;
    this->ordinalValue   = o.ordinalValue;
    this->cla            = o.cla;

    return *this;
}

std::vector<PoClass> PoClass::values()
{
    return valueList;
}

int PoClass::ordinal()
{
    return ordinalValue;
}

std::string PoClass::toString()
{
    return nameValue;
}

PoClass PoClass::valueOf(const std::string& name)
{
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
