#include "PcscProtocolSetting.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace pcsc {
                    using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                    using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                    using SeProtocolSettingList = org::eclipse::keyple::seproxy::protocol::SeProtocolSettingList;

PcscProtocolSetting PcscProtocolSetting::SETTING_PROTOCOL_ISO14443_4("SETTING_PROTOCOL_ISO14443_4", InnerEnum::SETTING_PROTOCOL_ISO14443_4, org::eclipse::keyple::seproxy::protocol::ContactlessProtocols::PROTOCOL_ISO14443_4, ProtocolSetting::REGEX_PROTOCOL_ISO14443_4);
PcscProtocolSetting PcscProtocolSetting::SETTING_PROTOCOL_B_PRIME("SETTING_PROTOCOL_B_PRIME", InnerEnum::SETTING_PROTOCOL_B_PRIME, org::eclipse::keyple::seproxy::protocol::ContactlessProtocols::PROTOCOL_B_PRIME, ProtocolSetting::REGEX_PROTOCOL_B_PRIME);
PcscProtocolSetting PcscProtocolSetting::SETTING_PROTOCOL_MIFARE_UL("SETTING_PROTOCOL_MIFARE_UL", InnerEnum::SETTING_PROTOCOL_MIFARE_UL, org::eclipse::keyple::seproxy::protocol::ContactlessProtocols::PROTOCOL_MIFARE_UL, ProtocolSetting::REGEX_PROTOCOL_MIFARE_UL);
PcscProtocolSetting PcscProtocolSetting::SETTING_PROTOCOL_MIFARE_CLASSIC("SETTING_PROTOCOL_MIFARE_CLASSIC", InnerEnum::SETTING_PROTOCOL_MIFARE_CLASSIC, org::eclipse::keyple::seproxy::protocol::ContactlessProtocols::PROTOCOL_MIFARE_CLASSIC, ProtocolSetting::REGEX_PROTOCOL_MIFARE_CLASSIC);
PcscProtocolSetting PcscProtocolSetting::SETTING_PROTOCOL_MIFARE_DESFIRE("SETTING_PROTOCOL_MIFARE_DESFIRE", InnerEnum::SETTING_PROTOCOL_MIFARE_DESFIRE, org::eclipse::keyple::seproxy::protocol::ContactlessProtocols::PROTOCOL_MIFARE_DESFIRE, ProtocolSetting::REGEX_PROTOCOL_MIFARE_DESFIRE);
PcscProtocolSetting PcscProtocolSetting::SETTING_PROTOCOL_MEMORY_ST25("SETTING_PROTOCOL_MEMORY_ST25", InnerEnum::SETTING_PROTOCOL_MEMORY_ST25, org::eclipse::keyple::seproxy::protocol::ContactlessProtocols::PROTOCOL_MEMORY_ST25, ProtocolSetting::REGEX_PROTOCOL_MEMORY_ST25);

std::vector<PcscProtocolSetting> PcscProtocolSetting::valueList;

PcscProtocolSetting::StaticConstructor::StaticConstructor() {
    valueList.push_back(SETTING_PROTOCOL_ISO14443_4);
    valueList.push_back(SETTING_PROTOCOL_B_PRIME);
    valueList.push_back(SETTING_PROTOCOL_MIFARE_UL);
    valueList.push_back(SETTING_PROTOCOL_MIFARE_CLASSIC);
    valueList.push_back(SETTING_PROTOCOL_MIFARE_DESFIRE);
    valueList.push_back(SETTING_PROTOCOL_MEMORY_ST25);
}

PcscProtocolSetting::StaticConstructor PcscProtocolSetting::staticConstructor;
int PcscProtocolSetting::nextOrdinal = 0;

                    PcscProtocolSetting::PcscProtocolSetting(const std::string &name, InnerEnum innerEnum, std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> flag, const std::string &value) : nameValue(name), ordinalValue(nextOrdinal++), innerEnumValue(innerEnum) {
                        this->flag = flag;
                        this->value = value;
                    }

                    std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> PcscProtocolSetting::getFlag() {
                        return this->flag;
                    }

                    std::string PcscProtocolSetting::getValue() {
                        return this->value;
                    }

bool PcscProtocolSetting::operator == (const PcscProtocolSetting &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool PcscProtocolSetting::operator != (const PcscProtocolSetting &other) {
    return this->ordinalValue != other.ordinalValue;
}

std::vector<PcscProtocolSetting> PcscProtocolSetting::values() {
    return valueList;
}

int PcscProtocolSetting::ordinal() {
    return ordinalValue;
}

std::string PcscProtocolSetting::toString() {
    return nameValue;
}

PcscProtocolSetting PcscProtocolSetting::valueOf(const std::string &name) {
    for (auto enumInstance : PcscProtocolSetting::valueList) {
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
