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
#include <memory>

/* Common */
#include "Export.h"

/* Core */
#include "SeProtocol.h"

namespace org {
    namespace eclipse {
        namespace keyple {
            namespace seproxy {
                namespace protocol {

                    /**
                     * Interface to be implemented by SeProtocolSetting enums
                     */
                    class EXPORT SeProtocolSettingList {
                    public:
                        virtual std::shared_ptr<SeProtocol> getFlag() = 0;

                        virtual std::string getValue() = 0;
                    };

                }
            }
        }
    }
}
