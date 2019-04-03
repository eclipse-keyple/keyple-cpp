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

#pragma once

#include <string>
#include <vector>

/* Core - Seproxy - Protocol */
#include "ContactlessProtocols_Import.h"
#include "SeProtocolSettingList.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace plugin {
                namespace stub {

                    using SeProtocolSettingList =
                        org::eclipse::keyple::seproxy::protocol::SeProtocolSettingList;
                    using ContactlessProtocols =
                        org::eclipse::keyple::seproxy::protocol::ContactlessProtocols;

                    /**
                     * These objects are used by the application to build the SeProtocolsMap
                     */
                    class StubProtocolSetting final : public SeProtocolSettingList {

public:
                        static StubProtocolSetting SETTING_PROTOCOL_ISO14443_4;
                        static StubProtocolSetting SETTING_PROTOCOL_B_PRIME;
                        static StubProtocolSetting SETTING_PROTOCOL_MIFARE_UL;
                        static StubProtocolSetting SETTING_PROTOCOL_MIFARE_CLASSIC;
                        static StubProtocolSetting SETTING_PROTOCOL_MIFARE_DESFIRE;
                        static StubProtocolSetting SETTING_PROTOCOL_MEMORY_ST25;

private:
                        static std::vector<StubProtocolSetting> valueList;

                        class StaticConstructor {
                        public:
                            StaticConstructor();
                        };

                        static StaticConstructor staticConstructor;

public:
                        enum class InnerEnum {
                            SETTING_PROTOCOL_ISO14443_4,
                            SETTING_PROTOCOL_B_PRIME,
                            SETTING_PROTOCOL_MIFARE_UL,
                            SETTING_PROTOCOL_MIFARE_CLASSIC,
                            SETTING_PROTOCOL_MIFARE_DESFIRE,
                            SETTING_PROTOCOL_MEMORY_ST25
                        };

                        const InnerEnum innerEnumValue;
private:
                        const std::string nameValue;
                        const int ordinalValue;
                        static int nextOrdinal;

                        
                      public:
                        /**
                         * Regular expressions to match ATRs produced by Stub readers
                         * <p>
                         * To be compared with the StubSE protocol
                         */
                        class ProtocolSetting
                        {
                          public:
                            static const std::string REGEX_PROTOCOL_ISO14443_4;
                            static const std::string REGEX_PROTOCOL_B_PRIME;
                            static const std::string REGEX_PROTOCOL_MIFARE_UL;
                            static const std::string REGEX_PROTOCOL_MIFARE_CLASSIC;
                            static const std::string REGEX_PROTOCOL_MIFARE_DESFIRE;
                            static const std::string REGEX_PROTOCOL_MEMORY_ST25;
                        };

                        /* the protocol flag */
                    public:
                        ContactlessProtocols flag;

                        /* the protocol setting value */
                        std::string value;

                        StubProtocolSetting(const std::string &name, InnerEnum innerEnum, ContactlessProtocols& flag, const std::string &value);

                        std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> getFlag() override;

                        std::string getValue() override;

public:
                        bool operator == (const StubProtocolSetting &other);

                        bool operator != (const StubProtocolSetting &other);

                        static std::vector<StubProtocolSetting> values();

                        int ordinal();

                        std::string toString();

                        static StubProtocolSetting valueOf(const std::string &name);
                    };

                }
            }
        }
    }
}
