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

#include "StubPoolPluginFactory.h"

/* Core */
#include "KeyplePluginInstantiationException.h"

/* Stub */
#include "StubPoolPluginImpl.h"

namespace keyple {
namespace plugin {
namespace stub {

using namespace keyple::core::seproxy;


StubPoolPluginFactory::StubPoolPluginFactory(const std::string& pluginName)
: mPluginName(pluginName)
{
}

const std::string& StubPoolPluginFactory::getPluginName() const
{
    return mPluginName;
}

std::shared_ptr<ReaderPlugin> StubPoolPluginFactory::getPlugin() const
{
    try {
        return std::make_shared<StubPoolPluginImpl>(mPluginName);
    } catch (const Exception& e) {
        throw KeyplePluginInstantiationException("Cannot access StubPool", e);
    }
}

}
}
}
