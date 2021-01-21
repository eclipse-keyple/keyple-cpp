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

#include "ReaderEvent.h"

/* Core */
#include "AbstractDefaultSelectionsResponse.h"
#include "DefaultSelectionsResponse.h"
#include "SeProxyService.h"

/* Common */
#include "IllegalArgumentException.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using namespace keyple::common::exception;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;

using EventType = ReaderEvent::EventType;

/* EVENT TYPE ----------------------------------------------------------------------------------- */

const EventType EventType::TIMEOUT_ERROR("SE Reader timeout Error");
const EventType EventType::SE_INSERTED("SE insertion");
const EventType EventType::SE_MATCHED("SE matched");
const EventType EventType::SE_REMOVED("SE removed");

EventType::EventType(const std::string& name) : mName(name) {}

const std::string& EventType::getName() const
{
    return mName;
}

bool EventType::operator==(const EventType& other) const
{
    return mName == other.mName;
}

bool EventType::operator!=(const EventType& other) const
{
    return !(*this == other);
}

const EventType& EventType::valueOf(const std::string& name)
{
    if (name == TIMEOUT_ERROR.mName)
        return TIMEOUT_ERROR;
    else if (name == SE_INSERTED.mName)
        return SE_INSERTED;
    else if (name == SE_MATCHED.mName)
        return SE_MATCHED;
    else if (name == SE_REMOVED.mName)
        return SE_REMOVED;
    else
        throw IllegalArgumentException("No event named " + name);
}

std::ostream& operator<<(std::ostream& os, const EventType& et)
{
    os << "EVENTTYPE: {"
       << "NAME = " << et.mName
       << "}";

    return os;
}

/* READER EVENT --------------------------------------------------------------------------------- */

ReaderEvent::ReaderEvent(
  const std::string& pluginName,
  const std::string& readerName,
  const EventType& eventType,
  std::shared_ptr<AbstractDefaultSelectionsResponse> defaultSelectionsResponse)
: mEventType(eventType),
  mPluginName(pluginName),
  mReaderName(readerName),
  mDefaultResponses(std::static_pointer_cast<DefaultSelectionsResponse>(
      defaultSelectionsResponse))
{
}

const std::string& ReaderEvent::getPluginName() const
{
    return mPluginName;
}

const std::string& ReaderEvent::getReaderName() const
{
    return mReaderName;
}

const ReaderEvent::EventType& ReaderEvent::getEventType() const
{
    return mEventType;
}

const std::shared_ptr<ReaderPlugin> ReaderEvent::getPlugin() const
{
    return SeProxyService::getInstance().getPlugin(mPluginName);
}

const std::shared_ptr<SeReader> ReaderEvent::getReader() const
{
    return getPlugin()->getReader(mReaderName);
}

std::ostream& operator<<(std::ostream& os, const ReaderEvent& re)
{
    os << "READEREVENT: {"
        << "EVENTTYPE = " << re.mEventType << ", "
        << "PLUGINNAME = " << re.mPluginName << ", "
        << "READERNAME = " << re.mReaderName
        << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<ReaderEvent>& re)
{
    if (re == nullptr)
        os << "READEREVENT = null";
    else
        os << *re.get();

    return os;
}

const std::shared_ptr<AbstractDefaultSelectionsResponse> ReaderEvent::getDefaultSelectionsResponse()
    const
{
    return mDefaultResponses;
}

}
}
}
}
