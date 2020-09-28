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
#include "ReaderPoolPlugin.h"

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
    std::shared_ptr<SeResource<CalypsoSam>> allocateSamResource(
        const AllocationMode allocationMode,
        const std::shared_ptr<SamIdentifier> samIdentifier) override;

    /**
     *
     */
    void freeSamResource(const SeResource<CalypsoSam>& samResource) override;

    /**
     * Protected constructor, use the {@link SamResourceManagerFactory}
     *
     * C++ vs. Java: Should be private but would forbid usage of make_shared
     *               from SamResourceManagerFactory class. Setting it
     *               public for now. Could use an intermediate derived class
     *               otherwise if need be.
     *
     * @param samReaderPoolPlugin the reader pool plugin
     * @param maxBlockingTime the maximum duration for which the
     *        allocateSamResource method will attempt to allocate a new reader
     *        by retrying (in milliseconds).
     * @param sleepTime the duration to wait between two retries
     */
    SamResourceManagerPool(
        ReaderPoolPlugin& samReaderPoolPlugin,
        const int maxBlockingTime,
        const int sleepTime);

protected:
    /**
     *
     */
    ReaderPoolPlugin& mSamReaderPlugin;

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
