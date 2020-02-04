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

#include <memory>

/* Calypso */
#include "CalypsoSam.h"
#include "SamIdentifier.h"

/* Core */
#include "SeResource.h"
#include "SeReader.h"

/* Common */
#include "Export.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::core::selection;
using namespace keyple::calypso;
using namespace keyple::calypso::transaction;
using namespace keyple::core::seproxy;

class EXPORT SamResource : public SeResource<CalypsoSam> {
public:
    /**
     * The free/busy enum status
     */
    enum SamResourceStatus {
        FREE,
        BUSY
    };

    /**
     * Constructor
     *
     * @param seReader the {@link SeReader} with which the SE is communicating
     * @param calypsoSam the {@link CalypsoSam} information structure
     */
    SamResource(std::shared_ptr<SeReader> seReader,
                std::shared_ptr<CalypsoSam> calypsoSam);

    /**
     *
     */
    virtual ~SamResource()
    {
    }

    /**
     * Indicates whether the SamResource is FREE or BUSY
     *
     * @return the busy status
     */
    bool isSamResourceFree();

    /**
     * Defines the {@link SamIdentifier} of the current {@link SamResource}
     *
     * @param samIdentifier the SAM identifier
     */
    void setSamIdentifier(SamIdentifier* samIdentifier);

    /**
     * Indicates whether the SamResource matches the provided SAM identifier.
     * <p>
     * The test includes the {@link org.eclipse.keyple.calypso.command.sam
     * .SamRevision}, serial number and group reference provided by the {@link
     * SamIdentifier}.
     * <p>
     * The SAM serial number can be null or empty, in this case all serial
     * numbers are accepted. It can also be a regular expression target one or
     * more specific serial numbers.
     * <p>
     * The groupe reference can be null or empty to let all group references
     * match but not empty the group reference must match the {@link
     * SamIdentifier} to have the method returning true.
     *
     * @param samIdentifier the SAM identifier
     * @return true or false according to the result of the correspondence test
     */
    bool isSamMatching(SamIdentifier* samIdentifier);

    /**
     * Sets the free/busy status of the SamResource
     *
     * @param samResourceStatus FREE/BUSY enum value
     */
    void setSamResourceStatus(SamResourceStatus samResourceStatus);

protected:
    /**
     *
     */
    std::shared_ptr<SamResource> shared_from_this()
    {
        return std::static_pointer_cast<SamResource>(
                   SeResource<CalypsoSam>::shared_from_this());
    }


private:
    /**
     * The free/busy status of the resource
     */
    SamResourceStatus samResourceStatus;

    /**
     * The sam identifier
     */
    SamIdentifier* samIdentifier;
};

}
}
}
