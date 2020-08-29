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

#include "AbstractSamResponseParser.h"

/* Core */
#include "ApduResponse.h"

/* Calypso */
#include "CalypsoSamIllegalParameterException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace keyple::calypso::command::sam::exception;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

const std::map<int, std::shared_ptr<StatusProperties>>
    AbstractSamResponseParser::STATUS_TABLE = {
    {
        0x6D00,
        std::make_shared<StatusProperties>(
            "Instruction unknown.",
            typeid(CalypsoSamIllegalParameterException))
    }, {
        0x6E00,
        std::make_shared<StatusProperties>(
            "Class not supported.",
            typeid(CalypsoSamIllegalParameterException))
    }
};

AbstractSamResponseParser::AbstractSamResponseParser(
  const std::shared_ptr<ApduResponse> response)
: AbstractApduResponseParser(response)
{
}

const std::map<int, std::shared_ptr<StatusProperties>>&
    AbstractSamResponseParser::getStatusTable() const
{
    return STATUS_TABLE;
}

std::shared_ptr<bstractSamCommandBuilder> AbstractPoResponseParser::getBuilder()
{
    return AbstractApduResponseParser::getBuilder();
}

KeypleSeCommandException AbstractSamResponseParser::buildCommandException(
    const type_info& exceptionClass,
    const std::string& message, SeCommand commandRef, int statusCode)
{
    KeypleSeCommandException e;

    std::shared_ptr<CalypsoPoCommand> command =
        std::dynamic_pointer_cast<CalypsoPoCommand>(mCommandRef);

    if (exceptionClass == typeid(CalypsoSamAccessForbiddenException)) {
        e = CalypsoSamAccessForbiddenException(message, command, statusCode);
    } else if (exceptionClass == typeid(CalypsoSamCounterOverflowException)) {
        e = CalypsoSamCounterOverflowException(message, command, statusCode);
    } else if (exceptionClass == typeid(CalypsoSamDataAccessException)) {
        e = CalypsoSamDataAccessException(message, command, statusCode);
    } else if (exceptionClass == typeid(CalypsoSamIllegalArgumentException)) {
        e = CalypsoSamIllegalArgumentException(message, command);
    } else if (exceptionClass == typeid(CalypsoSamIllegalParameterException)) {
        e = CalypsoSamIllegalParameterException(message, command, statusCode);
    } else if (exceptionClass == typeid(CalypsoSamIncorrectInputDataException)) {
        e = CalypsoSamIncorrectInputDataException(message, command, statusCode);
    } else if (exceptionClass == typeid(CalypsoSamSecurityDataException)) {
        e = CalypsoSamSecurityDataException(message, command, statusCode);
    } else {
        e = CalypsoSamUnknownStatusException(message, command, statusCode);
    }

    return std::move(e);
}

void AbstractSamResponseParser::checkStatus() const
{
    try {
        AbstractApduResponseParser::checkStatus();
    } catch (const KeypleSeCommandException& e) {
        throw CalypsoSamCommandException(e.message, e.cause);
    }
}

}
}
}
}
