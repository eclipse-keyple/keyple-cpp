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
                namespace android {
                    namespace nfc {


                        class AndroidNfcProtocolSettings final : public SeProtocolSettingList {

public:
                            static AndroidNfcProtocolSettings SETTING_PROTOCOL_ISO14443_4;

                            static AndroidNfcProtocolSettings SETTING_PROTOCOL_MIFARE_UL;

                            static AndroidNfcProtocolSettings SETTING_PROTOCOL_MIFARE_CLASSIC;

private:
                            static std::vector<AndroidNfcProtocolSettings> valueList;

                            class StaticConstructor {
                            public:
                                StaticConstructor();
                            };

                            static StaticConstructor staticConstructor;

public:
                            enum class InnerEnum {
                                SETTING_PROTOCOL_ISO14443_4,
                                SETTING_PROTOCOL_MIFARE_UL,
                                SETTING_PROTOCOL_MIFARE_CLASSIC
                            };

                            const InnerEnum innerEnumValue;
private:
                            const std::string nameValue;
                            const int ordinalValue;
                            static int nextOrdinal;

                        private:
                            const std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> flag;
                            const std::string value;

                        public:
                            AndroidNfcProtocolSettings(const std::string &name, InnerEnum innerEnum, std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> flag, const std::string &value);

                            std::shared_ptr<org::eclipse::keyple::seproxy::protocol::SeProtocol> getFlag() override;

                            std::string getValue() override;

//JAVA TO C++ CONVERTER TODO TASK: Java to C++ Converter does not convert types within enums:
//                            public interface ProtocolSetting
                        //    {
                        //
                        //        String NFC_TAG_TYPE_ISODEP = "android.nfc.tech.IsoDep";
                        //
                        //        String NFC_TAG_TYPE_MIFARE_UL = "android.nfc.tech.MifareUltralight";
                        //
                        //        String NFC_TAG_TYPE_MIFARE_CLASSIC = "android.nfc.tech.MifareClassic";
                        //
                        //
                        //
                        //    }

public:
                            bool operator == (const AndroidNfcProtocolSettings &other);

                            bool operator != (const AndroidNfcProtocolSettings &other);

                            static std::vector<AndroidNfcProtocolSettings> values();

                            int ordinal();

                            std::string toString();

                            static AndroidNfcProtocolSettings valueOf(const std::string &name);
                        };

                    }
                }
            }
        }
    }
}
