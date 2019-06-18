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
#include <unordered_map>
#include <vector>
#include <memory>

/* Common */
#include "Export.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace protocol { class SeProtocol; } } } } } }
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace protocol { class SeProtocolSettingList; } } } } } }

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace protocol {

/**
    * SeProtocol Map builder
    */
class EXPORT SeProtocolSetting : public std::enable_shared_from_this<SeProtocolSetting> {
private:
    std::unordered_map<std::shared_ptr<SeProtocol>, std::string> protocolsMap;

public:
    SeProtocolSetting(std::unordered_map<std::shared_ptr<SeProtocol>, std::string> &seProtocolSettingList);

    SeProtocolSetting(std::shared_ptr<SeProtocolSettingList> seProtocolSetting);

    SeProtocolSetting(std::vector<std::shared_ptr<SeProtocolSettingList>> seProtocolSettingList);

    virtual std::unordered_map<std::shared_ptr<SeProtocol>, std::string> getProtocolsMap();
};

}
}
}
}
}
}
