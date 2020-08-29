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

#include "AbstractPoResponseParser.h"

/* Core */
#include "ApduResponse.h"

/* Calypso */
#include "CalypsoPoCommand.h"
#include "CalypsoPoAccessForbiddenException.h"
#include "CalypsoPoDataAccessException.h"
#include "CalypsoPoDataOutOfBoundsException.h"
#include "CalypsoPoIllegalArgumentException.h"
#include "CalypsoPoPinException.h"
#include "CalypsoPoSecurityContextException.h"
#include "CalypsoPoSecurityDataException.h"
#include "CalypsoPoTerminatedException.h"
#include "CalypsoPoUnknownStatusException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace po {

using namespace keyple::calypso::command::po::exception;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

AbstractPoResponseParser::AbstractPoResponseParser(
  std::shared_ptr<ApduResponse> response,
  AbstractPoCommandBuilder<AbstractPoResponseParser>* builder);
: AbstractApduResponseParser(response, builder) {}

const std::shared_ptr<AbstractPoCommandBuilder<AbstractPoResponseParser>
    AbstractPoResponseParser::getBuilder() const
{
    return AbstractApduResponseParser::getBuilder();
}

KeypleSeCommandException AbstractPoResponseParser::buildCommandException(
    const std::type_info& exceptionClass,
    const std::string& message,
    const std::shared_ptr<SeCommand> commandRef,
    const int statusCode)
{
    KeypleSeCommandException e;

    std::shared_ptr<CalypsoPoCommand> command =
        std::dynamic_pointer_cast<CalypsoPoCommand>(commandRef);

    if (exceptionClass == typeid(CalypsoPoAccessForbiddenException)) {
        e = CalypsoPoAccessForbiddenException(message, command, statusCode);
    } else if (exceptionClass == typeid(CalypsoPoDataAccessException)) {
        e = CalypsoPoDataAccessException(message, command, statusCode);
    } else if (exceptionClass == typeid(CalypsoPoDataOutOfBoundsException)) {
        e = CalypsoPoDataOutOfBoundsException(message, command, statusCode);
    } else if (exceptionClass == typeid(CalypsoPoIllegalArgumentException)) {
        e = CalypsoPoIllegalArgumentException(message, command);
    } else if (exceptionClass == typeid(CalypsoPoIllegalParameterException)) {
        e = CalypsoPoIllegalParameterException(message, command, statusCode);
    } else if (exceptionClass == typeid(CalypsoPoPinException)) {
        e = CalypsoPoPinException(message, command, statusCode);
    } else if (exceptionClass == typeid(CalypsoPoSecurityContextException)) {
        e = CalypsoPoSecurityContextException(message, command, statusCode);
    } else if (exceptionClass == typeid(CalypsoPoSecurityDataException)) {
        e = CalypsoPoSecurityDataException(message, command, statusCode);
    } else if (exceptionClass == typeid(CalypsoPoSessionBufferOverflowException)) {
        e = CalypsoPoSessionBufferOverflowException(message, command, statusCode);
    } else if (exceptionClass == typeid(CalypsoPoTerminatedException)) {
        e = CalypsoPoTerminatedException(message, command, statusCode);
    } else {
        e = CalypsoPoUnknownStatusException(message, command, statusCode);
    }

    return std::move(e);
}

void AbstractPoResponseParser::checkStatus() const
{
    try {
        AbstractApduResponseParser::checkStatus();
    } catch (const KeypleSeCommandException& e) {
        throw CalypsoPoCommandException(e.getMessage(), e.getCause());
    }
}

}
}
}
}
