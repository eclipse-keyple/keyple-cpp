#include "ObservableReader.h"
#include "ReaderEvent_Import.h"
#include "AbstractDefaultSelectionsRequest.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using SeReader         = org::eclipse::keyple::core::seproxy::SeReader;
using NotificationMode = org::eclipse::keyple::core::seproxy::event::ObservableReader::NotificationMode;

NotificationMode NotificationMode::ALWAYS("ALWAYS", InnerEnum::ALWAYS, "always");
NotificationMode NotificationMode::MATCHED_ONLY("MATCHED_ONLY", InnerEnum::MATCHED_ONLY, "matched_only");

std::vector<NotificationMode> NotificationMode::valueList;
int NotificationMode::nextOrdinal = 0;

NotificationMode::NotificationMode(const std::string &nameValue,
                                   InnerEnum innerEnum,
                                   const std::string &name)
: innerEnumValue(innerEnum), nameValue(nameValue), ordinalValue(nextOrdinal++)
{
    this->name = name;
}

std::string NotificationMode::getName()
{
    return this->name;
}

const std::unordered_map<std::string, NotificationMode> ObservableReader::NotificationMode::lookup = std::unordered_map<std::string, NotificationMode>();
NotificationMode::StaticConstructor ObservableReader::NotificationMode::staticConstructor;

NotificationMode NotificationMode::get(const std::string &name)
{
    std::unordered_map<std::string, NotificationMode>::const_iterator it =
                                                             lookup.find(name);
    return it->second;
}

bool NotificationMode::operator == (const NotificationMode &other)
{
    return this->ordinalValue == other.ordinalValue;
}

bool NotificationMode::operator != (const NotificationMode &other)
{
    return this->ordinalValue != other.ordinalValue;
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

NotificationMode NotificationMode::valueOf(const std::string &name)
{
    for (auto enumInstance : NotificationMode::valueList) {
        if (enumInstance.nameValue == name) {
            return enumInstance;
        }
    }

    return NotificationMode::ALWAYS;
}

}
}
}
}
}
}
