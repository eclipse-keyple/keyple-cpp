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

#include <vector>
#include <memory>

#include "AbstractDefaultSelectionsResponse.h"
#include "KeypleCoreExport.h"

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::core::seproxy::event;

/**
 * Class containing the List of {@link SeResponse} used from a default selection
 * made at the {@link ObservableReader} level.<br>
 * It implements the {@link AbstractDefaultSelectionsResponse} class by
 * providing it with a public constructor.
 */
class KEYPLECORE_API DefaultSelectionsResponse final
: public AbstractDefaultSelectionsResponse {
public:
    /**
     *
     */
    DefaultSelectionsResponse(
        const std::vector<std::shared_ptr<SeResponse>>& selectionSeResponseSet);
};

}
}
}
}
