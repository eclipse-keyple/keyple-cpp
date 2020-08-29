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

#include "SamResourceManagerDefault.h"

namespace keyple {
namespace calypso {
namespace transaction {

/* SAM RESOURCE MANAGER DEFAULT --------------------------------------------- */

SamResourceManagerDefault::SamResourceManagerDefault(
  std::shared_ptr<ReaderPlugin> readerPlugin,
  const std::string& samReaderFilter,
  const int maxBlockingTime,
  const int sleepTime)
: mSleepTime(sleepTime),
  mMaxBlockingTime(maxBlockingTime),
  mSamReaderPlugin(readerPlugin)
{
    /* Assign parameters */
    if (sleepTime < 1)
        throw IllegalArgumentException("Sleep time must be greater than 0");

    if (maxBlockingTime < 1)
        throw IllegalArgumentException(
                  "Max Blocking Time must be greater than 0");

    readerObserver =
        std::make_shared<SamResourceManagerDefault::ReaderObserver>();

    mLogger->info("PLUGINNAME = % initialize the localManagedSamResources " \
                  "with the % connected readers filtered by %\n",
                  mSamReaderPlugin->getName(),
                  mSamReaderPlugin->getReaders().size(),
                  samReaderFilter);

    Pattern* p = Pattern::compile(samReaderFilter);

    const ConcurrentMap<const std::string, std::shared_ptr<SeReader>>& readers =
        mSamReaderPlugin->getReaders();

    for (const auto& samReader : readers) {
        if (p->matcher(samReader.first)->matches()) {
            mLogger->trace("Add reader: %\n", samReader.first);
            try {
                initSamReader(mSamReaderPlugin->getReader(samReader.first),
                              readerObserver);
            } catch (const KeypleReaderException& e) {
                mLogger->error("could not init samReader %\n",
                               samReaderName,
                               e);
            }
        } else {
            mLogger->trace("Reader not matching: %\n", samReaderName);
        }
    }

    if (std::dynamic_pointer_cast<ObservablePlugin>(readerPlugin)) {
        /* Add an observer to monitor reader and SAM insertions */
        std::shared_ptr<SamResourceManagerDefault::PluginObserver>
            pluginObserver =
                std::make_shared<SamResourceManagerDefault::PluginObserver>(
                    readerObserver, samReaderFilter);

        mLogger->trace("Add observer PLUGINNAME = %\n",
                       mSamReaderPlugin->getName());

        mSamReaderPlugin->addObserver(pluginObserver);
    }
}

void SamResourceManagerDefault::removeResource(
    const std::shared_ptr<SeReader> samReader)
{
    ConcurrentMap<const std::string, std::shared_ptr<ManagedSamResource>>
        ::const_iterator it;

    if ((it = mLocalManagedSamResources.find(samReader->getName()) !=
        mLocalManagedSamResources.end()) {
        mLocalManagedSamResources.erase(samReader.getName());

        mLogger->trace("Freed SAM resource: READER = %, SAM_REVISION = %, " \
                       "SAM_SERIAL_NUMBER = %\n",
                       samReader->getName(),
                       *it->getMatchingSe()->getSamRevision(),
                       *it->getMatchingSe()->getSerialNumber()));
        }
    }
}

std::shared_ptr<SeResource<CalypsoSam>>
    SamResourceManagerDefault::allocateSamResource(
        const AllocationMode allocationMode,
        const SamIdentifier& samIdentifier)
{
    const long maxBlockingDate = System::currentTimeMillis() + maxBlockingTime;
    bool noSamResourceLogged = false;

    mLogger->trace("Allocating SAM reader channel...\n");

    while (true) {
        mMutex.lock();
        for (const auto& entry : mLocalManagedSamResources) {
            std::shared_ptr<ManagedSamResource> managedSamResource =
                entry.second;
            if (managedSamResource->isSamResourceFree()) {
                if (managedSamResource->isSamMatching(samIdentifier)) {
                    managedSamResource->setSamResourceStatus(
                        SamResourceStatus::BUSY);
                    mLogger->debug("Allocation succeeded. SAM resource " \
                                   "created\n");
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
                      samIdentifier.getGroupReference());
        } else {
            if (!noSamResourceLogged) {
                /* Log once the first time */
                mLogger->trace("No SAM resources available at the moment\n");
                noSamResourceLogged = true;
            }

            try {
                Thread::sleep(sleepTime);
            } catch (const InterruptedException& e) {
                /* Cet interrupt flag */
                Thread::currentThread()->interrupt();
                mLogger->error("Interrupt exception in Thread::sleep\n");
            }

            if (System::currentTimeMillis() >= maxBlockingDate) {
                mLogger->error(
                    "The allocation process failed. Timeout % sec exceeded\n",
                    (maxBlockingTime / 1000.0));

                std::stringstream << "No Sam resource could be allocated within"
                                  << "timeout of " << maxBlockingTime << "ms "
                                  << "for samIdentifier";
                                  << samIdentifier.getGroupReference());

                throw CalypsoNoSamResourceAvailableException(ss.str());
            }
        }
    }
}

void SamResourceManagerDefault::freeSamResource(
    const std::shared_ptr<SeResource<CalypsoSam>> samResource)
{
    const std::lock_guard<std::mutex> lock(mMutex);

    ConcurrentMap<const std::string, std::shared_ptr<ManagedSamResource>>
        ::const_iterator it;

    if ((it = mLocalManagedSamResources.find(
            samResource->getSeReader()->getName()) !=
            mLocalManagedSamResources.end()) {
        mLogger->trace("Freeing local SAM resource\n");
        *it->setSamResourceStatus(SamResourceStatus::FREE);
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
  const std::string& samReaderFilter) {
: mReaderObserver(readerObserver), mSamReaderFilter(samReaderFilter) {}

void PluginObserver::update(const PluginEvent& event)
{
    for (const auto& readerName : event.getReaderNames()) {
        std::shared_ptr<SeReader> samReader = null;

        mLogger->info("PluginEvent: PLUGINNAME = %, READERNAME = %, EVENTTYPE" \
                      " = %\n",
                      event.getPluginName(),
                      readerName,
                      event.getEventType());

        /* We retrieve the reader object from its name. */
        try {
            samReader = SeProxyService::getInstance()
                           .getPlugin(event.getPluginName())
                           .getReader(readerName);
        } catch (const KeyplePluginNotFoundException& e) {
            mLogger->error("Plugin not found %\n", event.getPluginName());
            return;
        } catch (const KeypleReaderNotFoundException& e) {
            logger.error("Reader not found %\n", readerName);
            return;
        }

        switch (event.getEventType()) {
        case READER_CONNECTED:
            if (mLocalManagedSamResources.find(readerName) !=
                    mLocalManagedSamResources.end()) {
                mLogger->trace("Reader is already present in the local " \
                               "samResources -  READERNAME = %\n",
                               readerName);

                /* Do nothing */
                return;
            }

            mLogger->trace("New reader! READERNAME = %\n", samReader.getName());

            /*
             * We are informed here of a connection of a reader.
             *
             * We add an observer to this reader if possible.
             */
            p = Pattern::compile(samReaderFilter);
            if (p->matcher(readerName)->matches()) {
                /* Enable logging */
                try {
                    initSamReader(samReader, readerObserver);
                } catch (const KeypleReaderException& e) {
                    mLogger->error("Unable to init Sam reader %\n",
                                   samReader->getName(),
                                   e.getCause());
                }
            } else {
                mLogger->trace("Reader not matching: %\n", readerName);
            }
            break;
        case READER_DISCONNECTED:
            /*
             * We are informed here of a disconnection of a reader.
             *
             * The reader object still exists but will be removed from the
             * reader list right after. Thus, we can properly remove the
             * observer attached to this reader before the list update.
             */
            p = Pattern::compile(samReaderFilter);
            if (p->matcher(readerName)->matches()) {
                mLogger->trace("Reader removed. READERNAME = %\n", readerName);

                std::shared_ptr<ObservableReader> obsReader =
                    std::dynamic_pointer_cast<ObservableReader>(samReader);
                if (obsReader) {
                    if (readerObserver != nullptr) {
                        mLogger->trace("Remove observer and stop detection " \
                                       "READERNAME = %\n",
                                       readerName);
                        obsReader->removeObserver(readerObserver);
                        obsReader->stopSeDetection();
                    } else {
                        removeResource(samReader);
                        mLogger->trace("Unplugged reader READERNAME = % " \
                                       "wasn't observed. Resource removed\n",
                                       readerName);
                    }
                }
            } else {
                mLogger->trace("Reader not matching: %\n", readerName);
            }
            break;
        default:
            mLogger->warn("Unexpected reader event. EVENT = %\n",
                          event.getEventType().getName());
            break;
        }
    }
}

/* READER OBSERVER ---------------------------------------------------------- */

ReaderObserver::ReaderObserver()
: ObservableReader::ReaderObserver() {}

void update(const ReaderEvent& event) const
{
    /* TODO revise exception management */
    std::shared_ptr<SeReader> samReader = nullptr;

    try {
        samReader = mSamReaderPlugin->getReader(event.getReaderName());
    } catch (const KeypleReaderNotFoundException& e) {
        mLogger->error("KeypleReaderNotFoundException raised, %\n", e);
    }

    const std::lock_guard<std::mutex> lock(mMutex);

    switch (event.getEventType()) {
    case SE_MATCHED:
    case SE_INSERTED:
        if (mLocalManagedSamResources.find(samReader->getName()) !=
                mLocalManagedSamResources.end()) {
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
            newSamResource = createSamResource(samReader);
        } catch (const CalypsoNoSamResourceAvailableException& e) {
            mLogger->error("Failed to create a SeResource<CalypsoSam> from %\n",
                           samReader->getName());
        }

        /* Failures are ignored */
        if (newSamResource != nullptr) {
            mLogger->trace("Created SAM resource: READER = %, SAM_REVISION = " \
                           "%, SAM_SERIAL_NUMBER = %\n",
                           event.getReaderName(),
                           newSamResource->getMatchingSe()->getSamRevision(),
                           newSamResource->getMatchingSe()->getSerialNumber()));

            mLocalManagedSamResources.insert({samReader->getName(),
                                              newSamResource});
        }
        break;
    case SE_REMOVED:
    case TIMEOUT_ERROR:
        removeResource(samReader);
        break;
    }
}

/* MANAGED SAM RESOURCE ----------------------------------------------------- */

ManagedSamResource::ManagedSamResource(
  std::shared_ptr<SeReader> seReader,
  std::shared_ptr<CalypsoSam> calypsoSam)
: SeResource<CalypsoSam>(seReader, calypsoSam),
  mSamResourceStatus(SamResourceStatus::FREE),
  mSamIdentifier(nullptr) {}

const bool ManagedSamResource::isSamResourceFree() const
{
    return mSamResourceStatus == SamResourceStatus::FREE;
}

void ManagedSamResource::setSamIdentifier(
    std::shared_ptr<SamIdentifier> samIdentifier)
{
    mSamIdentifier = samIdentifier;
}

const bool ManagedSamResource::isSamMatching(
    const std::shared_ptr<SamIdentifier> samIdentifier) const
{
    return samIdentifier->matches(mSamIdentifier);
}

void ManagedSamResource::setSamResourceStatus(
    const SamResourceStatus& samResourceStatus)
{
    mSamResourceStatus = samResourceStatus;
}

}
}
}
