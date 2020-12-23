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
#include <type_traits>

/* Common */
#include "LoggerFactory.h"

/* Core */

#include "ApduRequest.h"
#include "ApduResponse.h"

/* Calypso */
#include "AbstractPoCommandBuilder.h"
#include "AbstractIso7816CommandBuilder.h"
#include "CalypsoPoCommand.h"
#include "KeypleCalypsoExport.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {

class AbstractPoResponseParser;

using namespace keyple::common;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

/**
 * Abstract class for all PO command builders.
 */
template<typename T>
class KEYPLECALYPSO_API AbstractPoCommandBuilder : public AbstractIso7816CommandBuilder {
public:
    /**
     * Constructor dedicated for the building of referenced Calypso commands
     *
     * @param commandRef a command reference from the Calypso command table
     * @param request the ApduRequest (the instruction byte will be overwritten)
     */
    AbstractPoCommandBuilder(std::shared_ptr<CalypsoPoCommand> commandRef,
                             std::shared_ptr<ApduRequest> request)
    : AbstractIso7816CommandBuilder(commandRef, request)
    {
        static_assert(std::is_base_of<AbstractPoResponseParser, T>::value,
                      "T must inherit from keyple::core::command" \
                      "::AbstractPoResponseParser");
    }

    /**
     *
     */
    virtual ~AbstractPoCommandBuilder() = default;

    /**
     * Create the response parser matching the builder
     *
     * @param apduResponse the response data from the SE
     * @return an keyple::core::command::AbstractApduResponseParser
     */
    virtual std::shared_ptr<T> createResponseParser(
        std::shared_ptr<ApduResponse> apduResponse) = 0;

    /**
     * Return type should be
     *   std::shared_ptr<CalypsoPoCommand>
     * ... but compiler complains about an invalid covariant return type
     */
    virtual const std::shared_ptr<SeCommand> getCommandRef() const override
    {
        return std::dynamic_pointer_cast<SeCommand>(mCommandRef);
    }

    /**
     * Indicates if the session buffer is used when executing this command.
     * <p>
     * Allows the management of the overflow of this buffer.
     *
     * @return true if this command uses the session buffer
     */
    virtual bool isSessionBufferUsed() const = 0;

protected:
    /**
     * common logger for all builders
     */
    const std::shared_ptr<Logger> mLogger =
        LoggerFactory::getLogger(typeid(AbstractPoCommandBuilder));
};

}
}
}
}
