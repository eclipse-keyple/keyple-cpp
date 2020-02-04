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

/* Common */
#include "InterruptedException.h"
#include "Pattern.h"
#include "System.h"
#include "Thread.h"

/* Core */
#include "ByteArrayUtil.h"
#include "SeReader.h"
#include "SeCommonProtocols.h"
#include "SeSelection.h"

/* Calypso */
#include "SamResourceManager.h"
#include "SamSelector.h"
#include "SamRevision_Import.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;
using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;
using namespace keyple::core::seproxy::protocol;
using namespace keyple::core::util;

using ReaderObserver = ObservableReader::ReaderObserver;
using PluginObserver = ObservablePlugin::PluginObserver;

const int SamResourceManager::MAX_BLOCKING_TIME = 1000; // 10 sec

SamResourceManager::SamResourceManager(
  std::shared_ptr<ReaderPlugin> samReaderPlugin,
  const std::string& samReaderFilter)
: samReaderPlugin(samReaderPlugin)
{
    if (std::dynamic_pointer_cast<ReaderPoolPlugin>(samReaderPlugin)) {
        logger->info("Create SAM resource manager from reader pool plugin: " \
                     "%s\n", samReaderPlugin->getName());

        /* HSM reader plugin type */
        dynamicAllocationPlugin = true;
    } else {
        logger->info("Create SAM resource manager from reader plugin: %s\n",
                     samReaderPlugin->getName());

        /* Local readers plugin type */
        dynamicAllocationPlugin = false;

        if (std::dynamic_pointer_cast<ObservablePlugin>(samReaderPlugin)) {
            /* Add an observer to monitor reader and SAM insertions */
            std::shared_ptr<ReaderObserver> readerObserver =
                std::make_shared<ReaderObserver>();

            std::shared_ptr<PluginObserver> pluginObserver =
                std::make_shared<PluginObserver>(readerObserver,
                                                 samReaderFilter);

            logger->info("Add observer PLUGINNAME = %s\n",
                         samReaderPlugin->getName());

            std::dynamic_pointer_cast<ObservablePlugin>(samReaderPlugin)
                ->addObserver(pluginObserver);
        } else {
            /*
             * The plugin isn't observable, just add resources from the current
             * readers if any
             */
            logger->info("PLUGINNAME = %s isn't observable. Add available " \
                         "readers\n", samReaderPlugin->getName());

            std::shared_ptr<std::set<std::shared_ptr<SeReader>>> samReaders =
                samReaderPlugin->getReaders();

            for (std::shared_ptr<SeReader> samReader : *samReaders) {
                std::string readerName = samReader->getName();
                Pattern* p = Pattern::compile(samReaderFilter);
                if (p->matcher(readerName).matches()) {
                    logger->debug("Add reader: %s\n", readerName);
                    localSamResources.insert(createSamResource(samReader));
                } else {
                    logger->debug("Reader not matching: %s\n", readerName);
                }
            }
        }
    }
}

std::unique_ptr<SamResource> SamResourceManager::createSamResource(
    std::shared_ptr<SeReader> samReader)
{
    logger->trace("Create SAM resource from reader NAME = %s\n",
                  samReader->getName());

    samReader->addSeProtocolSetting(SeCommonProtocols::PROTOCOL_ISO7816_3,".*");

    SamIdentifier samIdentifier(SamRevision::AUTO, "", "");
    std::shared_ptr<SamSelector> samSelector =
        std::make_shared<SamSelector>(samIdentifier, "SAM");

    /* Prepare selector, ignore MatchingSe here */
    SeSelection samSelection;
    samSelection.prepareSelection(
        std::make_shared<SamSelectionRequest>(samSelector));

    SelectionsResult selectionsResult =
         samSelection.processExplicitSelection(samReader);

    if (!selectionsResult.hasActiveSelection()) {
        throw IllegalStateException("Unable to open a logical channel for SAM");
    }

    std::shared_ptr<CalypsoSam> calypsoSam =
        std::dynamic_pointer_cast<CalypsoSam>(
            selectionsResult.getActiveSelection().getMatchingSe());

    return std::make_unique<SamResource>(samReader, calypsoSam);
}

