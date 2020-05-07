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

#include "StubPluginFactory.h"

/* Core */
#include "KeyplePluginInstantiationException.h"
#include "KeypleRuntimeException.h"

/* Stub Plugin */
#include "StubPluginImpl.h"

namespace keyple {
namespace plugin {
namespace stub {

StubPluginFactory::StubPluginFactory(const std::string pluginName)
: pluginName(pluginName)
{
}

const std::string& StubPluginFactory::getPluginName()
{
    return pluginName;
}

ReaderPlugin& StubPluginFactory::getPluginInstance()
{
    return *(new StubPluginImpl(pluginName));
}

}
}
}