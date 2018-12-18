#pragma once

#include <string>
#include <vector>

/********************************************************************************
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information regarding copyright
 * ownership.
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/
namespace org {
    namespace eclipse {
        namespace keyple {
            namespace example {
                namespace generic_Renamed {
                    namespace common {


                        /**
                         * Custom protocol setting definitions to illustrate the extension of the Keyple SDK definitions
                         */
                        class CustomProtocolSetting final : public SeProtocolSettingList {
public:
                            static CustomProtocolSetting CUSTOM_SETTING_PROTOCOL_B_PRIME;

                            static CustomProtocolSetting CUSTOM_SETTING_PROTOCOL_ISO14443_4;

private:
                            static std::vector<CustomProtocolSetting> valueList;

                            class StaticConstructor {
                            public:
                                StaticConstructor();
                            };

                            static StaticConstructor staticConstructor;

public:
                            enum class InnerEnum {
                                CUSTOM_SETTING_PROTOCOL_B_PRIME,
                                CUSTOM_SETTING_PROTOCOL_ISO14443_4
                            };

                            const InnerEnum innerEnumValue;
private:
                            const std::string nameValue;
                            const int ordinalValue;
                            static int nextOrdinal;

                            /* the protocol flag */
                        public:
                            std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> flag;

                            /* the protocol setting value */
                            std::string value;

                            CustomProtocolSetting(const std::string &name, InnerEnum innerEnum, std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> flag, const std::string &value);

                            std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> getFlag() override;

                            std::string getValue() override;

public:
                            bool operator == (const CustomProtocolSetting &other);

                            bool operator != (const CustomProtocolSetting &other);

                            static std::vector<CustomProtocolSetting> values();

                            int ordinal();

                            std::string toString();

                            static CustomProtocolSetting valueOf(const std::string &name);
                        };

                    }
                }
            }
        }
    }
}
