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

/* Core */
#include "SeResource.h"
#include "SeReader.h"

/* Calypso */
#include "CalypsoPo.h"

/* Common */
#include "Export.h"

namespace keyple {
namespace calypso {
namespace transaction {

using namespace keyple::core::selection;
using namespace keyple::calypso::transaction;
using namespace keyple::core::seproxy;

class EXPORT PoResource : public SeResource<CalypsoPo> {
public:
    /**
     * Constructor
     *
     * @param seReader the {@link SeReader} with which the SE is communicating
     * @param calypsoPo the {@link CalypsoPo} information structure
     */
    PoResource(std::shared_ptr<SeReader> seReader, std::shared_ptr<CalypsoPo> calypsoPo);

    /**
     *
     */
    virtual ~PoResource() {}

protected:
    /**
     *
     */
    std::shared_ptr<PoResource> shared_from_this()
    {
        return std::static_pointer_cast<PoResource>(SeResource<CalypsoPo>::shared_from_this());
    }
};

}
}
}
