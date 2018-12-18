#include "PluginEvent.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace event_Renamed {

EventType EventType::READER_CONNECTED("READER_CONNECTED", InnerEnum::READER_CONNECTED, "Reader connected");
EventType EventType::READER_DISCONNECTED("READER_DISCONNECTED", InnerEnum::READER_DISCONNECTED, "Reader disconnected");

std::vector<EventType> EventType::valueList;

EventType::StaticConstructor::StaticConstructor() {
    valueList.push_back(READER_CONNECTED);
    valueList.push_back(READER_DISCONNECTED);
}

EventType::StaticConstructor EventType::staticConstructor;
int EventType::nextOrdinal = 0;

                    PluginEvent::EventType::EventType(const std::string &name, InnerEnum innerEnum, std::shared_ptr<PluginEvent> outerInstance, const std::string &name) : nameValue(name), ordinalValue(nextOrdinal++), innerEnumValue(innerEnum) {
                                            this->outerInstance = outerInstance;
                        this->name = name;
                    }

                    std::string PluginEvent::EventType::getName() {
                        return outerInstance->name;
                    }

bool EventType::operator == (const EventType &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool EventType::operator != (const EventType &other) {
    return this->ordinalValue != other.ordinalValue;
}

std::vector<EventType> EventType::values() {
    return valueList;
}

int EventType::ordinal() {
    return ordinalValue;
}

std::string EventType::toString() {
    return nameValue;
}

EventType EventType::valueOf(const std::string &name) {
    for (auto enumInstance : EventType::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }
}

                    PluginEvent::PluginEvent(const std::string &pluginName, const std::string &readerName, EventType eventType) : pluginName(pluginName), readerName(readerName), eventType(eventType) {
                    }

                    std::string PluginEvent::getPluginName() {
                        return pluginName;
                    }

                    std::string PluginEvent::getReaderName() {
                        return readerName;
                    }

                    PluginEvent::EventType PluginEvent::getEventType() {
                        return eventType;
                    }
                }
            }
        }
    }
}
