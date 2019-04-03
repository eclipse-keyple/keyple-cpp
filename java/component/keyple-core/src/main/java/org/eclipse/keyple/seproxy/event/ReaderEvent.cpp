#include "ReaderEvent.h"
#include "SelectionResponse.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace event {
                    using org::eclipse::keyple::seproxy::event::SelectionResponse;
                    using EventType = org::eclipse::keyple::seproxy::event::ReaderEvent::EventType;

                    EventType EventType::IO_ERROR("IO_ERROR", InnerEnum::IO_ERROR, "SE Reader IO Error");
                    EventType EventType::SE_INSERTED("SE_INSERTED", InnerEnum::SE_INSERTED, "SE insertion");
                    EventType EventType::SE_MATCHED("SE_MATCHED", InnerEnum::SE_MATCHED, "SE matched");
                    EventType EventType::SE_REMOVAL("SE_REMOVAL", InnerEnum::SE_REMOVAL, "SE removal");

                    std::vector<EventType> EventType::valueList;

                    EventType::StaticConstructor::StaticConstructor() {
                        valueList.push_back(IO_ERROR);
                        valueList.push_back(SE_INSERTED);
                        valueList.push_back(SE_MATCHED);
                        valueList.push_back(SE_REMOVAL);
                    }

                    EventType::StaticConstructor EventType::staticConstructor;
                    int EventType::nextOrdinal = 0;

                    ReaderEvent::EventType::EventType(const std::string &nameValue, InnerEnum innerEnum, const std::string &name) : innerEnumValue(innerEnum), nameValue(nameValue), ordinalValue(nextOrdinal++){

                        this->name = name;
                    }

                    std::string ReaderEvent::EventType::getName() {
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

                        /* Should not end up here */
                        return EventType::IO_ERROR;
                    }

                    ReaderEvent::ReaderEvent(const std::string &pluginName, const std::string &readerName, EventType eventType, std::shared_ptr<SelectionResponse> selectionResponse)
                    : pluginName(pluginName), readerName(readerName), eventType(eventType), defaultResponseSet(selectionResponse) {
                    }

                    std::string ReaderEvent::getPluginName() {
                        return pluginName;
                    }

                    std::string ReaderEvent::getReaderName() {
                        return readerName;
                    }

                    ReaderEvent::EventType ReaderEvent::getEventType() {
                        return eventType;
                    }

                    std::shared_ptr<SelectionResponse> ReaderEvent::getDefaultSelectionResponse() {
                        return defaultResponseSet;
                    }
                }
            }
        }
    }
}
