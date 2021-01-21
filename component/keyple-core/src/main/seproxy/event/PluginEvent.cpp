/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include <iostream>

#include "PluginEvent.h"

/* Common */
#include "IllegalArgumentException.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using namespace keyple::common::exception;

using EventType = PluginEvent::EventType;

/* EVENT TYPE --------------------------------------------------------------- */

const EventType EventType::READER_CONNECTED("Reader connected");
const EventType EventType::READER_DISCONNECTED("Reader disconnected");

EventType::EventType(const std::string& name) : mName(name) {}

const std::string& EventType::getName() const
{
    return mName;
}

bool EventType::operator==(const EventType& o) const
{
    return this->mName == o.mName;
}

bool EventType::operator!=(const EventType& o) const
{
    return !(*this == o);
}

const EventType& EventType::valueOf(const std::string& name)
{
    if (name == READER_CONNECTED.mName)
        return READER_CONNECTED;
    else if (name == READER_DISCONNECTED.mName)
        return READER_DISCONNECTED;
    else
        throw IllegalArgumentException("No event named " + name);
}

std::ostream& operator<<(std::ostream& os, const PluginEvent::EventType& et)
{
    os << "EVENTTYPE: {"
       << "NAME = " << et.mName
       << "}";

    return os;
}

/* PLUGIN EVENT ------------------------------------------------------------- */

PluginEvent::PluginEvent(const std::string& pluginName,
                         const std::string& readerName,
                         const EventType& eventType)
: mReaderName(readerName), mEventType(eventType), mPluginName(pluginName)
{
    mReaderNames.insert(readerName);
}

PluginEvent::PluginEvent(const std::string& pluginName,
                         std::shared_ptr<std::set<std::string>> readerNames,
                         const EventType& eventType)
: mEventType(eventType), mPluginName(pluginName)
{
    mReaderNames.insert(readerNames->begin(), readerNames->end());
}

const std::string& PluginEvent::getPluginName() const
{
    return mPluginName;
}

const std::set<std::string>& PluginEvent::getReaderNames() const
{
    return mReaderNames;
}

const EventType& PluginEvent::getEventType() const
{
    return mEventType;
}

std::ostream& operator<<(std::ostream& os, const PluginEvent& pe)
{
    os << "PLUGINEVENT: {"
       << "EVENTTYPE = " << pe.mEventType << ", "
       << "PLUGINNAME = " << pe.mPluginName << ", "
       << "READERNAME = " << pe.mReaderName << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<PluginEvent>& pe)
{
    if (pe == nullptr)
        os << "PLUGINEVENT = null";
    else
        os << *pe.get();

    return os;
}

}
}
}
}
