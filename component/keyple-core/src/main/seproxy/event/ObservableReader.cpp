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

#include "ObservableReader.h"
#include "ReaderEvent_Import.h"
#include "AbstractDefaultSelectionsRequest.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;

using NotificationMode = ObservableReader::NotificationMode;

NotificationMode NotificationMode::ALWAYS("ALWAYS", InnerEnum::ALWAYS,
                                          "always");
NotificationMode NotificationMode::MATCHED_ONLY("MATCHED_ONLY",
                                                InnerEnum::MATCHED_ONLY,
                                                "matched_only");

std::vector<NotificationMode> NotificationMode::valueList;
int NotificationMode::nextOrdinal = 0;

NotificationMode::NotificationMode(const std::string& nameValue,
                                   InnerEnum innerEnum, const std::string& name)
: innerEnumValue(innerEnum), name(name), nameValue(nameValue),
  ordinalValue(nextOrdinal++)
{
}

NotificationMode::NotificationMode(const NotificationMode& o)
: innerEnumValue(o.innerEnumValue), name(o.name), nameValue(o.nameValue),
  ordinalValue(o.ordinalValue)
{
}

std::string NotificationMode::getName()
{
    return this->name;
}

const std::map<std::string, NotificationMode> NotificationMode::lookup =
    std::map<std::string, NotificationMode>();

NotificationMode::StaticConstructor NotificationMode::staticConstructor;

NotificationMode NotificationMode::get(const std::string& name)
{
    std::map<std::string, NotificationMode>::const_iterator it =
        lookup.find(name);

    return it->second;
}

bool NotificationMode::operator==(const NotificationMode& other)
{
    return this->ordinalValue == other.ordinalValue;
}

bool NotificationMode::operator!=(const NotificationMode& other)
{
    return this->ordinalValue != other.ordinalValue;
}

NotificationMode& NotificationMode::operator=(NotificationMode o)
{
    this->innerEnumValue = o.innerEnumValue;
    this->name           = o.name;
    this->nameValue      = o.nameValue;
    this->ordinalValue   = o.ordinalValue;

    return *this;
}

std::vector<NotificationMode> NotificationMode::values()
{
    return valueList;
}

int NotificationMode::ordinal()
{
    return ordinalValue;
}

std::string NotificationMode::toString()
{
    return nameValue;
}

NotificationMode NotificationMode::valueOf(const std::string& name)
{
    for (auto enumInstance : NotificationMode::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    return NotificationMode::ALWAYS;
}

std::ostream& operator<<(std::ostream& os,
                         const ObservableReader::PollingMode& pm)
{
    switch (pm) {
    case ObservableReader::PollingMode::REPEATING:
        os << "REPEATING";
        break;
    case ObservableReader::PollingMode::SINGLESHOT:
        os << "SINGLESHOT";
        break;
    }

    return os;
}

}
}
}
}
