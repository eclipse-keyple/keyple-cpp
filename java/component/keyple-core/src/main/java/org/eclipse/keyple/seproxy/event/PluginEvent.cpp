#include "PluginEvent.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace event {

                    using EventType = org::eclipse::keyple::seproxy::event::PluginEvent::EventType;

                    EventType EventType::READER_CONNECTED("READER_CONNECTED", InnerEnum::READER_CONNECTED, "Reader connected");
                    EventType EventType::READER_DISCONNECTED("READER_DISCONNECTED", InnerEnum::READER_DISCONNECTED, "Reader disconnected");

                    std::vector<EventType> EventType::valueList;

                    EventType::StaticConstructor::StaticConstructor() {
                        valueList.push_back(READER_CONNECTED);
                        valueList.push_back(READER_DISCONNECTED);
                    }

                    EventType::StaticConstructor EventType::staticConstructor;
                    int EventType::nextOrdinal = 0;

                    PluginEvent::EventType::EventType(const std::string &nameValue, InnerEnum innerEnum, const std::string &name) : nameValue(nameValue), ordinalValue(nextOrdinal++), innerEnumValue(innerEnum) {
                        this->name = name;
                    }

                    std::string PluginEvent::EventType::getName() {
                        return this->name;
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

                        return EventType::READER_DISCONNECTED;
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
