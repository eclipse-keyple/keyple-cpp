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

#pragma once

/* Core */
#include "PluginFactory.h"

/* Plugin */
#include "KeyplePluginIOSExport.h"

namespace keyple {
namespace plugin {
namespace ios {

using namespace keyple::core::seproxy;

/**
 * Builds a keyple::plugin::IOS::IOSPlugin
 */
class KEYPLEPLUGINIOS_API IOSPluginFactory : public PluginFactory {
public:
    /**
     *
     */
    const std::string& getPluginName() const override;

    /**
     * Returns an instance of the keyple::plugin::IOS::IOSPlugin if the platform is ready
     *
     * @return IOSPlugin instance
     * @throws KeyplePluginInstantiationException if Smartcard.io library is not
     * ready
     */
    std::shared_ptr<ReaderPlugin> getPlugin() const override;
};

}
}
}
