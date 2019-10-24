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

/* Common */
#include "Export.h"

/* Forward class declarations */
namespace keyple { namespace core { namespace seproxy { namespace message {
    class SeResponseSet; } } } }

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using namespace keyple::core::seproxy::message;

class EXPORT AbstractDefaultSelectionsResponse
: public std::enable_shared_from_this<AbstractDefaultSelectionsResponse> {
public:
    /**
     *
     */
    virtual ~AbstractDefaultSelectionsResponse() {}

protected:
    /**
     * The {@link keyple::core::seproxy::message::SeResponseSet}
     */
    const std::shared_ptr<SeResponseSet> selectionSeResponseSet;

    /**
     *
     */
    AbstractDefaultSelectionsResponse(std::shared_ptr<SeResponseSet>
        selectionSeResponseSet);

    /**
     *
     */
    virtual std::shared_ptr<SeResponseSet> getSelectionSeResponseSet() = 0;
};

}
}
}
}
