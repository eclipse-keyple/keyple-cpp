#include <iostream>

#include "PluginEvent.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace event {

                    using EventType = org::eclipse::keyple::seproxy::event::PluginEvent::EventType;

                    const EventType EventType::READER_CONNECTED("READER_CONNECTED", InnerEnum::READER_CONNECTED, "Reader connected");
                    const EventType EventType::READER_DISCONNECTED("READER_DISCONNECTED", InnerEnum::READER_DISCONNECTED, "Reader disconnected");

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

                    EventType EventType::valueOf(const std::string &name) {
                        for (auto enumInstance : EventType::valueList) {
                            if (enumInstance.nameValue == name) {
                                return enumInstance;
                            }
                        }

                        return EventType::READER_DISCONNECTED;
                    }

                    PluginEvent::PluginEvent(const std::string &pluginName, const std::string &readerName, EventType eventType) : pluginName(pluginName), readerName(readerName), eventType(eventType) {
			this->readNames.insert(readerName);
                    }

		PluginEvent::PluginEvent(const std::string &pluginName, std::shared_ptr<std::set<std::string>> readerNames, EventType eventType) : pluginName(pluginName), eventType(eventType) {
                        this->readerNames->addAll(readerNames);
                    }

                    std::string PluginEvent::getPluginName() {
                        return pluginName;
                    }

                    std::shared_ptr<std::set<std::string>> PluginEvent::getReaderNames() {
                        return std::make_shared<std::set<std::string>>(readerNames);
                    }

                    PluginEvent::EventType PluginEvent::getEventType() {
                        return eventType;
                    }
                }
            }
        }
    }
}
