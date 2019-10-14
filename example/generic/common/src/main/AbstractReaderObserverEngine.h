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

#include <memory>

/* Core */
#include "ObservableReader_Import.h"
#include "ReaderEvent_Import.h"
#include "AbstractDefaultSelectionsResponse.h"

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

namespace org {
namespace eclipse {
namespace keyple {
namespace example {
namespace generic {
namespace common {

using namespace org::eclipse::keyple::core::seproxy::event;
using namespace org::eclipse::keyple::common;

/**
 * This abstract class is intended to be extended by the applications classes in which the SE
 * insertion, selection, removal is factorized here.
 */
class AbstractReaderObserverEngine
: public std::enable_shared_from_this<AbstractReaderObserverEngine>, public ObservableReader::ReaderObserver {

private:
    const std::shared_ptr<Logger> logger = LoggerFactory::getLogger(typeid(AbstractReaderObserverEngine));

protected:
    virtual void processSeMatch(std::shared_ptr<AbstractDefaultSelectionsResponse> defaultSelectionsResponse) = 0;

    virtual void processSeInsertion() = 0; // alternative AID selection

    virtual void processSeRemoval() = 0;

    virtual void processUnexpectedSeRemoval() = 0;

  public:
    bool currentlyProcessingSe = false;

    virtual void update(std::shared_ptr<ReaderEvent> event);
};

} // namespace common
}     // namespace generic
}         // namespace example
}             // namespace keyple
}                 // namespace eclipse
} // namespace org
