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

#include "AbstractDefaultSelectionsResponse.h"

/* Forward class declarations */
namespace keyple { namespace core { namespace seproxy { namespace message {
    class SeResponseSet; } } } }

namespace keyple {
namespace core {
namespace seproxy {
namespace message {

using namespace keyple::core::seproxy::event;

/**
 * Class containing the List of {@link SeResponse} used from a default selection
 * made at the {@link ObservableReader} level.
 */
class DefaultSelectionsResponse final
: public AbstractDefaultSelectionsResponse {
public:
    /**
     *
     */
    DefaultSelectionsResponse(
        std::list<std::shared_ptr<SeResponse>>& selectionSeResponseSet);

    /**
     *
     */
    std::list<std::shared_ptr<SeResponse>>& getSelectionSeResponseSet();

private:
    /**
     * The List of {@link SeResponse}
     */
    std::list<std::shared_ptr<SeResponse>>& selectionSeResponseSet;
};

}
}
}
}
