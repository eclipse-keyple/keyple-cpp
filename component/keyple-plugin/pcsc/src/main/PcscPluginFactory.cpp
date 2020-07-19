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

#include "PcscPluginFactory.h"

/* Common */
#include "Exception.h"

/* Core */
#include "KeyplePluginInstantiationException.h"

/* PC/SC Plugin */
#include "PcscPlugin.h"
#include "PcscPluginImpl.h"

namespace keyple {
namespace plugin {
namespace pcsc {

using namespace keyple::core::seproxy::exception;

const std::string& PcscPluginFactory::getPluginName() const
{
    return PcscPlugin::PLUGIN_NAME;
}

std::shared_ptr<ReaderPlugin> PcscPluginFactory::getPlugin() const
{
    try {
        return PcscPluginImpl::getInstance();
    } catch (const Exception& e) {
        throw KeyplePluginInstantiationException(
            "Can not access Smartcard.io readers, check "
            "createVirtual trace",
            e);
    }
}

}
}
}