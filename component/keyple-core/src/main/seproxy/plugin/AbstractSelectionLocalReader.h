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

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

/* Common */
#include "Export.h"
#include "Logger.h"
#include "LoggerFactory.h"

/* Core */
#include "AbstractLocalReader.h"
#include "ObservableReader_Import.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy::exception;

class EXPORT AbstractSelectionLocalReader : public AbstractLocalReader { //, public ObservableReader {
protected:
    /**
     *
     */
    AbstractSelectionLocalReader(const std::string &pluginName,
                                 const std::string &readerName);

    /**
     * Executes the selection application command and returns the requested data
     * according to AidSelector attributes.
     *
     * @param aidSelector the selection parameters
     * @return the response to the select application command
     * @throws KeypleIOReaderException if a reader error occurs
     */
    std::shared_ptr<ApduResponse> openChannelForAid(
      std::shared_ptr<SeSelector::AidSelector> aidSelector) override;

    /**
     *
     */
    std::shared_ptr<AbstractSelectionLocalReader> shared_from_this()
    {
        return std::static_pointer_cast<AbstractSelectionLocalReader>(
                   AbstractLocalReader::shared_from_this());
    }

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(AbstractSelectionLocalReader));
};

}
}
}
}
