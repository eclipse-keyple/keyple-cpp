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

#pragma once

/* Core */
#include "SeReader.h"
#include "SeResource.h"

/* Calypso */
#include "CalypsoSam.h"
#include "KeypleCalypsoExport.h"
#include "SamIdentifier.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::core::selection;
using namespace keyple::core::seproxy;

/**
 * Management of SAM resources:
 *
 * <p>
 * Provides methods fot the allocation/deallocation of SAM resources
 */
class KEYPLECALYPSO_API SamResourceManager {
public:
    /**
     *
     */
    enum class AllocationMode {
        BLOCKING,
        NON_BLOCKING
    };

    /**
     * (package-private)<br>
     * Inner class to handle specific attributes associated with an {@code
     * SeResource<CalypsoSam>} in the {@link SamResourceManager} context.
     *
     * @since 0.9
     */
    class KEYPLECALYPSO_API ManagedSamResource final : public SeResource<CalypsoSam> {
    public:
        /**
         * The free/busy enum status
         */
        enum class SamResourceStatus {
            FREE,
            BUSY
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
        bool isSamResourceFree() const;

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
        bool isSamMatching(const std::shared_ptr<SamIdentifier> samIdentifier) const;

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
    };

    /**
     *
     */
    virtual ~SamResourceManager() = default;

    /**
     * Allocate a SAM resource from the specified SAM group.
     *
     * <p>
     * In the case where the allocation mode is BLOCKING, this method will wait
     * until a SAM resource becomes free and then return the reference to the
     * allocated resource. However, the BLOCKING mode will wait a maximum time
     * defined in milliseconds by MAX_BLOCKING_TIME.
     *
     * <p>
     * In the case where the allocation mode is NON_BLOCKING and no SAM resource
     * is available, this method will return an exception.
     *
     * <p>
     * If the samGroup argument is null, the first available SAM resource will
     * be selected and returned regardless of its group.
     *
     * @param allocationMode the blocking/non-blocking mode
     * @param samIdentifier the targeted SAM identifier
     * @return a SAM resource
     * @throw CalypsoNoSamResourceAvailableException if no resource is available
     * @throw KeypleReaderException if a reader error occurs
     * @throw KeypleAllocationReaderException if reader allocation fails
     */
    virtual std::shared_ptr<SeResource<CalypsoSam>> allocateSamResource(
        const AllocationMode allocationMode,
        const std::shared_ptr<SamIdentifier> samIdentifier) = 0;

    /**
     * Free a previously allocated SAM resource.
     *
     * @param samResource the SAM resource reference to free
     */
    virtual void freeSamResource(const SeResource<CalypsoSam>& samResource) = 0;

protected:
    /**
     * Create a SAM resource from the provided SAM reader.
     *
     * <p>
     * Proceed with the SAM selection and combine the SAM reader and the Calypso
     * SAM resulting from the selection.
     *
     * @param samReader the SAM reader with which the APDU exchanges will be
     *        done.
     * @return a {@link SeResource}
     * @throw CalypsoNoSamResourceAvailableException if an error occurs while
     *        doing the selection
     */
    virtual std::shared_ptr<ManagedSamResource> createSamResource(
        std::shared_ptr<SeReader> samReader);
};

}
}
}
