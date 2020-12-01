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

#include "SamResourceManagerPool.h"

/* Calypso */
#include "CalypsoNoSamResourceAvailableException.h"

/* Common */
#include "IllegalArgumentException.h"
#include "InterruptedException.h"
#include "System.h"
#include "Thread.h"

/* Core */
#include "KeypleAllocationReaderException.h"
#include "KeypleAllocationNoReaderException.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::exception;
using namespace keyple::common::exception;
using namespace keyple::core::seproxy::exception;

SamResourceManagerPool::SamResourceManagerPool(
  ReaderPoolPlugin& samReaderPoolPlugin,
  const int maxBlockingTime,
  const int sleepTime)
: mSamReaderPlugin(samReaderPoolPlugin),
  mMaxBlockingTime(maxBlockingTime),
  mSleepTime(sleepTime)
{
    if (sleepTime < 1)
        throw IllegalArgumentException("Sleep time must be greater than 0");

    if (maxBlockingTime < 1)
        throw IllegalArgumentException("Max Blocking Time must be greater than 0");

    mLogger->info("Create SAM resource manager from reader pool plugin: %\n",
                  mSamReaderPlugin.getName());

    /* HSM reader plugin type */
}

std::shared_ptr<SeResource<CalypsoSam>> SamResourceManagerPool::allocateSamResource(
    const AllocationMode allocationMode,
    const std::shared_ptr<SamIdentifier> samIdentifier)
{
    const uint64_t maxBlockingDate = System::currentTimeMillis() + mMaxBlockingTime;
    bool noSamResourceLogged = false;

    mLogger->debug("Allocating SAM reader channel...\n");

    while (true) {
        try {
            /* virtually infinite number of readers */
            std::shared_ptr<SeReader> samReader = mSamReaderPlugin.allocateReader(
                                                      samIdentifier->getGroupReference());

            if (samReader != nullptr) {
                std::shared_ptr<ManagedSamResource> managedSamResource = createSamResource(samReader);
                mLogger->debug("Allocation succeeded. SAM resource created\n");
                return managedSamResource;
            }

            /*
             * Loop until MAX_BLOCKING_TIME in blocking mode, only once in
             * non-blocking mode
             */
            if (allocationMode == AllocationMode::NON_BLOCKING) {
                mLogger->trace("No SAM resources available at the moment\n");
                throw CalypsoNoSamResourceAvailableException(
                          "No Sam resource could be allocated for " \
                          "samIdentifier " +
                          samIdentifier->getGroupReference());
            } else {
                if (!noSamResourceLogged) {
                    /* log once the first time */
                    mLogger->trace("No SAM resources available at the moment\n");
                    noSamResourceLogged = true;
                }

                try {
                    Thread::sleep(mSleepTime);
                } catch (const InterruptedException& e) {
                    /*
                     * C++ vs. Java: Cannot interrupt thread easily. This
                     *               scenario is unlikely to happen so throw
                     *               for now.
                     */
                    (void)e;
                    //Thread::currentThread()->interrupt();
                    mLogger->error("Interrupt exception in Thread::sleep\n");
                    throw;
                }

                if (System::currentTimeMillis() >=
                        static_cast<unsigned long long>(maxBlockingDate)) {
                    mLogger->error("The allocation process failed. Timeout % " \
                                   "sec exceeded\n",
                                   (mMaxBlockingTime / 1000.0));
                    std::stringstream ss;
                    ss << "No Sam resource could be allocated"
                       << "within " << mMaxBlockingTime
                       << "ms for samIdentifier "
                       << samIdentifier->getGroupReference();
                    throw CalypsoNoSamResourceAvailableException(ss.str());
                }
            }

        } catch (const KeypleAllocationReaderException& e) {
            throw KeypleAllocationReaderException(
                      "Allocation failed due to a plugin technical error", e);
        } catch (const KeypleAllocationNoReaderException& e) {
            /* No reader is available, let's retry */
            (void)e;
        }
    }
}

void SamResourceManagerPool::freeSamResource(
    const SeResource<CalypsoSam>& samResource)
{
    /* Virtually infinite number of readers */
    mLogger->debug("Freeing HSM SAM resource\n");
    dynamic_cast<ReaderPoolPlugin&>(mSamReaderPlugin)
        .releaseReader(samResource.getSeReader());

}

}
}
}
