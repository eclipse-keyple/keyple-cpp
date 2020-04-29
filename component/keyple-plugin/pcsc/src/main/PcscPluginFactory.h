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
#include "KeyplePluginPcscExport.h"

namespace keyple {
namespace plugin {
namespace pcsc {

using namespace keyple::core::seproxy;

/**
 * Builds a {@link PcscPlugin}
 */
class KEYPLEPLUGINPCSC_API PcscPluginFactory : public AbstractPluginFactory {
public:
    /**
     *
     */
    const std::string& getPluginName() override;

protected:
    /**
     * Returns an instance of the {@link PcscPlugin} if the platform is ready
     *
     * @return PcscPlugin instance
     * @throws KeyplePluginInstantiationException if Smartcard.io library is not
     * ready
     */
    ReaderPlugin& getPluginInstance() override;
};

}
}
}