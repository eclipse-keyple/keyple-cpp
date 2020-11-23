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

#include "SamResourceManagerDefault.h"

/* Calypso */
#include "CalypsoNoSamResourceAvailableException.h"
#include "CalypsoSam.h"

/* Common */
#include "ConcurrentMap.h"
#include "IllegalArgumentException.h"
#include "InterruptedException.h"
#include "KeypleStd.h"
#include "System.h"
#include "Thread.h"

/* Core */
#include "KeyplePluginNotFoundException.h"
#include "ObservableReader.h"
#include "SeCommonProtocols.h"
#include "SeProxyService.h"
#include "SeResource.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::calypso::exception;
using namespace keyple::common::exception;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::exception;

using PluginObserver = SamResourceManagerDefault::PluginObserver;
using PollingMode = ObservableReader::PollingMode;
using ReaderObserver = SamResourceManagerDefault::ReaderObserver;
using SamResourceStatus = SamResourceManager::ManagedSamResource::SamResourceStatus;

/* SAM RESOURCE MANAGER DEFAULT --------------------------------------------- */

SamResourceManagerDefault::SamResourceManagerDefault(std::shared_ptr<ReaderPlugin> readerPlugin,
                                                     const std::string& samReaderFilter,
                                                     const int maxBlockingTime,
                                                     const int sleepTime)
: mSamReaderPlugin(readerPlugin), mMaxBlockingTime(maxBlockingTime), mSleepTime(sleepTime)
{
    /* Assign parameters */
    if (sleepTime < 1)
        throw IllegalArgumentException("Sleep time must be greater than 0");

    if (maxBlockingTime < 1)
        throw IllegalArgumentException("Max Blocking Time must be greater than 0");

    mReaderObserver = std::make_shared<ReaderObserver>(*this);

    mLogger->info("PLUGINNAME = % initialize the localManagedSamResources " \
                  "with the % connected readers filtered by %\n",
                  mSamReaderPlugin->getName(),
                  mSamReaderPlugin->getReaders().size(),
                  samReaderFilter);

    std::unique_ptr<Pattern> p = Pattern::compile(samReaderFilter);

    const ConcurrentMap<const std::string, std::shared_ptr<SeReader>>& readers =
        mSamReaderPlugin->getReaders();

    for (const auto& samReader : readers) {
        if (p->matcher(samReader.first)->matches()) {
            mLogger->trace("Add reader: %\n", samReader.first);
            try {
                initSamReader(mSamReaderPlugin->getReader(samReader.first), mReaderObserver);
            } catch (const KeypleReaderException& e) {
                mLogger->error("could not init samReader %\n", samReader.second, e);
            }
        } else {
            mLogger->trace("Reader not matching: %\n", samReader.second);
        }
    }

    if (std::dynamic_pointer_cast<ObservablePlugin>(readerPlugin)) {
        /* Add an observer to monitor reader and SAM insertions */
        auto pluginObserver = std::make_shared<PluginObserver>(mReaderObserver,
                                                               samReaderFilter,
                                                               *this);

        mLogger->trace("Add observer PLUGINNAME = %\n", mSamReaderPlugin->getName());

        std::dynamic_pointer_cast<ObservablePlugin>(mSamReaderPlugin)->addObserver(pluginObserver);
    }
}

void SamResourceManagerDefault::removeResource(const std::shared_ptr<SeReader> samReader)
{
    const auto it = mLocalManagedSamResources.find(samReader->getName());

    if (it != mLocalManagedSamResources.end()) {
        mLocalManagedSamResources.erase(samReader->getName());

        mLogger->trace("Freed SAM resource: READER = %, SAM_REVISION = %, " \
                       "SAM_SERIAL_NUMBER = %\n",
                       samReader->getName(),
                       it->second->getMatchingSe()->getSamRevision(),
                       it->second->getMatchingSe()->getSerialNumber());
    }
}

