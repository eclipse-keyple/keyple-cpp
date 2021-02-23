/**************************************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                                                *
 * https://www.calypsonet-asso.org/                                                               *
 *                                                                                                *
 * See the NOTICE file(s) distributed with this work for additional information regarding         *
 * copyright ownership.                                                                           *
 *                                                                                                *
 * This program and the accompanying materials are made available under the terms of the Eclipse  *
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0                  *
 *                                                                                                *
 * SPDX-License-Identifier: EPL-2.0                                                               *
 **************************************************************************************************/

#include "IOSProtocolSetting.h"

namespace keyple {
namespace plugin {
namespace ios {

using namespace keyple::core::seproxy::protocol;

std::map<std::shared_ptr<SeCommonProtocol>, std::string> IOSProtocolSetting::IOS_PROTOCOL_SETTING;

IOSProtocolSetting::StaticConstructor::StaticConstructor()
{
    IOS_PROTOCOL_SETTING.emplace(SeCommonProtocols::PROTOCOL_ISO14443_4,
                                  "3B8880....................|3B8B80.*|"
                                  "3B8C800150.*|.*4F4D4141544C4153.*");
    IOS_PROTOCOL_SETTING.emplace(SeCommonProtocols::PROTOCOL_B_PRIME,
                                  "3B8F8001805A0...................829000..");
    IOS_PROTOCOL_SETTING.emplace(SeCommonProtocols::PROTOCOL_MIFARE_UL,
                                  "3B8F8001804F0CA0000003060300030000000068");
    IOS_PROTOCOL_SETTING.emplace(SeCommonProtocols::PROTOCOL_MIFARE_CLASSIC,
                                  "3B8F8001804F0CA000000306030001000000006A");
    IOS_PROTOCOL_SETTING.emplace(SeCommonProtocols::PROTOCOL_MIFARE_DESFIRE,
                                  "3B8180018080");
    IOS_PROTOCOL_SETTING.emplace(SeCommonProtocols::PROTOCOL_MEMORY_ST25,
                                  "3B8F8001804F0CA000000306070007D0020C00B6");
    IOS_PROTOCOL_SETTING.emplace(SeCommonProtocols::PROTOCOL_ISO7816_3, "3.*");
}

IOSProtocolSetting::StaticConstructor IOSProtocolSetting::staticConstructor;

const std::map<std::shared_ptr<SeCommonProtocol>, std::string>
    IOSProtocolSetting::getSpecificSettings(
        const std::set<std::shared_ptr<SeCommonProtocol>>& specificProtocols)
{
    std::map<std::shared_ptr<SeCommonProtocol>, std::string> map;

    for (auto seCommonProtocols : specificProtocols) {
        map.emplace(std::make_pair(seCommonProtocols,
                                   IOS_PROTOCOL_SETTING[seCommonProtocols]));
    }

    return map;
}

const std::map<std::shared_ptr<SeCommonProtocol>, std::string>&
    IOSProtocolSetting::getAllSettings()
{
    return IOS_PROTOCOL_SETTING;
}

}
}
}
