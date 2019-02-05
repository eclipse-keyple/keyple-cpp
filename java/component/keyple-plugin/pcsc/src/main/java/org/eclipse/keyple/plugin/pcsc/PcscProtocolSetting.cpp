
#include "ContactlessProtocols_Import.h"
#include "PcscProtocolSetting.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace pcsc {
                    using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                    using SeProtocolSettingList = org::eclipse::keyple::seproxy::protocol::SeProtocolSettingList;

                    PcscProtocolSetting PcscProtocolSetting::SETTING_PROTOCOL_ISO14443_4("SETTING_PROTOCOL_ISO14443_4", InnerEnum::SETTING_PROTOCOL_ISO14443_4, ContactlessProtocols::PROTOCOL_ISO14443_4, ProtocolSetting::REGEX_PROTOCOL_ISO14443_4);
                    PcscProtocolSetting PcscProtocolSetting::SETTING_PROTOCOL_B_PRIME("SETTING_PROTOCOL_B_PRIME", InnerEnum::SETTING_PROTOCOL_B_PRIME, ContactlessProtocols::PROTOCOL_B_PRIME, ProtocolSetting::REGEX_PROTOCOL_B_PRIME);
                    PcscProtocolSetting PcscProtocolSetting::SETTING_PROTOCOL_MIFARE_UL("SETTING_PROTOCOL_MIFARE_UL", InnerEnum::SETTING_PROTOCOL_MIFARE_UL, ContactlessProtocols::PROTOCOL_MIFARE_UL, ProtocolSetting::REGEX_PROTOCOL_MIFARE_UL);
                    PcscProtocolSetting PcscProtocolSetting::SETTING_PROTOCOL_MIFARE_CLASSIC("SETTING_PROTOCOL_MIFARE_CLASSIC", InnerEnum::SETTING_PROTOCOL_MIFARE_CLASSIC, ContactlessProtocols::PROTOCOL_MIFARE_CLASSIC, ProtocolSetting::REGEX_PROTOCOL_MIFARE_CLASSIC);
                    PcscProtocolSetting PcscProtocolSetting::SETTING_PROTOCOL_MIFARE_DESFIRE("SETTING_PROTOCOL_MIFARE_DESFIRE", InnerEnum::SETTING_PROTOCOL_MIFARE_DESFIRE, ContactlessProtocols::PROTOCOL_MIFARE_DESFIRE, ProtocolSetting::REGEX_PROTOCOL_MIFARE_DESFIRE);
                    PcscProtocolSetting PcscProtocolSetting::SETTING_PROTOCOL_MEMORY_ST25("SETTING_PROTOCOL_MEMORY_ST25", InnerEnum::SETTING_PROTOCOL_MEMORY_ST25, ContactlessProtocols::PROTOCOL_MEMORY_ST25, ProtocolSetting::REGEX_PROTOCOL_MEMORY_ST25);

                    const std::string PcscProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_ISO14443_4 = "3B8880010000000000718100F9|3B8C800150........00000000007181..";
                    const std::string PcscProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_B_PRIME = "3B8F8001805A0A0103200311........829000..";
                    const std::string PcscProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_MIFARE_UL = "3B8F8001804F0CA0000003060300030000000068";
                    const std::string PcscProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_MIFARE_CLASSIC = "3B8F8001804F0CA000000306030001000000006A";
                    const std::string PcscProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_MIFARE_DESFIRE = "3B8180018080";
                    const std::string PcscProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_MEMORY_ST25 = "3B8F8001804F0CA000000306070007D0020C00B6";

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

                    PcscProtocolSetting::PcscProtocolSetting(const std::string &name, InnerEnum innerEnum, ContactlessProtocols &flag, const std::string &value) : nameValue(name), ordinalValue(nextOrdinal++), innerEnumValue(innerEnum), flag(flag) {
                        this->value = value;
                    }

                    std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> PcscProtocolSetting::getFlag() {
                        return std::dynamic_pointer_cast<SeProtocol>(std::make_shared<ContactlessProtocols>(this->flag) );
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

                        /* Warning fix, should not go there */
                        return PcscProtocolSetting::valueList.front();
                    }
                }
            }
        }
    }
}
