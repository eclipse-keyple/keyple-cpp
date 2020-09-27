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
#include "ReaderPlugin.h"

#include "SamResourceManager.h"
#include "SamResourceManagerDefault.h"
#include "SamResourceManagerPool.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::core::seproxy;

/**
 * Factory that builds a SamResourceManager depending on the plugin used
 */
class SamResourceManagerFactory {
public:

    /**
     * Instantiate a new SamResourceManager.
     * <p>
     * The samReaderPlugin is used to retrieve the available SAM according to
     * the provided filter.
     * <p>
     * Setup a plugin observer if the reader plugin is observable.
     *
     * @param readerPlugin the plugin through which SAM readers are accessible
     * @param samReaderFilter the regular expression defining how to identify
     *        SAM readers among others.
     * @param maxBlockingTime the maximum duration for which the
     *        allocateSamResource method will attempt to allocate a new reader
     *        by retrying (in milliseconds).
     * @param sleepTime the duration to wait between two retries
     * @throw KeypleReaderException throw if an error occurs while getting the
     *        readers list.
     * @return SamResourceManager working with a default plugin
     */
    static std::shared_ptr<SamResourceManager> instantiate(
        const std::shared_ptr<ReaderPlugin> readerPlugin,
        const std::string& samReaderFilter,
        const int maxBlockingTime,
        const int sleepTime);

    /**
     *
     */
    static std::shared_ptr<SamResourceManager> instantiate(
        const std::shared_ptr<ReaderPlugin> readerPlugin,
        const std::string& samReaderFilter);

    /**
     * Instantiate a new SamResourceManager.
     * <p>
     * The samReaderPlugin is used to retrieve the available SAM in the
     * ReaderPoolPlugin.
     * <p>
     * Setup a plugin observer if the reader plugin is observable.
     *
     * @param samReaderPoolPlugin the plugin through which SAM readers are
     *        accessible
     * @param maxBlockingTime the maximum duration for which the
     *        allocateSamResource method will attempt to allocate a new reader
     *        by retrying (in milliseconds).
     * @param sleepTime the duration to wait between two retries
     * @return SamResourceManager working with a pool plugin
     */
    static std::shared_ptr<SamResourceManager> instantiate(
        const ReaderPoolPlugin& samReaderPoolPlugin,
        const int maxBlockingTime,
        const int sleepTime);

    /**
     *
     */
    static std::shared_ptr<SamResourceManager> instantiate(
        const ReaderPoolPlugin& samReaderPoolPlugin);

private:
    /**
     *
     */
    static const int MAX_BLOCKING_TIME = 1000;
    static const int DEFAULT_SLEEP_TIME = 10;
};

}
}
}
