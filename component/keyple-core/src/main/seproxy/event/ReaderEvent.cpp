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

#include "ReaderEvent.h"
#include "DefaultSelectionsResponse.h"
#include "AbstractDefaultSelectionsResponse.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::event;

using EventType = ReaderEvent::EventType;

EventType EventType::TIMEOUT_ERROR("TIMEOUT_ERROR", InnerEnum::TIMEOUT_ERROR,
                                   "SE Reader timeout Error");
EventType EventType::SE_INSERTED("SE_INSERTED", InnerEnum::SE_INSERTED,
                                 "SE insertion");
EventType EventType::SE_MATCHED("SE_MATCHED", InnerEnum::SE_MATCHED,
                                "SE matched");
EventType EventType::SE_REMOVED("SE_REMOVED", InnerEnum::SE_REMOVAL,
                                "SE removed");

std::vector<EventType> EventType::valueList;

EventType::StaticConstructor::StaticConstructor()
{
    valueList.push_back(TIMEOUT_ERROR);
    valueList.push_back(SE_INSERTED);
    valueList.push_back(SE_MATCHED);
    valueList.push_back(SE_REMOVED);
}

EventType::StaticConstructor EventType::staticConstructor;
int EventType::nextOrdinal = 0;

EventType::EventType(const std::string &nameValue, InnerEnum innerEnum,
                     const std::string &name)
: innerEnumValue(innerEnum), nameValue(nameValue), ordinalValue(nextOrdinal++),
  name(name)
{
}

EventType::EventType(const EventType& o)
: innerEnumValue(o.innerEnumValue), nameValue(o.nameValue),
  ordinalValue(o.ordinalValue), name(o.name)
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

std::string EventType::toString()
{
    return nameValue;
}

EventType EventType::valueOf(const std::string &name)
{
    for (auto enumInstance : EventType::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    /* Should not end up here */
    return EventType::TIMEOUT_ERROR;
}

ReaderEvent::ReaderEvent(const std::string &pluginName,
  const std::string &readerName, EventType eventType,
  std::shared_ptr<AbstractDefaultSelectionsResponse> defaultSelectionsResponse)
: eventType(eventType), pluginName(pluginName), readerName(readerName),
  defaultResponseSet(std::static_pointer_cast<DefaultSelectionsResponse>(
      defaultSelectionsResponse))
{
}

std::string ReaderEvent::getPluginName()
{
    return pluginName;
}

std::string ReaderEvent::getReaderName()
{
    return readerName;
}

ReaderEvent::EventType ReaderEvent::getEventType()
{
    return eventType;
}

std::shared_ptr<AbstractDefaultSelectionsResponse>
ReaderEvent::getDefaultSelectionsResponse()
{
    return defaultResponseSet;
}

}
}
}
}
