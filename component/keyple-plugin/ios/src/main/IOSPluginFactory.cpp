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

#include "IOSPluginFactory.h"

/* Common */
#include "Exception.h"

/* Core */
#include "KeyplePluginInstantiationException.h"

/* iOS Plugin */
#include "IOSPlugin.h"
#include "IOSPluginImpl.h"

namespace keyple {
namespace plugin {
namespace ios {

using namespace keyple::core::seproxy::exception;

const std::string& IOSPluginFactory::getPluginName() const
{
    return IOSPlugin::PLUGIN_NAME;
}

std::shared_ptr<ReaderPlugin> IOSPluginFactory::getPlugin() const
{
    try {
        return IOSPluginImpl::getInstance();
    } catch (const Exception& e) {
        throw KeyplePluginInstantiationException(
                  "Can not access Smartcard.io readers, check createVirtual trace", e);
    }
}

}
}
}
