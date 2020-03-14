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

/* Common */
#include "Export.h"
#include "LoggerFactory.h"

/* Core */
#include "AbstractObservableLocalReader.h"
#include "AbstractObservableState.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {
namespace state {

/**
 * Wait for Se Removal State
 * <p>
 * The state in which the SE is still present and awaiting removal.
 * <ul>
 * <li>Upon SE_REMOVED event, the machine changes state for
 * WAIT_FOR_SE_INSERTION or WAIT_FOR_SE_DETECTION according to the {@link
 * ObservableReader.PollingMode} setting.
 * <li>Upon STOP_DETECT event, the machine changes state for
 * WAIT_FOR_SE_DETECTION.
 * </ul>
 */
class EXPORT WaitForSeRemoval : public AbstractObservableState {
public:
    /**
     *
     */
    WaitForSeRemoval(AbstractObservableLocalReader* reader);

    /**
     *
     */
    WaitForSeRemoval(AbstractObservableLocalReader* reader,
                     std::shared_ptr<MonitoringJob> monitoringJob,
                     std::shared_ptr<MonitoringPool> executorService);

    /**
     *
     */
    virtual ~WaitForSeRemoval() = default;

    /**
     *
     */
    void onEvent(const InternalEvent event) override;

private:
    /**
     * Logger
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(WaitForSeRemoval));
};

}
}
}
}
}
}
