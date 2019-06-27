#include "StubProtocolSetting.h"
#include "SeProtocol.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace plugin {
namespace stub {

using SeCommonProtocols = org::eclipse::keyple::core::seproxy::protocol::SeCommonProtocols;
using SeProtocol        = org::eclipse::keyple::core::seproxy::protocol::SeProtocol;

std::unordered_map<SeCommonProtocols, std::string> StubProtocolSetting::STUB_PROTOCOL_SETTING;

StubProtocolSetting::StaticConstructor::StaticConstructor()
{
    std::unordered_map<SeCommonProtocols, std::string> map;
    map.emplace(SeCommonProtocols::PROTOCOL_ISO14443_4, "PROTOCOL_ISO14443_4");
    map.emplace(SeCommonProtocols::PROTOCOL_B_PRIME, "PROTOCOL_B_PRIME");
    map.emplace(SeCommonProtocols::PROTOCOL_MIFARE_UL, "PROTOCOL_MIFARE_UL");
    map.emplace(SeCommonProtocols::PROTOCOL_MIFARE_CLASSIC, "PROTOCOL_MIFARE_CLASSIC");
    map.emplace(SeCommonProtocols::PROTOCOL_MIFARE_DESFIRE, "PROTOCOL_MIFARE_DESFIRE");
    map.emplace(SeCommonProtocols::PROTOCOL_MEMORY_ST25, "PROTOCOL_MEMORY_ST25");
    map.emplace(SeCommonProtocols::PROTOCOL_ISO7816_3, "PROTOCOL_ISO7816_3");
    STUB_PROTOCOL_SETTING = map;
}

StubProtocolSetting::StaticConstructor StubProtocolSetting::staticConstructor;

std::unordered_map<SeCommonProtocols, std::string>
StubProtocolSetting::getSpecificSettings(std::set<SeCommonProtocols> specificProtocols)
{
    std::unordered_map<SeCommonProtocols, std::string> map;

    for (auto seCommonProtocols : specificProtocols) {
        map.emplace(std::make_pair(seCommonProtocols, STUB_PROTOCOL_SETTING[seCommonProtocols]));
    }

    return map;
}

std::unordered_map<SeCommonProtocols, std::string> StubProtocolSetting::getAllSettings()
{
    return STUB_PROTOCOL_SETTING;
}

}
}
}
}
}