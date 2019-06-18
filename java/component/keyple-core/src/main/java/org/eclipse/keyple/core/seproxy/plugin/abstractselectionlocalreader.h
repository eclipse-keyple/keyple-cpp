/********************************************************************************
 * Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
 *
 * See the NOTICE file(s) distributed with this work for additional information regarding copyright
 * ownership.
 *
 * This program and the accompanying materials are made available under the terms of the Eclipse
 * Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
 *
 * SPDX-License-Identifier: EPL-2.0
 ********************************************************************************/

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
#include "ObservableReader.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using KeypleApplicationSelectionException = org::eclipse::keyple::core::seproxy::exception::KeypleApplicationSelectionException;
using KeypleChannelStateException         = org::eclipse::keyple::core::seproxy::exception::KeypleChannelStateException;
using KeypleIOReaderException             = org::eclipse::keyple::core::seproxy::exception::KeypleIOReaderException;

class EXPORT AbstractSelectionLocalReader : public AbstractLocalReader { //, public ObservableReader {
 private:
                        const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(AbstractSelectionLocalReader));

protected:
    AbstractSelectionLocalReader(const std::string &pluginName, const std::string &readerName);

                        /** ==== ATR filtering and application selection by AID ================ */

    /**
                         * Build a select application command, transmit it to the SE and deduct the SelectionStatus.
     *
                         * @param seSelector the targeted application SE selector
                         * @return the SelectionStatus
                         * @throws KeypleIOReaderException if a reader error occurs
     */
                        std::shared_ptr<SelectionStatus> openLogicalChannel(std::shared_ptr<SeSelector> seSelector) override;

protected:
                        std::shared_ptr<AbstractSelectionLocalReader> shared_from_this() {
        return std::static_pointer_cast<AbstractSelectionLocalReader>(AbstractLocalReader::shared_from_this());
    }
};

}
}
}
}
}
}
