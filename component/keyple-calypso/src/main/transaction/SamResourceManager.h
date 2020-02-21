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

#pragma once

#include <list>
#include <mutex>

/* Common */
#include "Export.h"
#include "LoggerFactory.h"

/* Core */
#include "ObservablePlugin.h"
#include "ObservableReader.h"
#include "PluginEvent_Import.h"
#include "ReaderEvent_Import.h"
#include "ReaderPlugin.h"

/* Calypso */
#include "SamIdentifier.h"
#include "SamResource.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::common;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::event;

/**
 * Management of SAM resources:
 * <p>
 * Provides methods fot the allocation/deallocation of SAM resources
 */
class SamResourceManager {
public:
    /**
     *
     */
    enum class AllocationMode {
        BLOCKING,
        NON_BLOCKING
    };

    /**
     * Instantiate a new SamResourceManager.
     * <p>
     * The samReaderPlugin is used to retrieve the available SAM according to
     * the provided filter.
     * <p>
     * Setup a plugin observer if the reader plugin is observable.
     *
     * @param samReaderPlugin the plugin through which SAM readers are
     *        accessible
     * @param samReaderFilter the regular expression defining how to identify
     *        SAM readers among others.
     * @throws KeypleReaderException throw if an error occurs while getting the
     *         readers list.
     */
    SamResourceManager(std::shared_ptr<ReaderPlugin> samReaderPlugin,
                       const std::string& samReaderFilter);
    /**
     * Allocate a SAM resource from the specified SAM group.
     * <p>
     * In the case where the allocation mode is BLOCKING, this method will wait
     * until a SAM resource becomes free and then return the reference to the
     * allocated resource. However, the BLOCKING mode will wait a maximum time
     * defined in tenths of a second by MAX_BLOCKING_TIME.
     * <p>
     * In the case where the allocation mode is NON_BLOCKING and no SAM resource
     * is available, this method will return null.
     * <p>
     * If the samGroup argument is null, the first available SAM resource will
     * be selected and returned regardless of its group.
     *
     * @param allocationMode the blocking/non-blocking mode
     * @param samIdentifier the targeted SAM identifier
     * @return a SAM resource
     * @throws KeypleReaderException if a reader error occurs
     */
    std::unique_ptr<SamResource> allocateSamResource(
       const AllocationMode allocationMode, const SamIdentifier& samIdentifier);

    /**
     * Free a previously allocated SAM resource.
     *
     * @param samResource the SAM resource reference to free
     */
    void freeSamResource(std::unique_ptr<SamResource> samResource);

    /**
     *
     */
    const std::shared_ptr<Logger> logger =
              LoggerFactory::getLogger(typeid(SamResourceManager));

private:
    /**
     * The maximum time (in tenths of a second) during which the BLOCKING mode
     * will wait
     */
    static const int MAX_BLOCKING_TIME;

    /**
     *
     */
    std::shared_ptr<ReaderPlugin> samReaderPlugin;

    /**
     *
     */
    std::list<std::unique_ptr<SamResource>> localSamResources;

    /**
     *
     */
    bool dynamicAllocationPlugin;

    /**
     *
     */
    std::mutex mtx;

    /**
     * Create a SAM resource from the provided SAM reader.
     * <p>
     * Proceed with the SAM selection and combine the SAM reader and the
     * Calypso SAM resulting from the selection.
     *
     * @param samReader the SAM reader with which the APDU exchanges will be
     *        done.
     * @return a {@link SamResource}
     * @throws KeypleReaderException if an reader error occurs while doing the
     *         selection
     */
    std::unique_ptr<SamResource> createSamResource(
        std::shared_ptr<SeReader> samReader);

    /**
     * Remove a {@link SamResource}from the current SamResource list
     *
     * @param samReader the SAM reader of the resource to remove from the list.
     */
    void removeResource(std::shared_ptr<SeReader> samReader);

    /**
     * Reader observer to handle SAM insertion/withdraw
     */
    class ReaderObserver : public ObservableReader::ReaderObserver {
    public:
        /**
         * Handle {@link ReaderEvent}
         * <p>
         * Create {@link SamResource}
         *
         * @param event the reader event
         */
        void update(std::shared_ptr<ReaderEvent> event) override;

        /**
         *
         */
        ReaderObserver(SamResourceManager *parent);

    private:
        /**
         *
         */
        const std::shared_ptr<Logger> logger;

        /**
         *
         */
        SamResourceManager *parent;
    };

    /**
     * Plugin observer to handle SAM reader connection/disconnection.
     * <p>
     * Add or remove readers
     * <p>
     * Add a reader observer when an {@link ObservableReader} is connected.
     */
    class PluginObserver : public ObservablePlugin::PluginObserver {
    public:
        /**
         * Handle {@link PluginEvent}
         *
         * @param event the plugin event
         */
        void update(std::shared_ptr<PluginEvent> event) override;

        /**
         *
         */
        PluginObserver(SamResourceManager *parent,
                       std::shared_ptr<ReaderObserver> readerObserver,
                       const std::string samReaderFilter);

    private:
        /**
         *
         */
        std::shared_ptr<ReaderObserver> readerObserver;

        /**
         *
         */
        const std::string samReaderFilter;

        /**
         *
         */
        Pattern *p;

        /**
         *
         */
        const std::shared_ptr<Logger> logger;

        /**
         *
         */
        SamResourceManager *parent;
    };
};

}
}
}
