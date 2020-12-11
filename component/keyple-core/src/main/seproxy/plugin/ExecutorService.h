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

#include "AbstractMonitoringJob.h"

#include <future>
#include <typeinfo>
#include <vector>

/* Core */
#include "KeypleCoreExport.h"

/* Common */
#include "LoggerFactory.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace plugin {

class AbstractObservableState;

using namespace keyple::common;

class KEYPLECORE_API ExecutorService final {
public:
    /**
     *
     */
    ExecutorService();

    /**
     *
     */
    ~ExecutorService();

    /**
     *
     */
    std::future<void>* submit(std::shared_ptr<AbstractMonitoringJob> monitoringJob,
                              AbstractObservableState* state,
                              std::atomic<bool>& cancellationFlag);

    /**
     * /!\ MSVC requires operator= to be deleted because of std::future
     * not being copyable.
     */
    ExecutorService& operator=(ExecutorService o) = delete;

    /**
     * /!\ MSVC requires copy constructor to be deleted because of std::future
     * not being copyable.
     */
    ExecutorService(const ExecutorService& o) = delete;

private:
    /**
     *
     */
    std::vector<std::future<void>> mPool;

    /**
     *
     */
    std::thread* mThread;

    /**
     *
     */
    void run();

    /**
     *
     */
    std::atomic<bool> mRunning;

    /**
     *
     */
    std::atomic<bool> mTerminated;
};

}
}
}
}
