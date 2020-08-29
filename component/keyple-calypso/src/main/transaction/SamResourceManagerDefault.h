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

/* Common */
#include "LoggerFactory.h"

/* Calypso */
#include "SamResourceManager.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;

/**
 * Implementation of Sam Resource Manager working a {@link ReaderPlugin} (either
 * Stub or Pcsc) It is meant to work with a Keyple Pcsc Plugin or a Keyple Stub
 * Plugin.
 */
class SamResourceManagerDefault : public SamResourceManager {
public:
    /**
     * Only used with observable readers
     */
    std::shared_ptr<SamResourceManagerDefault::ReaderObserver> mReaderObserver;

    /**
     *
     */
    std::shared_ptr<SeResource<CalypsoSam>> allocateSamResource(
        const AllocationMode allocationMode,
        const SamIdentifier& samIdentifier) override;

    /**
     *
     */
    void freeSamResource(
        const std::shared_ptr<SeResource<CalypsoSam>> samResource) override;

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
                       const std::string& samReaderFilter);
        /**
         * Handle {@link PluginEvent}
         *
         * @param event the plugin event
         */
        void update(const PluginEvent& event) override;
    };

    /**
     * Reader observer to handle SAM insertion/withdraw
     */
    class ReaderObserver final : public ObservableReader::ReaderObserver {
    public:
        /**
         *
         */
        ReaderObserver();

        /**
         * Handle {@link ReaderEvent}
         * <p>
         * Create {@link SeResource<CalypsoSam>}
         *
         * @param event the reader event
         */
        void update(const ReaderEvent& event) override;
    };

    /**
     * (package-private)<br>
     * Inner class to handle specific attributes associated with an {@code
     * SeResource<CalypsoSam>} in the {@link SamResourceManager} context.
     *
     * @since 0.9
     */
    static class ManagedSamResource final : public SeResource<CalypsoSam> {
    public:
        /**
         * The free/busy enum status
         */
        enum class SamResourceStatus {
            FREE,
            BUSY;
        };

        /**
         * Constructor
         *
         * @param seReader the {@link SeReader} with which the SE is
         *        communicating
         * @param calypsoSam the {@link CalypsoSam} information structure
         */
        ManagedSamResource(std::shared_ptr<SeReader> seReader,
                           std::shared_ptr<CalypsoSam> calypsoSam);

        /**
         * Indicates whether the ManagedSamResource is FREE or BUSY
         *
         * @return the busy status
         */
        const bool isSamResourceFree() const;

        /**
         * Defines the {@link SamIdentifier} of the current {@link
         * ManagedSamResource}
         *
         * @param samIdentifier the SAM identifier
         */
        void setSamIdentifier(std::shared_ptr<SamIdentifier> samIdentifier);

        /**
         * Indicates whether the ManagedSamResource matches the provided SAM
         * identifier.
         * <p>
         * The test includes the {@link SamRevision}, serial number and group
         * reference provided by the {@link SamIdentifier}.
         * <p>
         * The SAM serial number can be null or empty, in this case all serial
         * numbers are accepted. It can also be a regular expression target one
         * or more specific serial numbers.
         * <p>
         * The groupe reference can be null or empty to let all group references
         * match but not empty the group reference must match the {@link
         * SamIdentifier} to have the method returning true.
         *
         * @param samIdentifier the SAM identifier
         * @return true or false according to the result of the correspondence
         *         test
         */
        const bool isSamMatching(
            const std::shared_ptr<SamIdentifier> samIdentifier) const;

        /**
         * Sets the free/busy status of the ManagedSamResource
         *
         * @param samResourceStatus FREE/BUSY enum value
         */
        void setSamResourceStatus(const SamResourceStatus& samResourceStatus);

    private:
        /**
         * The free/busy status of the resource
         */
        SamResourceStatus mSamResourceStatus;

        /**
         * The sam identifier
         */
        std::shared_ptr<SamIdentifier> mSamIdentifier;
    }

protected:
    /**
     *
     */
    std::shared_ptr<ReaderPlugin> mSamReaderPlugin;

    /**
     * Protected constructor, use the {@link SamResourceManagerFactory}
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
