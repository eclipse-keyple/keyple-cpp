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

/* Plugin */
#include "KeyplePluginStubExport.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy;

/**
 * Builds a {@link StubPlugin}
 */
class KEYPLEPLUGINSTUB_API StubPluginFactory : public AbstractPluginFactory {
public:
    /**
     * Create the factory
     *
     * @param pluginName name of the plugin that will be instantiated
     */
    StubPluginFactory(const std::string pluginName);

    /**
     *
     */
    virtual ~StubPluginFactory() = default;

    /**
     *
     */
    const std::string& getPluginName() override;

    /**
     * Returns an instance of the {@link PcscPlugin} if the platform is ready
     *
     * @return PcscPlugin instance
     * @throws KeyplePluginInstantiationException if Smartcard.io library is not
     * ready
     */
    ReaderPlugin& getPluginInstance() override;

private:
    /**
     *
     */
    const std::string pluginName;
};

}
}
}
