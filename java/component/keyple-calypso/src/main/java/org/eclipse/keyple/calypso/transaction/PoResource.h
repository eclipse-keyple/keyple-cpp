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

namespace org {
namespace eclipse {
namespace keyple {
namespace calypso {
namespace transaction {

using namespace org::eclipse::keyple::core::selection;

using CalypsoPo = org::eclipse::keyple::calypso::transaction::CalypsoPo;
using SeReader  = org::eclipse::keyple::core::seproxy::SeReader;

class PoResource : public SeResource<CalypsoPo> {
    /**
        * Constructor
        *
        * @param seReader the {@link SeReader} with which the SE is communicating
        * @param calypsoPo the {@link CalypsoPo} information structure
        */
public:
    PoResource(std::shared_ptr<SeReader> seReader, std::shared_ptr<CalypsoPo> calypsoPo);

protected:
    std::shared_ptr<PoResource> shared_from_this() {
        return std::static_pointer_cast<PoResource>(SeResource<CalypsoPo>::shared_from_this());
    }
};

}
}
}
}
}
