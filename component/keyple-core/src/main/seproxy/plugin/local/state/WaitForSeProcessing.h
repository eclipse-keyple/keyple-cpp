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
#include "AbstractObservableState.h"
#include "AbstractObservableLocalReader.h"
#include "MonitoringJob.h"
#include "MonitoringPool.h"

/* Common */
#include "Export.h"
#include "LoggerFactory.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {
namespace local {
namespace state {

using namespace keyple::core::seproxy::plugin::local;

/**
 * Wait for Se Processing State
 * <p>
 * The state during which the SE is being processed by the application.
 * <ul>
 * <li>Upon SE_PROCESSED event, the machine changes state for
 * WAIT_FOR_SE_REMOVAL or WAIT_FOR_SE_DETECTION according to the {@link
 * ObservableReader.PollingMode} setting.
 * <li>Upon SE_REMOVED event, the machine changes state for
 * WAIT_FOR_SE_INSERTION or WAIT_FOR_SE_DETECTION according to the {@link
 * ObservableReader.PollingMode} setting.
 * <li>Upon STOP_DETECT event, the machine changes state for
 * WAIT_FOR_SE_DETECTION.
 * </ul>
 */
class EXPORT WaitForSeProcessing : public AbstractObservableState {
public:
    /**
     *
     */
    WaitForSeProcessing(AbstractObservableLocalReader* reader);

    /**
     *
     */
    WaitForSeProcessing(AbstractObservableLocalReader* reader,
                        std::shared_ptr<MonitoringJob> monitoringJob,
                        std::shared_ptr<MonitoringPool> executorService);

    /**
     *
     */
    virtual ~WaitForSeProcessing() = default;

    /**
     *
     */
    void onEvent(const InternalEvent event) override;

private:
    /**
     * Logger
     */
    const std::shared_ptr<Logger> logger =
        LoggerFactory::getLogger(typeid(WaitForSeProcessing));
};

}
}
}
}
}
}
