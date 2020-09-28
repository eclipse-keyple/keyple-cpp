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

#include <mutex>
#include <typeinfo>

/* Calypso */
#include "SamResourceManager.h"

/* Common */
#include "LoggerFactory.h"
#include "Pattern.h"

/* Core */
#include "ObservablePlugin.h"
#include "ObservableReader.h"
#include "PluginEvent.h"
#include "ReaderEvent.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;
using namespace keyple::core::seproxy::event;

/**
 * Implementation of Sam Resource Manager working a {@link ReaderPlugin} (either
 * Stub or Pcsc) It is meant to work with a Keyple Pcsc Plugin or a Keyple Stub
 * Plugin.
 */
class SamResourceManagerDefault : public SamResourceManager {
public:
    /**
     * Reader observer to handle SAM insertion/withdraw
     */
    class ReaderObserver final : public ObservableReader::ReaderObserver {
    public:
        /**
         *
         */
        ReaderObserver(SamResourceManagerDefault& parent);

        /**
         * Handle {@link ReaderEvent}
         * <p>
         * Create {@link SeResource<CalypsoSam>}
         *
         * @param event the reader event
         */
        void update(const std::shared_ptr<ReaderEvent> event) override;

    private:
        /**
         *
         */
        const std::shared_ptr<Logger> mLogger =
            LoggerFactory::getLogger(typeid(ReaderObserver));

        /**
         *
         */
        SamResourceManagerDefault& mParent;
    };

    /**
     * Plugin observer to handle SAM reader connection/disconnection.
     * <p>
     * Add or remove readers
     * <p>
     * Add a reader observer when an {@link ObservableReader} is connected.
     */
    class PluginObserver final : public ObservablePlugin::PluginObserver {
    public:
        /**
         *
         */
        const std::shared_ptr<ReaderObserver> mReaderObserver;

        /**
         *
         */
        const std::string mSamReaderFilter;

        /**
         *
         */
        Pattern* p;

        /**
         *
         */
        PluginObserver(const std::shared_ptr<ReaderObserver> readerObserver,
                       const std::string& samReaderFilter,
                       SamResourceManagerDefault& parent);
        /**
         * Handle {@link PluginEvent}
         *
         * @param event the plugin event
         */
        void update(const std::shared_ptr<PluginEvent> event) override;

    private:
        /**
         *
         */
        const std::shared_ptr<Logger> mLogger =
            LoggerFactory::getLogger(typeid(PluginObserver));

        /**
         *
         */
        SamResourceManagerDefault& mParent;
    };

    /**
     * Only used with observable readers
     */
    std::shared_ptr<SamResourceManagerDefault::ReaderObserver> mReaderObserver;

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
     * @param readerPlugin the plugin through which SAM readers are accessible
     * @param samReaderFilter the regular expression defining how to identify
     *        SAM readers among others.
     * @param maxBlockingTime the maximum duration for which the
     *        allocateSamResource method will attempt to allocate a new reader
     *        by retrying (in milliseconds)
     * @param sleepTime the duration to wait between two retries
     * @throw KeypleReaderException thrown if an error occurs while getting the
     *        readers list.
     */
    SamResourceManagerDefault(std::shared_ptr<ReaderPlugin> readerPlugin,
                              const std::string& samReaderFilter,
                              const int maxBlockingTime,
                              const int sleepTime);

protected:
    /**
     *
     */
    std::shared_ptr<ReaderPlugin> mSamReaderPlugin;

    /**
     * Remove a {@link SeResource} from the current
     * {@code SeResourceCalypsoSam>} list
     *
     * @param samReader the SAM reader of the resource to remove from the list.
     */
    void removeResource(const std::shared_ptr<SeReader> samReader);

private:
    /**
     *
     */
    const std::shared_ptr<Logger> mLogger =
        LoggerFactory::getLogger(typeid(SamResourceManagerDefault));

    /**
     *
     */
    ConcurrentMap<const std::string, std::shared_ptr<ManagedSamResource>>
        mLocalManagedSamResources;

    /**
     * The maximum time (in milliseconds) during which the BLOCKING mode will
     * wait
     */
    const int mMaxBlockingTime;

    /*
     * The sleep time between two tries (in milliseconds) during which the
     * BLOCKING mode will wait
     */
    const int mSleepTime;

    /**
     *
     */
    std::mutex mMutex;

    /**
     *
     */
    void initSamReader(const std::shared_ptr<SeReader> samReader,
                       const std::shared_ptr<ReaderObserver> readerObserver);
};

}
}
}