std::shared_ptr<SeResource<CalypsoSam>>
    SamResourceManagerDefault::allocateSamResource(
        const AllocationMode allocationMode,
        const std::shared_ptr<SamIdentifier> samIdentifier)
{
    const uint64_t maxBlockingDate = System::currentTimeMillis() + mMaxBlockingTime;
    bool noSamResourceLogged = false;

    mLogger->trace("Allocating SAM reader channel...\n");

    while (true) {
        mMutex.lock();
        for (const auto& entry : mLocalManagedSamResources) {
            std::shared_ptr<ManagedSamResource> managedSamResource = entry.second;
            if (managedSamResource->isSamResourceFree()) {
                if (managedSamResource->isSamMatching(samIdentifier)) {
                    managedSamResource->setSamResourceStatus(SamResourceStatus::BUSY);
                    mLogger->debug("Allocation succeeded. SAM resource created\n");
                    mMutex.unlock();
                    return managedSamResource;
                }
            }
        }
        mMutex.unlock();

        /*
         * Loop until MAX_BLOCKING_TIME in blocking mode, only once in
         * non-blocking mode
         */
        if (allocationMode == AllocationMode::NON_BLOCKING) {
            mLogger->trace("No SAM resources available at the moment\n");
            throw CalypsoNoSamResourceAvailableException(
                      "No Sam resource could be allocated for samIdentifier " +
                      samIdentifier->getGroupReference());
        } else {
            if (!noSamResourceLogged) {
                /* Log once the first time */
                mLogger->trace("No SAM resources available at the moment\n");
                noSamResourceLogged = true;
            }

            try {
                Thread::sleep(mSleepTime);
            } catch (const InterruptedException& e) {
                /* Set interrupt flag */
                /*
                 * C++ vs. Java: cannot interrupt thread as easily as in Java,
                 *               as this scenario is quite unlikely to happen,
                 *               will throw instead for now.
                 */
                //Thread::currentThread()->interrupt();
                mLogger->error("Interrupt exception in Thread::sleep\n");
                throw e;
            }

            if (System::currentTimeMillis() >=
                    static_cast<unsigned long long>(maxBlockingDate)) {
                mLogger->error(
                    "The allocation process failed. Timeout % sec exceeded\n",
                    (mMaxBlockingTime / 1000.0));

                std::stringstream ss;
                ss << "No Sam resource could be allocated within"
                   << "timeout of " << mMaxBlockingTime << "ms "
                   << "for samIdentifier "<< samIdentifier->getGroupReference();

                throw CalypsoNoSamResourceAvailableException(ss.str());
            }
        }
    }
}

void SamResourceManagerDefault::freeSamResource(
    const SeResource<CalypsoSam>& samResource)
{
    const std::lock_guard<std::mutex> lock(mMutex);

    const auto it = mLocalManagedSamResources.find(
                        samResource.getSeReader()->getName());

    if (it != mLocalManagedSamResources.end()) {
        mLogger->trace("Freeing local SAM resource\n");
        it->second->setSamResourceStatus(SamResourceStatus::FREE);
    } else {
        mLogger->error("SAM resource not found while freeing\n");
    }
}

void SamResourceManagerDefault::initSamReader(
    const std::shared_ptr<SeReader> samReader,
    const std::shared_ptr<ReaderObserver> readerObserver)
{
    samReader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO7816_3,".*");

    /* Specific to PCSC reader (no effect on Stub) */

    try {
        /* Contactless SE works with T0 protocol */
        samReader->setParameter("protocol", "T0");

        /* Shared mode */
        samReader->setParameter("mode", "shared");
        if (samReader->isSePresent()) {
            mLogger->trace("Create SAM resource: %\n", samReader->getName());

            mMutex.lock();
            mLocalManagedSamResources.insert({samReader->getName(),
                                              createSamResource(samReader)});
            mMutex.unlock();
        }
    } catch (const KeypleException& e) {
        (void)e;
        throw IllegalArgumentException("Parameters are not supported for this" \
                                       " reader : protocol:TO, mode:shared");
    }

    std::shared_ptr<ObservableReader> obsReader =
        std::dynamic_pointer_cast<ObservableReader>(samReader);

    if (obsReader && readerObserver != nullptr) {
        mLogger->trace("Add observer and start detection READERNAME = %\n",
                       samReader->getName());

        obsReader->addObserver(readerObserver);
        obsReader->startSeDetection(PollingMode::REPEATING);
    } else {
        mLogger->trace("Sam Reader is not an ObservableReader = %\n",
                       samReader->getName());
    }
}

/* PLUGIN OBSERVER ---------------------------------------------------------- */

PluginObserver::PluginObserver(
  const std::shared_ptr<ReaderObserver> readerObserver,
  const std::string& samReaderFilter,
  SamResourceManagerDefault& parent)
: mReaderObserver(readerObserver),
  mSamReaderFilter(samReaderFilter),
  mParent(parent) {}

