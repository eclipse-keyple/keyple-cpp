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

/* Common */
#include "LoggerFactory.h"

/* Core */
#include "ReaderPlugin.h"

/* Calypso */
#include "SamResourceManager.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;
using namespace keyple::core::seproxy;

/**
 * Implementation of Sam Resource Manager working a {@link ReaderPoolPlugin}
 */
class SamResourceManagerPool final : public SamResourceManager {
public:
    /**
     * {@inheritDoc}
     */
    std::unique_ptr<SeResource<CalypsoSam>> allocateSamResource(
        const AllocationMode allocationMode,
        const SamIdentifier& samIdentifier) override;

    /**
     *
     */
    void freeSamResource(SeResource<CalypsoSam>& samResource) override;

protected:
    /**
     *
     */
    const std::shared_ptr<ReaderPlugin> mSamReaderPlugin;

    /**
     * Protected constructor, use the {@link SamResourceManagerFactory}
     *
     * @param samReaderPoolPlugin the reader pool plugin
     * @param maxBlockingTime the maximum duration for which the
     *        allocateSamResource method will attempt to allocate a new reader
     *        by retrying (in milliseconds).
     * @param sleepTime the duration to wait between two retries
     */
    SamResourceManagerPool(
        std::shared_ptr<ReaderPoolPlugin> samReaderPoolPlugin,
        const int maxBlockingTime,
        const int sleepTime);

private:
    /**
     *
     */
    const std::shared_ptr<Logger> mLogger =
        LoggerFactory::getLogger(typeid(SamResourceManagerPool));

    /**
     *
     */
    const int mMaxBlockingTime;

    /**
     *
     */
    const int mSleepTime;
};

}
}
}
