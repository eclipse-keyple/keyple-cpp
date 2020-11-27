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

#include <vector>
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
 * The abstract class defining the default selections response in return to the default selection
 * made when the SE was inserted..
 * <p>
 * The default selections response provides a list of {@link SeResponse}<br>
 * The purpose of this abstract class is to hide the constructor that is defined as public in its
 * implementation {@link keyple::core::seproxy::message::DefaultSelectionsRequest}.
 */
class KEYPLECORE_API AbstractDefaultSelectionsResponse {
public:
    /**
     *
     */
    virtual ~AbstractDefaultSelectionsResponse() {}

    /**
     * @return the list of {@link SeResponse}
     */
    virtual const std::vector<std::shared_ptr<SeResponse>>& getSelectionSeResponses() const;

protected:
    /**
     *
     */
    explicit AbstractDefaultSelectionsResponse(
        const std::vector<std::shared_ptr<SeResponse>>& selectionSeResponses);

private:
    /**
     *
     */
    const std::vector<std::shared_ptr<SeResponse>> mSelectionSeResponses;
};

}
}
}
}
