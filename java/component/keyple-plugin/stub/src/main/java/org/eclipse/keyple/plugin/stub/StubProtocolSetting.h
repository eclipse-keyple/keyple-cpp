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
            namespace plugin {
                namespace stub {


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

                        /**
                         * Regular expressions to match ATRs produced by Stub readers
                         * <p>
                         * To be compared with the StubSE protocol
                         */
//JAVA TO C++ CONVERTER TODO TASK: Java to C++ Converter does not convert types within enums:
//                        public interface ProtocolSetting
                    //    {
                    //        public static String REGEX_PROTOCOL_ISO14443_4 = "PROTOCOL_ISO14443_4";
                    //
                    //        public static String REGEX_PROTOCOL_B_PRIME = "PROTOCOL_B_PRIME";
                    //
                    //        public static String REGEX_PROTOCOL_MIFARE_UL = "PROTOCOL_MIFARE_UL";
                    //
                    //        public static String REGEX_PROTOCOL_MIFARE_CLASSIC = "PROTOCOL_MIFARE_CLASSIC";
                    //
                    //        public static String REGEX_PROTOCOL_MIFARE_DESFIRE = "PROTOCOL_MIFARE_DESFIRE";
                    //
                    //        public static String REGEX_PROTOCOL_MEMORY_ST25 = "PROTOCOL_MEMORY_ST25";
                    //    }

                        /* the protocol flag */
                    public:
                        std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> flag;

                        /* the protocol setting value */
                        std::string value;

                        StubProtocolSetting(const std::string &name, InnerEnum innerEnum, std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> flag, const std::string &value);

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
