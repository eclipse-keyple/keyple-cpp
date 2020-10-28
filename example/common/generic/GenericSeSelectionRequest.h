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

#include "AbstractMatchingSe.h"
#include "AbstractSeSelectionRequest.h"
#include "SeSelector.h"
#include "TransmissionMode.h"

namespace keyple {
namespace example {
namespace generic {
namespace common {

using namespace keyple::core::selection;
using namespace keyple::core::seproxy;
using namespace keyple::core::seproxy::message;
using namespace keyple::core::seproxy::protocol;

/**
 * Create a new class extending AbstractSeSelectionRequest
 */
class GenericSeSelectionRequest final
: public AbstractSeSelectionRequest<AbstractApduCommandBuilder> {
public:
    /**
     *
     */
    GenericSeSelectionRequest(std::shared_ptr<SeSelector> seSelector);

    /**
     *
     */
    class GenericMatchingSe final : public AbstractMatchingSe {
    public:
        /**
         * Constructor
         */
        GenericMatchingSe(std::shared_ptr<SeResponse> selectionResponse,
                          const TransmissionMode& transmissionMode);
    };

protected:
    /**
     *
     */
    const std::shared_ptr<AbstractMatchingSe> parse(std::shared_ptr<SeResponse> seResponse)
        override;

private:
    /**
     *
     */
    const TransmissionMode mTransmissionMode;
};

}
}
}
}
