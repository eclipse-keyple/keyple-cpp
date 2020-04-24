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
#include <memory>

/* Core */
#include "KeypleCoreExport.h"
#include "SeResponse.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace event {

using namespace keyple::core::seproxy::message;

/**
 * The abstract class defining the default selections response in return to the
 * default selection made when the SE was inserted..
 * <p>
 * The default selections response provides a list of {@link SeResponse}
 */
class KEYPLECORE_API AbstractDefaultSelectionsResponse
: public std::enable_shared_from_this<AbstractDefaultSelectionsResponse> {
public:
    /**
     *
     */
    virtual ~AbstractDefaultSelectionsResponse()
    {
    }

protected:
    /**
     * @return the list of {@link SeResponse}
     */
    virtual std::list<std::shared_ptr<SeResponse>>&
        getSelectionSeResponseSet() = 0;
};

}
}
}
}
