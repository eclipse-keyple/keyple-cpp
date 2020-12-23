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
#include "KeyplePluginPcscExport.h"

namespace keyple {
namespace plugin {
namespace pcsc {

using namespace keyple::core::seproxy;

/**
 * Builds a keyple::plugin::pcsc::PcscPlugin
 */
class KEYPLEPLUGINPCSC_API PcscPluginFactory : public PluginFactory {
public:
    /**
     *
     */
    const std::string& getPluginName() const override;

    /**
     * Returns an instance of the keyple::plugin::pcsc::PcscPlugin if the platform is ready
     *
     * @return PcscPlugin instance
     * @throws KeyplePluginInstantiationException if Smartcard.io library is not
     * ready
     */
    std::shared_ptr<ReaderPlugin> getPlugin() const override;
};

}
}
}
