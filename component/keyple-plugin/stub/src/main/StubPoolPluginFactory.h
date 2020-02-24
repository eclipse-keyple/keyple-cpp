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

#pragma once

/* Core */
#include "AbstractPluginFactory.h"
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
class StubPoolPluginFactory : public AbstractPluginFactory {
public:
    /**
     * Create the factory
     *
     * @param pluginName name of the plugin that will be instantiated
     */
    StubPoolPluginFactory(const std::string& pluginName);

    /**
     *
     */
    const std::string& getPluginName() override;

    /**
     *
     */
    ReaderPlugin& getPluginInstance();

private:
    /**
     *
     */
    const std::string pluginName;
};

}
}
}
