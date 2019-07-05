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

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace transaction {

using namespace org::eclipse::keyple::core::selection;

using CalypsoSam = org::eclipse::keyple::calypso::transaction::CalypsoSam;

using SeReader   = org::eclipse::keyple::core::seproxy::SeReader;

class SamResource : public SeResource<CalypsoSam> {
    /**
        * Constructor
        *
        * @param seReader the {@link SeReader} with which the SE is communicating
        * @param calypsoSam the {@link CalypsoSam} information structure
        */
public:
    SamResource(std::shared_ptr<SeReader> seReader, std::shared_ptr<CalypsoSam> calypsoSam);

protected:
    std::shared_ptr<SamResource> shared_from_this() {
        return std::static_pointer_cast<SamResource>(SeResource<CalypsoSam>::shared_from_this());
    }
};

}
}
}
}
}
