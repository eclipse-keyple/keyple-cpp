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

#include <iostream>

#include "PluginEvent.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using namespace keyple::core::seproxy::event;

using EventType = PluginEvent::EventType;

const EventType EventType::READER_CONNECTED("READER_CONNECTED",
                                            InnerEnum::READER_CONNECTED,
                                            "Reader connected");
const EventType EventType::READER_DISCONNECTED("READER_DISCONNECTED",
                                               InnerEnum::READER_DISCONNECTED,
                                               "Reader disconnected");

std::vector<EventType> EventType::valueList;

EventType::StaticConstructor::StaticConstructor()
{
    valueList.push_back(READER_CONNECTED);
    valueList.push_back(READER_DISCONNECTED);
}

EventType::StaticConstructor EventType::staticConstructor;
int EventType::nextOrdinal = 0;

EventType::EventType(const std::string &nameValue, InnerEnum innerEnum,
                     const std::string &name)
: innerEnumValue(innerEnum), nameValue(nameValue), ordinalValue(nextOrdinal++)
{
    this->name = name;
}

EventType::EventType(const EventType& o)
: innerEnumValue(o.innerEnumValue), nameValue(o.nameValue),
  ordinalValue(o.ordinalValue)
{
}

std::string EventType::getName()
{
    return this->name;
}

bool EventType::operator == (const EventType &other)
{
    return this->ordinalValue == other.ordinalValue;
}

bool EventType::operator != (const EventType &other)
{
    return this->ordinalValue != other.ordinalValue;
}

std::vector<EventType> EventType::values()
{
    return valueList;
}

int EventType::ordinal()
{
    return ordinalValue;
}

EventType EventType::valueOf(const std::string &name)
{
    for (auto enumInstance : EventType::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    return EventType::READER_DISCONNECTED;
}

PluginEvent::PluginEvent(const std::string &pluginName,
                         const std::string &readerName, EventType eventType)
: readerName(readerName), eventType(eventType), pluginName(pluginName)
{
    this->readerNames->insert(readerName);
}

PluginEvent::PluginEvent(const std::string &pluginName,
                         std::shared_ptr<std::set<std::string>> readerNames,
                         EventType eventType)
: eventType(eventType), pluginName(pluginName)
{
    this->readerNames->insert(readerNames->begin(), readerNames->end());
}

std::string PluginEvent::getPluginName()
{
    return pluginName;
}

std::shared_ptr<std::set<std::string>> PluginEvent::getReaderNames()
{
    return readerNames;
}

PluginEvent::EventType PluginEvent::getEventType()
{
    return eventType;
}

}
}
}
}
