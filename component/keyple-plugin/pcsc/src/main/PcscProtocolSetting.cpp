/******************************************************************************
 * Copyright (c) 2018 Calypso Networks Association                            *
 * https://www.calypsonet-asso.org/                                           *
 *                                                                            *
 * See the NOTICE file(s) distributed with this work for additional           *
 * information regarding copyright ownership.                                 *
 *                                                                            *
 * This program and the accompanying materials are made available under the   *
 * terms of the Eclipse Public License 2.0 which is available at              *
 * http://www.eclipse.org/legal/epl-2.0                                       *
 *                                                                            *
 * SPDX-License-Identifier: EPL-2.0                                           *
 ******************************************************************************/

#include "PcscProtocolSetting.h"

namespace keyple {
namespace plugin {
namespace pcsc {

using namespace keyple::core::seproxy::protocol;

std::map<SeCommonProtocols, std::string>
PcscProtocolSetting::PCSC_PROTOCOL_SETTING;

PcscProtocolSetting::StaticConstructor::StaticConstructor()
{
    PCSC_PROTOCOL_SETTING.emplace(
       SeCommonProtocols::PROTOCOL_ISO14443_4,
       "3B8880....................|3B8B80.*|3B8C800150.*|.*4F4D4141544C4153.*");
    PCSC_PROTOCOL_SETTING.emplace(
        SeCommonProtocols::PROTOCOL_B_PRIME,
        "3B8F8001805A0...................829000..");
    PCSC_PROTOCOL_SETTING.emplace(
        SeCommonProtocols::PROTOCOL_MIFARE_UL,
        "3B8F8001804F0CA0000003060300030000000068");
    PCSC_PROTOCOL_SETTING.emplace(
        SeCommonProtocols::PROTOCOL_MIFARE_CLASSIC,
        "3B8F8001804F0CA000000306030001000000006A");
    PCSC_PROTOCOL_SETTING.emplace(
        SeCommonProtocols::PROTOCOL_MIFARE_DESFIRE,
        "3B8180018080");
    PCSC_PROTOCOL_SETTING.emplace(
        SeCommonProtocols::PROTOCOL_MEMORY_ST25,
        "3B8F8001804F0CA000000306070007D0020C00B6");
    PCSC_PROTOCOL_SETTING.emplace(
        SeCommonProtocols::PROTOCOL_ISO7816_3,
        "3.*");
}

PcscProtocolSetting::StaticConstructor PcscProtocolSetting::staticConstructor;

std::map<SeCommonProtocols, std::string>
    PcscProtocolSetting::getSpecificSettings(
        std::set<SeCommonProtocols>& specificProtocols)
{
    std::map<SeCommonProtocols, std::string> map;

    for (auto seCommonProtocols : specificProtocols) {
        map.emplace(std::make_pair(seCommonProtocols,
                                   PCSC_PROTOCOL_SETTING[seCommonProtocols]));
    }

    return map;
}

std::map<SeCommonProtocols, std::string> PcscProtocolSetting::getAllSettings()
{
    return PCSC_PROTOCOL_SETTING;
}

}
}
}
