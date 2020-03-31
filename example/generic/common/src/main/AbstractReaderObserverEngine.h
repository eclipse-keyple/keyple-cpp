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

#include <memory>

/* Core */
#include "ObservableReader_Import.h"
#include "ReaderEvent_Import.h"
#include "AbstractDefaultSelectionsResponse.h"

/* Common */
#include "Logger.h"
#include "LoggerFactory.h"

namespace keyple {
namespace example {
namespace generic {
namespace common {

using namespace keyple::core::seproxy::event;
using namespace keyple::common;

/**
 * This abstract class is intended to be extended by the applications classes in
 * which the SE insertion, selection, removal is factorized here.
 */
class AbstractReaderObserverEngine
: public std::enable_shared_from_this<AbstractReaderObserverEngine>,
  public ObservableReader::ReaderObserver {
public:
    /**
     *
     */
    bool currentlyProcessingSe = false;

    /**
     *
     */
    virtual ~AbstractReaderObserverEngine()
    {
    }

    /**
     *
     */
    virtual void update(std::shared_ptr<ReaderEvent> event);

protected:
    /**
     *
     */
    virtual void
    processSeMatch(std::shared_ptr<AbstractDefaultSelectionsResponse>
                       defaultSelectionsResponse) = 0;

    /**
     * Alternative AID selection
     */
    virtual void processSeInserted() = 0;

    /**
     *
     */
    virtual void processSeRemoval() = 0;

    /**
     *
     */
    virtual void processUnexpectedSeRemoval() = 0;

private:
    /**
     *
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(AbstractReaderObserverEngine));

    /**
     *
     */
    void* runSeInsertedThread(std::shared_ptr<ReaderEvent> event);

    /**
     *
     */
    void* runSeMatchedThread(std::shared_ptr<ReaderEvent> event);
};

}
}
}
}
