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

#include "CustomProtocols.h"

namespace keyple {
namespace example {
namespace generic {
namespace common {

using namespace keyple::core::seproxy::protocol;

CustomProtocols CustomProtocols::CUSTOM_PROTOCOL_B_PRIME(
    "CUSTOM_PROTOCOL_B_PRIME",
    InnerEnum::CUSTOM_PROTOCOL_B_PRIME,
    "Custom Old Calypso B prime",
    TransmissionMode::CONTACTLESS);
CustomProtocols CustomProtocols::CUSTOM_PROTOCOL_MIFARE_DESFIRE(
    "CUSTOM_PROTOCOL_MIFARE_DESFIRE",
    InnerEnum::CUSTOM_PROTOCOL_MIFARE_DESFIRE,
    "Custom Mifare DESFire",
    TransmissionMode::CONTACTLESS);

std::vector<CustomProtocols> CustomProtocols::valueList;
CustomProtocols::StaticConstructor CustomProtocols::staticConstructor;
int CustomProtocols::nextOrdinal = 0;

CustomProtocols::StaticConstructor::StaticConstructor()
{
    valueList.push_back(CUSTOM_PROTOCOL_B_PRIME);
    valueList.push_back(CUSTOM_PROTOCOL_MIFARE_DESFIRE);
}

CustomProtocols::CustomProtocols(
  const std::string& nameValue, InnerEnum innerEnum, const std::string& name,
  TransmissionMode transmissionMode)
: /*innerEnumValue(innerEnum), */nameValue(nameValue),
  ordinalValue(nextOrdinal++), name(name), transmissionMode(transmissionMode)
{
    /* innerEnumValue is not used, commented it out in class def. */
    (void)innerEnum;
}

std::string CustomProtocols::getName() const
{
    return name;
}

bool CustomProtocols::operator == (const CustomProtocols &other)
{
    return this->ordinalValue == other.ordinalValue;
}

bool CustomProtocols::operator != (const CustomProtocols &other)
{
    return this->ordinalValue != other.ordinalValue;
}

std::vector<CustomProtocols> CustomProtocols::values()
{
    return valueList;
}

int CustomProtocols::ordinal()
{
    return ordinalValue;
}

std::string CustomProtocols::toString()
{
    return nameValue;
}

CustomProtocols CustomProtocols::valueOf(const std::string &name)
{
    for (auto enumInstance : CustomProtocols::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    /* Make compiler happy */
    return valueList.front();
}

TransmissionMode CustomProtocols::getTransmissionMode() const
{
    return transmissionMode;
}

}
}
}
}
