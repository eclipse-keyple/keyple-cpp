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

#pragma once

#include <atomic>

/* Core */
#include "ObservableReader.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

using namespace keyple::core::seproxy::event;

/**
 * The keyple::core::seproxy::plugin::ObservableReaderNotifier interface provides the API to notify
 * the observers of a keyple::core::seproxy::event::ObservableReader
 */
class ObservableReaderNotifier : public virtual ObservableReader {
public:
    /**
     *
     */
    ObservableReaderNotifier() : mShuttingDown(false) {}

    /**
     *
     */
    virtual ~ObservableReaderNotifier()
    {
        mShuttingDown = true;
    }

    /**
     * Push a ReaderEvent of the keyple::core::seproxy::plugin::ObservableReaderNotifier to its
     * registered observers.
     *
     * @param event the event (see keyple::core::seproxy::event::ReaderEvent)
     */
    virtual void notifyObservers(const std::shared_ptr<ReaderEvent> event) = 0;

    /**
     * C++ vs. Java: added this flag to handle threads gracefully
     */
    std::atomic<bool> mShuttingDown;
};

}
}
}
}
