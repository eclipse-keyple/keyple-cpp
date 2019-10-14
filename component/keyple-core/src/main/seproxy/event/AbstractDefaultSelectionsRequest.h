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

/* Common */
#include "Export.h"

//JAVA TO C++ CONVERTER NOTE: Forward class declarations:
namespace org { namespace eclipse { namespace keyple { namespace core { namespace seproxy { namespace message { class SeRequestSet; } } } } } }

namespace org {
namespace eclipse {
namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using SeRequestSet = org::eclipse::keyple::core::seproxy::message::SeRequestSet;

/**
    * The {@link AbstractDefaultSelectionsRequest} class is dedicated to
    */
class EXPORT AbstractDefaultSelectionsRequest : public std::enable_shared_from_this<AbstractDefaultSelectionsRequest> {
    /** The {@link org.eclipse.keyple.core.seproxy.message.SeRequestSet} */
protected:
    const std::shared_ptr<SeRequestSet> selectionSeRequestSet;

    AbstractDefaultSelectionsRequest(std::shared_ptr<SeRequestSet> selectionSeRequestSet);

    virtual std::shared_ptr<SeRequestSet> getSelectionSeRequestSet() = 0;
};

}
}
}
}
}
}
