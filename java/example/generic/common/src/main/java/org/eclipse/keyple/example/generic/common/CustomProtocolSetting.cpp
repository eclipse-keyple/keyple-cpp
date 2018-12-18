#include "CustomProtocolSetting.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic_Renamed {
                    namespace common {
                        using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                        using SeProtocolSettingList = org::eclipse::keyple::seproxy::protocol::SeProtocolSettingList;

CustomProtocolSetting CustomProtocolSetting::CUSTOM_SETTING_PROTOCOL_B_PRIME("CUSTOM_SETTING_PROTOCOL_B_PRIME", InnerEnum::CUSTOM_SETTING_PROTOCOL_B_PRIME, CustomProtocols::CUSTOM_PROTOCOL_B_PRIME, "3B8F8001805A0A0103200311........829000..");
CustomProtocolSetting CustomProtocolSetting::CUSTOM_SETTING_PROTOCOL_ISO14443_4("CUSTOM_SETTING_PROTOCOL_ISO14443_4", InnerEnum::CUSTOM_SETTING_PROTOCOL_ISO14443_4, CustomProtocols::CUSTOM_PROTOCOL_MIFARE_DESFIRE, "3B8180018080");

std::vector<CustomProtocolSetting> CustomProtocolSetting::valueList;

CustomProtocolSetting::StaticConstructor::StaticConstructor() {
    valueList.push_back(CUSTOM_SETTING_PROTOCOL_B_PRIME);
    valueList.push_back(CUSTOM_SETTING_PROTOCOL_ISO14443_4);
}

CustomProtocolSetting::StaticConstructor CustomProtocolSetting::staticConstructor;
int CustomProtocolSetting::nextOrdinal = 0;

                        CustomProtocolSetting::CustomProtocolSetting(const std::string &name, InnerEnum innerEnum, std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> flag, const std::string &value) : nameValue(name), ordinalValue(nextOrdinal++), innerEnumValue(innerEnum) {
                            this->flag = flag;
                            this->value = value;
                        }

                        std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> CustomProtocolSetting::getFlag() {
                            return flag;
                        }

                        std::string CustomProtocolSetting::getValue() {
                            return value;
                        }

bool CustomProtocolSetting::operator == (const CustomProtocolSetting &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool CustomProtocolSetting::operator != (const CustomProtocolSetting &other) {
    return this->ordinalValue != other.ordinalValue;
}

std::vector<CustomProtocolSetting> CustomProtocolSetting::values() {
    return valueList;
}

int CustomProtocolSetting::ordinal() {
    return ordinalValue;
}

std::string CustomProtocolSetting::toString() {
    return nameValue;
}

CustomProtocolSetting CustomProtocolSetting::valueOf(const std::string &name) {
    for (auto enumInstance : CustomProtocolSetting::valueList) {
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
}
