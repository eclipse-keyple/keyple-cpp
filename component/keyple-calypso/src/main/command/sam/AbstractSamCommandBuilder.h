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

#include "AbstractIso7816CommandBuilder.h"
#include "SamRevision.h"
#include "CalypsoSamCommands.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

/**
 * Superclass for all SAM command builders.
 * <p>
 * Used directly, this class can serve as low level command builder.
 */
class AbstractSamCommandBuilder : public AbstractIso7816CommandBuilder {
public:
    /**
     *
     */
    AbstractSamCommandBuilder(CalypsoSamCommands& reference,
                              std::shared_ptr<ApduRequest> request);

    /**
     *
     */
    virtual ~AbstractSamCommandBuilder()
    {
    }

protected:
    /**
     *
     */
    SamRevision defaultRevision = SamRevision::S1D; // 94

    /**
     *
     */
    std::shared_ptr<AbstractSamCommandBuilder> shared_from_this()
    {
        return std::static_pointer_cast<AbstractSamCommandBuilder>(
            AbstractIso7816CommandBuilder::shared_from_this());
    }
};

}
}
}
}
