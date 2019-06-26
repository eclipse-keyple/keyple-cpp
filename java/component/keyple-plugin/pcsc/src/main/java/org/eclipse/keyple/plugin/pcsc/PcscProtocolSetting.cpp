#include "PcscProtocolSetting.h"
#include "SeCommonProtocols_Import.h"
#include "SeProtocol.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace plugin {
namespace pcsc {

using SeCommonProtocols = org::eclipse::keyple::core::seproxy::protocol::SeCommonProtocols;
using SeProtocol        = org::eclipse::keyple::core::seproxy::protocol::SeProtocol;

std::unordered_map<SeCommonProtocols, std::string> PcscProtocolSetting::PCSC_PROTOCOL_SETTING;

PcscProtocolSetting::StaticConstructor::StaticConstructor()
{
    std::unordered_map<SeCommonProtocols, std::string> map;
    map.emplace(SeCommonProtocols::PROTOCOL_ISO14443_4, "3B8880....................|3B8C800150.*|.*4F4D4141544C4153.*");
    map.emplace(SeCommonProtocols::PROTOCOL_B_PRIME, "3B8F8001805A0A0103200311........829000..");
    map.emplace(SeCommonProtocols::PROTOCOL_MIFARE_UL, "3B8F8001804F0CA0000003060300030000000068");
    map.emplace(SeCommonProtocols::PROTOCOL_MIFARE_CLASSIC, "3B8F8001804F0CA000000306030001000000006A");
    map.emplace(SeCommonProtocols::PROTOCOL_MIFARE_DESFIRE, "3B8180018080");
    map.emplace(SeCommonProtocols::PROTOCOL_MEMORY_ST25, "3B8F8001804F0CA000000306070007D0020C00B6");
    map.emplace(SeCommonProtocols::PROTOCOL_ISO7816_3, "3.*");
    PCSC_PROTOCOL_SETTING = map;
}

PcscProtocolSetting::StaticConstructor PcscProtocolSetting::staticConstructor;

std::unordered_map<SeCommonProtocols, std::string>& 
PcscProtocolSetting::getSpecificSettings(std::set<SeCommonProtocols>& specificProtocols)
{
    std::unordered_map<SeCommonProtocols, std::string> map;
    
    for (auto seCommonProtocols : specificProtocols) {
        map.emplace(std::make_pair(seCommonProtocols, PCSC_PROTOCOL_SETTING[seCommonProtocols]));
    }

    return map;
}

std::unordered_map<SeCommonProtocols, std::string>& PcscProtocolSetting::getAllSettings()
{
    return PCSC_PROTOCOL_SETTING;
}

}
}
}
}
}
