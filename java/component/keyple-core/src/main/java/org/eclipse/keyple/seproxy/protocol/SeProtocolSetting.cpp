#include "SeProtocolSetting.h"
#include "SeProtocol.h"
#include "SeProtocolSettingList.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace protocol {

                    SeProtocolSetting::SeProtocolSetting(std::unordered_map<std::shared_ptr<SeProtocol>, std::string> &seProtocolSettingList) {
                        this->protocolsMap = (seProtocolSettingList);
                    }

                    SeProtocolSetting::SeProtocolSetting(std::shared_ptr<SeProtocolSettingList> seProtocolSetting) {
                        this->protocolsMap.emplace(seProtocolSetting->getFlag(), seProtocolSetting->getValue());
                    }

                    SeProtocolSetting::SeProtocolSetting(std::vector<std::shared_ptr<SeProtocolSettingList>> &seProtocolSettingList) {
                        for (auto seProtocolSetting : seProtocolSettingList) {
                            this->protocolsMap.emplace(seProtocolSetting->getFlag(), seProtocolSetting->getValue());
                        }
                    }

                    std::unordered_map<std::shared_ptr<SeProtocol>, std::string> SeProtocolSetting::getProtocolsMap() {
                        return protocolsMap;
                    }
                }
            }
        }
    }
}
