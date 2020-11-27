/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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
#include "PluginFactory.h"

/* Plugin */
#include "KeyplePluginStubExport.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy;

/**
 * Builds a {@link StubPlugin}
 */
class KEYPLEPLUGINSTUB_API StubPluginFactory final : public PluginFactory {
public:
    /**
     * Create the factory
     *
     * @param pluginName name of the plugin that will be instantiated
     */
    explicit StubPluginFactory(const std::string& pluginName);

    /**
     *
     */
    const std::string& getPluginName() const override;

    /**
     * Returns an instance of the {@link PcscPlugin} if the platform is ready
     *
     * @return PcscPlugin instance
     * @throw KeyplePluginInstantiationException if Smartcard.io library is not
     *        ready
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
