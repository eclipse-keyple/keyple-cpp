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
 * Class containing the
 * {@link org.eclipse.keyple.core.seproxy.message.SeResponseSet} used from a
 * default selection made at the {@link ObservableReader} level.
 */
class DefaultSelectionsResponse final
: public AbstractDefaultSelectionsResponse {
public:
    /**
     *
     */
    DefaultSelectionsResponse(std::shared_ptr<SeResponseSet>
        selectionSeResponseSet);

    /**
     *
     */
    std::shared_ptr<SeResponseSet> getSelectionSeResponseSet() override;

protected:
    /**
     *
     */
    std::shared_ptr<DefaultSelectionsResponse> shared_from_this()
    {
        return std::static_pointer_cast<DefaultSelectionsResponse>(
                   AbstractDefaultSelectionsResponse::shared_from_this());
    }
};

}
}
}
}
