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

#include <memory>

/* Core */
#include "AbstractIso7816CommandBuilder.h"
#include "ApduResponse.h"

/* Calypso */
#include "SamRevision.h"
#include "CalypsoSamCommand.h"

/* Common */
#include "InstantiationException.h"

/* Forward declaration */
namespace keyple { namespace calypso { namespace command { namespace sam {
    class AbstractSamResponseParser; } } } }

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace keyple::common;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

/**
 * Superclass for all SAM command builders.
 * <p>
 * Used directly, this class can serve as low level command builder.
 */
template<typename T>
class AbstractSamCommandBuilder : public AbstractIso7816CommandBuilder {
public:
    /**
     *
     */
    AbstractSamCommandBuilder(const std::shared_ptr<SeCommand> reference,
                              const std::shared_ptr<ApduRequest> request)
    : AbstractIso7816CommandBuilder(reference, request)
    {
#if !defined(_WIN32)
        static_assert(std::is_base_of<AbstractSamResponseParser, T>::value,
                      "T must inherit from keyple::core::command" \
                      "::AbstractSamResponseParser");
#endif
    }

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
    virtual std::shared_ptr<T> createResponseParser(
        const std::shared_ptr<ApduResponse> apduResponse) = 0;

    /**
     * Return type should be
     *   const std::shared_ptr<CalypsoSamCommand>
     * ... but considered invalid covariant
     */
    const std::shared_ptr<SeCommand> getCommandRef() const override
    {
        const std::shared_ptr<CalypsoSamCommand> cmd =
            std::dynamic_pointer_cast<CalypsoSamCommand>(mCommandRef);

        if (cmd == nullptr)
            throw InstantiationException(
                      "mCommandRef conversion to CalypsoSamCommand failed");

        return cmd;
    }

protected:
    /**
     *
     */
    SamRevision mDefaultRevision = SamRevision::C1;
};

}
}
}
}
