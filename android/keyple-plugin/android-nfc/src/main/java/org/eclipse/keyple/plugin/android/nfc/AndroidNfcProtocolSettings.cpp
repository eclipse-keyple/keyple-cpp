#include "AndroidNfcProtocolSettings.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace android {
                    namespace nfc {
                        using ContactlessProtocols = org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                        using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                        using SeProtocolSettingList = org::eclipse::keyple::seproxy::protocol::SeProtocolSettingList;

AndroidNfcProtocolSettings AndroidNfcProtocolSettings::SETTING_PROTOCOL_ISO14443_4("SETTING_PROTOCOL_ISO14443_4", InnerEnum::SETTING_PROTOCOL_ISO14443_4, org::eclipse::keyple::seproxy::protocol::ContactlessProtocols::PROTOCOL_ISO14443_4, ProtocolSetting::NFC_TAG_TYPE_ISODEP);
AndroidNfcProtocolSettings AndroidNfcProtocolSettings::SETTING_PROTOCOL_MIFARE_UL("SETTING_PROTOCOL_MIFARE_UL", InnerEnum::SETTING_PROTOCOL_MIFARE_UL, org::eclipse::keyple::seproxy::protocol::ContactlessProtocols::PROTOCOL_MIFARE_UL, ProtocolSetting::NFC_TAG_TYPE_MIFARE_UL);
AndroidNfcProtocolSettings AndroidNfcProtocolSettings::SETTING_PROTOCOL_MIFARE_CLASSIC("SETTING_PROTOCOL_MIFARE_CLASSIC", InnerEnum::SETTING_PROTOCOL_MIFARE_CLASSIC, org::eclipse::keyple::seproxy::protocol::ContactlessProtocols::PROTOCOL_MIFARE_CLASSIC, ProtocolSetting::NFC_TAG_TYPE_MIFARE_CLASSIC);

std::vector<AndroidNfcProtocolSettings> AndroidNfcProtocolSettings::valueList;

AndroidNfcProtocolSettings::StaticConstructor::StaticConstructor() {
    valueList.push_back(SETTING_PROTOCOL_ISO14443_4);
    valueList.push_back(SETTING_PROTOCOL_MIFARE_UL);
    valueList.push_back(SETTING_PROTOCOL_MIFARE_CLASSIC);
}

AndroidNfcProtocolSettings::StaticConstructor AndroidNfcProtocolSettings::staticConstructor;
int AndroidNfcProtocolSettings::nextOrdinal = 0;

                        AndroidNfcProtocolSettings::AndroidNfcProtocolSettings(const std::string &name, InnerEnum innerEnum, std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> flag, const std::string &value) : nameValue(name), ordinalValue(nextOrdinal++), innerEnumValue(innerEnum) {
                            this->flag = flag;
                            this->value = value;
                        }

                        std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> AndroidNfcProtocolSettings::getFlag() {
                            return flag;
                        }

                        std::string AndroidNfcProtocolSettings::getValue() {
                            return value;
                        }

bool AndroidNfcProtocolSettings::operator == (const AndroidNfcProtocolSettings &other) {
    return this->ordinalValue == other.ordinalValue;
}

bool AndroidNfcProtocolSettings::operator != (const AndroidNfcProtocolSettings &other) {
    return this->ordinalValue != other.ordinalValue;
}

std::vector<AndroidNfcProtocolSettings> AndroidNfcProtocolSettings::values() {
    return valueList;
}

int AndroidNfcProtocolSettings::ordinal() {
    return ordinalValue;
}

std::string AndroidNfcProtocolSettings::toString() {
    return nameValue;
}

AndroidNfcProtocolSettings AndroidNfcProtocolSettings::valueOf(const std::string &name) {
    for (auto enumInstance : AndroidNfcProtocolSettings::valueList) {
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