void PluginObserver::update(const std::shared_ptr<PluginEvent> event)
{
    for (const auto& readerName : event->getReaderNames()) {
        std::shared_ptr<SeReader> samReader = nullptr;

        mLogger->info("PluginEvent: PLUGINNAME = %, READERNAME = %, EVENTTYPE" \
                      " = %\n",
                      event->getPluginName(),
                      readerName,
                      event->getEventType());

        /* We retrieve the reader object from its name. */
        try {
            samReader = SeProxyService::getInstance()
                           .getPlugin(event->getPluginName())
                           ->getReader(readerName);
        } catch (const KeyplePluginNotFoundException& e) {
            (void)e;
            mLogger->error("Plugin not found %\n", event->getPluginName());
            return;
        } catch (const KeypleReaderNotFoundException& e) {
            (void)e;
            mLogger->error("Reader not found %\n", readerName);
            return;
        }

        const PluginEvent::EventType& type = event->getEventType();
        if (type == PluginEvent::EventType::READER_CONNECTED) {
            if (mParent.mLocalManagedSamResources.find(readerName) !=
                    mParent.mLocalManagedSamResources.end()) {
                mLogger->trace("Reader is already present in the local " \
                               "samResources -  READERNAME = %\n",
                               readerName);

                /* Do nothing */
                return;
            }

            mLogger->trace("New reader! READERNAME = %\n", samReader->getName());

            /*
             * We are informed here of a connection of a reader.
             *
             * We add an observer to this reader if possible.
             */
            mPattern = Pattern::compile(mSamReaderFilter);
            if (mPattern->matcher(readerName)->matches()) {
                /* Enable logging */
                try {
                    mParent.initSamReader(samReader, mReaderObserver);
                } catch (const KeypleReaderException& e) {
                    mLogger->error("Unable to init Sam reader %\n",
                                   samReader->getName(),
                                   e.getCause().what());
                }
            } else {
                mLogger->trace("Reader not matching: %\n", readerName);
            }

        } else if (type == PluginEvent::EventType::READER_DISCONNECTED) {
            /*
             * We are informed here of a disconnection of a reader.
             *
             * The reader object still exists but will be removed from the
             * reader list right after. Thus, we can properly remove the
             * observer attached to this reader before the list update.
             */
            mPattern = Pattern::compile(mSamReaderFilter);
            if (mPattern->matcher(readerName)->matches()) {
                mLogger->trace("Reader removed. READERNAME = %\n", readerName);

                std::shared_ptr<ObservableReader> obsReader =
                    std::dynamic_pointer_cast<ObservableReader>(samReader);
                if (obsReader) {
                    if (mReaderObserver != nullptr) {
                        mLogger->trace("Remove observer and stop detection " \
                                       "READERNAME = %\n",
                                       readerName);
                        obsReader->removeObserver(mReaderObserver);
                        obsReader->stopSeDetection();
                    } else {
                        mParent.removeResource(samReader);
                        mLogger->trace("Unplugged reader READERNAME = % " \
                                       "wasn't observed. Resource removed\n",
                                       readerName);
                    }
                }
            } else {
                mLogger->trace("Reader not matching: %\n", readerName);
            }

        } else {
            mLogger->warn("Unexpected reader event-> EVENT = %\n",
                          event->getEventType().getName());
        }
    }
}

/* READER OBSERVER ---------------------------------------------------------- */

ReaderObserver::ReaderObserver(SamResourceManagerDefault& parent)
: ObservableReader::ReaderObserver(), mParent(parent) {}

void ReaderObserver::update(const std::shared_ptr<ReaderEvent> event)
{
    /* TODO revise exception management */
    std::shared_ptr<SeReader> samReader = nullptr;

    try {
        samReader = mParent.mSamReaderPlugin->getReader(event->getReaderName());
    } catch (const KeypleReaderNotFoundException& e) {
        mLogger->error("KeypleReaderNotFoundException raised, %\n", e);
    }

    const std::lock_guard<std::mutex> lock(mParent.mMutex);

    const ReaderEvent::EventType& type = event->getEventType();
    if (type == ReaderEvent::EventType::SE_MATCHED ||
        type == ReaderEvent::EventType::SE_INSERTED) {
        if (mParent.mLocalManagedSamResources.find(samReader->getName()) !=
                mParent.mLocalManagedSamResources.end()) {
            mLogger->trace("Reader is already present in the local " \
                           "samResources -  READERNAME = %\n",
                           samReader->getName());

            /* Do nothing */
            return;
        }

        std::shared_ptr<ManagedSamResource> newSamResource = nullptr;

        try {
            /*
             * Although the reader allocation is dynamic, the SAM resource type
             * is STATIC
             */
            newSamResource = mParent.createSamResource(samReader);
        } catch (const CalypsoNoSamResourceAvailableException& e) {
            (void)e;
            mLogger->error("Failed to create a SeResource<CalypsoSam> from %\n",
                           samReader->getName());
        }

        /* Failures are ignored */
        if (newSamResource != nullptr) {
            mLogger->trace("Created SAM resource: READER = %, SAM_REVISION = " \
                           "%, SAM_SERIAL_NUMBER = %\n",
                           event->getReaderName(),
                           newSamResource->getMatchingSe()->getSamRevision(),
                           newSamResource->getMatchingSe()->getSerialNumber());

            mParent.mLocalManagedSamResources.insert({samReader->getName(),
                                                      newSamResource});
        }

    } else if (type == ReaderEvent::EventType::SE_REMOVED ||
               type == ReaderEvent::EventType::TIMEOUT_ERROR) {
        mParent.removeResource(samReader);
    }
}

}
}
}
