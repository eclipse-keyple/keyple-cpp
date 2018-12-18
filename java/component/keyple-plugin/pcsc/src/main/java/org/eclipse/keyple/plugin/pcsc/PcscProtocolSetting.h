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
                namespace pcsc {


                    /**
                     * These objects are used by the application to build the SeProtocolsMap
                     */
                    class PcscProtocolSetting final : public SeProtocolSettingList {

public:
                        static PcscProtocolSetting SETTING_PROTOCOL_ISO14443_4;

                        static PcscProtocolSetting SETTING_PROTOCOL_B_PRIME;

                        static PcscProtocolSetting SETTING_PROTOCOL_MIFARE_UL;

                        static PcscProtocolSetting SETTING_PROTOCOL_MIFARE_CLASSIC;

                        static PcscProtocolSetting SETTING_PROTOCOL_MIFARE_DESFIRE;

                        static PcscProtocolSetting SETTING_PROTOCOL_MEMORY_ST25;

private:
                        static std::vector<PcscProtocolSetting> valueList;

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
                         * Regular expressions to match ATRs produced by PcSc readers
                         */
//JAVA TO C++ CONVERTER TODO TASK: Java to C++ Converter does not convert types within enums:
//                        public interface ProtocolSetting
                    //    {
                    //        public static String REGEX_PROTOCOL_ISO14443_4 = "3B8880010000000000718100F9|3B8C800150........00000000007181..";
                    //
                    //        public static String REGEX_PROTOCOL_B_PRIME = "3B8F8001805A0A0103200311........829000..";
                    //
                    //        public static String REGEX_PROTOCOL_MIFARE_UL = "3B8F8001804F0CA0000003060300030000000068";
                    //
                    //        public static String REGEX_PROTOCOL_MIFARE_CLASSIC = "3B8F8001804F0CA000000306030001000000006A";
                    //
                    //        public static String REGEX_PROTOCOL_MIFARE_DESFIRE = "3B8180018080";
                    //
                    //        public static String REGEX_PROTOCOL_MEMORY_ST25 = "3B8F8001804F0CA000000306070007D0020C00B6";
                    //    }

                        /* the protocol flag */
                    public:
                        std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> flag;

                        /* the protocol setting value */
                        std::string value;

                        PcscProtocolSetting(const std::string &name, InnerEnum innerEnum, std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> flag, const std::string &value);

                        std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> getFlag() override;

                        std::string getValue() override;

public:
                        bool operator == (const PcscProtocolSetting &other);

                        bool operator != (const PcscProtocolSetting &other);

                        static std::vector<PcscProtocolSetting> values();

                        int ordinal();

                        std::string toString();

                        static PcscProtocolSetting valueOf(const std::string &name);
                    };

                }
            }
        }
    }
}
