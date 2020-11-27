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
#include "ReaderPlugin.h"

/* Stub plugin */
#include "StubPoolPluginImpl.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy;

/**
 * Instantiate a {@link StubPoolPlugin} with a custom plugin name
 */
class StubPoolPluginFactory : public PluginFactory {
public:
    /**
     * Create the factory
     *
     * @param pluginName name of the plugin that will be instantiated
     */
    explicit StubPoolPluginFactory(const std::string& pluginName);

    /**
     *
     */
    virtual ~StubPoolPluginFactory() = default;

    /**
     *
     */
    const std::string& getPluginName() const override;

    /**
     * protected in Java ?
     */
    std::shared_ptr<ReaderPlugin> getPlugin() const override;

private:
    /**
     *
     */
    const std::string mPluginName;
};

}
}
}
