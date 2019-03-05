/* Core - Seproxy - Protocol */
#include "ContactlessProtocols_Import.h"

/* Plugin - Stub */
#include "StubProtocolSetting.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace stub {

                    using ContactlessProtocols =
                        org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;
                    using SeProtocol = org::eclipse::keyple::seproxy::protocol::SeProtocol;
                    using SeProtocolSettingList =
                        org::eclipse::keyple::seproxy::protocol::SeProtocolSettingList;

                    StubProtocolSetting StubProtocolSetting::SETTING_PROTOCOL_ISO14443_4(
                        "SETTING_PROTOCOL_ISO14443_4", InnerEnum::SETTING_PROTOCOL_ISO14443_4, ContactlessProtocols::PROTOCOL_ISO14443_4,
                        ProtocolSetting::REGEX_PROTOCOL_ISO14443_4);
                    StubProtocolSetting StubProtocolSetting::SETTING_PROTOCOL_B_PRIME(
                        "SETTING_PROTOCOL_B_PRIME", InnerEnum::SETTING_PROTOCOL_B_PRIME, ContactlessProtocols::PROTOCOL_B_PRIME,
                        ProtocolSetting::REGEX_PROTOCOL_B_PRIME);
                    StubProtocolSetting StubProtocolSetting::SETTING_PROTOCOL_MIFARE_UL(
                        "SETTING_PROTOCOL_MIFARE_UL", InnerEnum::SETTING_PROTOCOL_MIFARE_UL, ContactlessProtocols::PROTOCOL_MIFARE_UL,
                        ProtocolSetting::REGEX_PROTOCOL_MIFARE_UL);
                    StubProtocolSetting StubProtocolSetting::SETTING_PROTOCOL_MIFARE_CLASSIC(
                        "SETTING_PROTOCOL_MIFARE_CLASSIC", InnerEnum::SETTING_PROTOCOL_MIFARE_CLASSIC, ContactlessProtocols::
                            PROTOCOL_MIFARE_CLASSIC,
                        ProtocolSetting::REGEX_PROTOCOL_MIFARE_CLASSIC);
                    StubProtocolSetting StubProtocolSetting::SETTING_PROTOCOL_MIFARE_DESFIRE(
                        "SETTING_PROTOCOL_MIFARE_DESFIRE", InnerEnum::SETTING_PROTOCOL_MIFARE_DESFIRE, ContactlessProtocols::
                            PROTOCOL_MIFARE_DESFIRE,
                        ProtocolSetting::REGEX_PROTOCOL_MIFARE_DESFIRE);
                    StubProtocolSetting StubProtocolSetting::SETTING_PROTOCOL_MEMORY_ST25(
                        "SETTING_PROTOCOL_MEMORY_ST25", InnerEnum::SETTING_PROTOCOL_MEMORY_ST25, ContactlessProtocols::PROTOCOL_MEMORY_ST25,
                        ProtocolSetting::REGEX_PROTOCOL_MEMORY_ST25);

                    const std::string StubProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_ISO14443_4 = 
                        "PROTOCOL_ISO14443_4";
                    const std::string StubProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_B_PRIME =
                        "PROTOCOL_B_PRIME";
                    const std::string StubProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_MIFARE_UL =
                        "PROTOCOL_MIFARE_UL";
                    const std::string StubProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_MIFARE_CLASSIC =
                        "PROTOCOL_MIFARE_CLASSIC";
                    const std::string StubProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_MIFARE_DESFIRE =
                        "PROTOCOL_MIFARE_DESFIRE";
                    const std::string StubProtocolSetting::ProtocolSetting::REGEX_PROTOCOL_MEMORY_ST25 =
                        "PROTOCOL_MEMORY_ST25";

                    std::vector<StubProtocolSetting> StubProtocolSetting::valueList;

                    StubProtocolSetting::StaticConstructor::StaticConstructor()
                    {
                        valueList.push_back(SETTING_PROTOCOL_ISO14443_4);
                        valueList.push_back(SETTING_PROTOCOL_B_PRIME);
                        valueList.push_back(SETTING_PROTOCOL_MIFARE_UL);
                        valueList.push_back(SETTING_PROTOCOL_MIFARE_CLASSIC);
                        valueList.push_back(SETTING_PROTOCOL_MIFARE_DESFIRE);
                        valueList.push_back(SETTING_PROTOCOL_MEMORY_ST25);
                    }

                    StubProtocolSetting::StaticConstructor StubProtocolSetting::staticConstructor;
                    int StubProtocolSetting::nextOrdinal = 0;

                    StubProtocolSetting::StubProtocolSetting(const std::string &name, InnerEnum innerEnum, ContactlessProtocols& flag, const std::string &value)
                        : nameValue(name), ordinalValue(nextOrdinal++), innerEnumValue(innerEnum), flag(flag)
                    {
                        this->value = value;
                    }

                    std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol>
                    StubProtocolSetting::getFlag()
                    {
                        return std::dynamic_pointer_cast<SeProtocol>(
                            std::make_shared<ContactlessProtocols>(this->flag));
                    }

                    std::string StubProtocolSetting::getValue()
                    {
                        return this->value;
                    }

                    bool StubProtocolSetting::operator==(const StubProtocolSetting &other)
                    {
                        return this->ordinalValue == other.ordinalValue;
                    }

                    bool StubProtocolSetting::operator!=(const StubProtocolSetting &other)
                    {
                        return this->ordinalValue != other.ordinalValue;
                    }

                    std::vector<StubProtocolSetting> StubProtocolSetting::values()
                    {
                        return valueList;
                    }

                    int StubProtocolSetting::ordinal()
                    {
                        return ordinalValue;
                    }

                    std::string StubProtocolSetting::toString()
                    {
                        return nameValue;
                    }

                    StubProtocolSetting StubProtocolSetting::valueOf(const std::string &name)
                    {
                        for (auto enumInstance : StubProtocolSetting::valueList)
                        {
                            if (enumInstance.nameValue == name)
                            {
                                return enumInstance;
                            }
                        }

			/* Avoid C++ compiler warning */
			return StubProtocolSetting::valueList.front();
                    }
                } // namespace stub
            }     // namespace plugin
        }         // namespace keyple
    }             // namespace eclipse
} // namespace org