std::unique_ptr<SamResource> SamResourceManager::allocateSamResource(
    const AllocationMode allocationMode, const SamIdentifier& samIdentifier)
{
    long maxBlockingDate = System::currentTimeMillis() + MAX_BLOCKING_TIME;
    bool noSamResourceLogged = false;

    logger->debug("Allocating SAM reader channel...\n");

    while (true) {
        if (dynamicAllocationPlugin) {
            // virtually infinite number of readers
            std::shared_ptr<SeReader> samReader =
                (std::dynamic_pointer_cast<ReaderPoolPlugin>(samReaderPlugin))
                    ->allocateReader(samIdentifier.getGroupReference());
            if (samReader != nullptr) {
                std::unique_ptr<SamResource> samResource =
                    createSamResource(samReader);
                logger->debug("Allocation succeeded. SAM resource created\n");
                return samResource;
            }
        } else {
            std::unique_lock<std::mutex> lock(this->mtx);
            for (std::unique_ptr<SamResource> samResource : localSamResources) {
                if (samResource->isSamResourceFree()) {
                    if (samResource->isSamMatching(samIdentifier)) {
                        samResource->setSamResourceStatus(
                            SamResource::SamResourceStatus::BUSY);
                        logger->debug("Allocation succeeded. SAM resource " \
                                      "created.");
                        std::unique_lock<std::mutex> unlock(this->mtx);
                        return samResource;
                    }
                }
            }
            std::unique_lock<std::mutex> unlock(this->mtx);
        }

        /*
         * Loop until MAX_BLOCKING_TIME in blocking mode, only once in
         * non-blocking mode
         */
        if (allocationMode == AllocationMode::NON_BLOCKING) {
            logger->trace("No SAM resources available at the moment\n");
            break;
        } else {
            if (!noSamResourceLogged) {
                /* log once the first time */
                logger->trace("No SAM resources available at the moment\n");
                noSamResourceLogged = true;
            }

            try {
                Thread::sleep(10);
            } catch (InterruptedException e) {
                Thread::currentThread().interrupt(); // set interrupt flag
                logger->error("Interrupt exception in Thread::sleep\n");
            }
            if (System::currentTimeMillis() >= maxBlockingDate) {
                logger->error("The allocation process failed. Timeout %d sec " \
                              "exceeded\n", (MAX_BLOCKING_TIME / 100.0));
                return nullptr;
            }
        }
    }

    return nullptr;
}

void SamResourceManager::freeSamResource(
    std::unique_ptr<SamResource> samResource)
{
    if (dynamicAllocationPlugin) {
        /* Virtually infinite number of readers */
        logger->debug("Freeing HSM SAM resource\n");
        (std::dynamic_pointer_cast<ReaderPoolPlugin>(samReaderPlugin))
            ->releaseReader(samResource->getSeReader());
    } else {
        std::unique_lock<std::mutex> lock(this->mtx);
        logger->debug("Freeing local SAM resource\n");
        samResource.setSamResourceStatus(SamResource::SamResourceStatus::FREE);
        std::unique_lock<std::mutex> unlock(this->mtx);
    }
}

void SamResourceManager::removeResource(std::shared_ptr<SeReader> samReader)
{
    std::list<std::unique_ptr<SamResource>>::iterator samResource =
        localSamResources.begin();

    while (samResource != localSamResources.end()) {
        if ((*samResource)->getSeReader() == samReader) {
            logger->info("Freed SAM resource: READER = %s, SAM_REVISION = %s," \
                         " SAM_SERIAL_NUMBER = %s", samReader->getName(),
                         (*samResource)->getMatchingSe()->getSamRevision(),
                         ByteArrayUtil::toHex((*samResource)->getMatchingSe()
                                                  ->getSerialNumber()));
            localSamResources.erase(*samResource);
        }
    }
}

}
}
}