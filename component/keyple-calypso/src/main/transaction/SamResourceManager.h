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

/* Core */
#include "SeReader.h"
#include "SeResource.h"

/* Calypso */
#include "CalypsoSam.h"
#include "ManagedSamResource.h"
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
        const AllocationMode allocationMode, const SamIdentifier& samIdentifier)
        = 0;

    /**
     * Free a previously allocated SAM resource.
     *
     * @param samResource the SAM resource reference to free
     */
    virtual void freeSamResource(
        std::shared_ptr<SeResource<CalypsoSam>> samResource) = 0;

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
    std::unique_ptr<ManagedSamResource> createSamResource(
        std::shared_ptr<SeReader> samReader);
};

}
}
}
