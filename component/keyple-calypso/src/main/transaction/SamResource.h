/********************************************************************************
* Copyright (c) 2019 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <memory>

#include "CalypsoSam.h"

#include "SeResource.h"
#include "SeReader.h"

/* Common */
#include "Export.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::core::selection;
using namespace keyple::calypso::transaction;
using namespace keyple::core::seproxy;

class EXPORT SamResource : public SeResource<CalypsoSam> {
public:
    /**
     * Constructor
     *
     * @param seReader the {@link SeReader} with which the SE is communicating
     * @param calypsoSam the {@link CalypsoSam} information structure
     */
    SamResource(std::shared_ptr<SeReader> seReader, std::shared_ptr<CalypsoSam> calypsoSam);

    /**
     *
     */
    virtual ~SamResource()
    {
    }

protected:
    /**
     *
     */
    std::shared_ptr<SamResource> shared_from_this()
    {
        return std::static_pointer_cast<SamResource>(SeResource<CalypsoSam>::shared_from_this());
    }
};

}
}
}
