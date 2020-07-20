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
#include "ObservableReader.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {

using namespace keyple::core::seproxy::event;

/**
 * The {@link ObservableReaderNotifier} interface provides the API to notify the
 * observers of a {@link ObservableReader}
 */
class ObservableReaderNotifier : public virtual ObservableReader {
    /**
     * Push a ReaderEvent of the {@link ObservableReaderNotifier} to its
     * registered observers.
     *
     * @param event the event (see {@link ReaderEvent})
     */
    virtual void notifyObservers(const std::shared_ptr<ReaderEvent> event) = 0;
};

}
}
}
}
}
