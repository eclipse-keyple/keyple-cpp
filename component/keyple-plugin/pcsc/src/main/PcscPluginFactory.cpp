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

#include "PcscPluginFactory.h"

/* Core */
#include "KeyplePluginInstantiationException.h"
#include "KeypleRuntimeException.h"

/* PC/SC Plugin */
#include "PcscPlugin.h"
#include "PcscPluginImpl.h"

namespace keyple {
namespace plugin {
namespace pcsc {

const std::string& PcscPluginFactory::getPluginName()
{
    return PcscPlugin::PLUGIN_NAME;
}

ReaderPlugin& PcscPluginFactory::getPluginInstance()
{
    try {
        return PcscPluginImpl::getInstance();
    } catch (KeypleRuntimeException& e) {
        throw KeyplePluginInstantiationException(
                  "Can not access Smartcard.io readers, check " \
                  "initNativeReaders trace", e);
    }
}

}
}
}