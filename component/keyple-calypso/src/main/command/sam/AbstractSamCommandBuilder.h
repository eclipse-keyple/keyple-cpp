/******************************************************************************
 * Copyright (c) 2020 Calypso Networks Association                            *
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

/* Core */
#include "AbstractIso7816CommandBuilder.h"

/* Calypso */
#include "AbstractSamResponseParser.h"
#include "SamRevision.h"
#include "CalypsoSamCommand.h"

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
template<class T>
class AbstractSamCommandBuilder<T> : public AbstractIso7816CommandBuilder {
public:
    static_assert(std::is_base_of<AbstractSamResponseParser, T>::value,
                  "T must inherit from keyple::core::command" \
                  "::AbstractApduResponseParser");

    /**
     *
     */
    AbstractSamCommandBuilder(const CalypsoSamCommand& reference,
                              const std::shared_ptr<ApduRequest> request);

    /**
     *
     */
    virtual ~AbstractSamCommandBuilder() = default;

    /**
     * Create the response parser matching the builder
     *
     * @param apduResponse the response data from the SE
     * @return an {@link AbstractApduResponseParser}
     */
    virtual std::unique_ptr<T> createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse) = 0;

    /**
     *
     */
    const std::shared_ptr<CalypsoSamCommand> getCommandRef() const override;

protected:
    /**
     *
     */
    SamRevision& mDefaultRevision = SamRevision::C1;
};

}
}
}
}
