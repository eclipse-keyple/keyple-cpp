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

#include "StubProtocolSetting.h"
#include "SeProtocol_Import.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::seproxy::protocol;

std::map<SeCommonProtocols, std::string>
    StubProtocolSetting::STUB_PROTOCOL_SETTING;

StubProtocolSetting::StaticConstructor::StaticConstructor()
{
    STUB_PROTOCOL_SETTING.emplace(SeCommonProtocols::PROTOCOL_ISO14443_4,
                                  "PROTOCOL_ISO14443_4");
    STUB_PROTOCOL_SETTING.emplace(SeCommonProtocols::PROTOCOL_B_PRIME,
                                  "PROTOCOL_B_PRIME");
    STUB_PROTOCOL_SETTING.emplace(SeCommonProtocols::PROTOCOL_MIFARE_UL,
                                  "PROTOCOL_MIFARE_UL");
    STUB_PROTOCOL_SETTING.emplace(SeCommonProtocols::PROTOCOL_MIFARE_CLASSIC,
                                  "PROTOCOL_MIFARE_CLASSIC");
    STUB_PROTOCOL_SETTING.emplace(SeCommonProtocols::PROTOCOL_MIFARE_DESFIRE,
                                  "PROTOCOL_MIFARE_DESFIRE");
    STUB_PROTOCOL_SETTING.emplace(SeCommonProtocols::PROTOCOL_MEMORY_ST25,
                                  "PROTOCOL_MEMORY_ST25");
    STUB_PROTOCOL_SETTING.emplace(SeCommonProtocols::PROTOCOL_ISO7816_3,
                                  "PROTOCOL_ISO7816_3");
}

StubProtocolSetting::StaticConstructor StubProtocolSetting::staticConstructor;

std::map<SeCommonProtocols, std::string>
StubProtocolSetting::getSpecificSettings(
    std::set<SeCommonProtocols> specificProtocols)
{
    std::map<SeCommonProtocols, std::string> map;

    for (auto seCommonProtocols : specificProtocols) {
        map.emplace(std::make_pair(seCommonProtocols,
                                   STUB_PROTOCOL_SETTING[seCommonProtocols]));
    }

    return map;
}

std::map<SeCommonProtocols, std::string> StubProtocolSetting::getAllSettings()
{
    return STUB_PROTOCOL_SETTING;
}

}
}
}
