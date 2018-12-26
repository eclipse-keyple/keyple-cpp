#include "ObservableReader.h"
#include "ReaderEvent.h"
#include "../../transaction/SelectionRequest.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace event_Renamed {
                    using SeReader = org::eclipse::keyple::seproxy::SeReader;
                    using SelectionRequest = org::eclipse::keyple::transaction::SelectionRequest;
                    using Observable = org::eclipse::keyple::util::Observable;

NotificationMode NotificationMode::ALWAYS("ALWAYS", InnerEnum::ALWAYS, "always");
NotificationMode NotificationMode::MATCHED_ONLY("MATCHED_ONLY", InnerEnum::MATCHED_ONLY, "matched_only");

std::vector<NotificationMode> NotificationMode::valueList;
int NotificationMode::nextOrdinal = 0;

                    NotificationMode::NotificationMode(const std::string &name, InnerEnum innerEnum, const std::string &name) : nameValue(name), ordinalValue(nextOrdinal++), innerEnumValue(innerEnum) {
                        this->name = name;
                    }

                    std::string NotificationMode::getName() {
                        return this->name;
                    }

const std::unordered_map<std::string, NotificationMode> ObservableReader::NotificationMode::lookup = std::unordered_map<std::string, NotificationMode>();
NotificationMode::StaticConstructor ObservableReader::NotificationMode::staticConstructor;

                    NotificationMode NotificationMode::get(const std::string &name) {
                        return lookup->get(name);
                    }

bool NotificationMode::operator == (const NotificationMode &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool NotificationMode::operator != (const NotificationMode &other) {
    return this->ordinalValue != other.ordinalValue;
}

std::vector<NotificationMode> NotificationMode::values() {
    return valueList;
}

int NotificationMode::ordinal() {
    return ordinalValue;
}

std::string NotificationMode::toString() {
    return nameValue;
}

NotificationMode NotificationMode::valueOf(const std::string &name) {
    for (auto enumInstance : NotificationMode::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }
}
                }
            }
        }
    }
}
