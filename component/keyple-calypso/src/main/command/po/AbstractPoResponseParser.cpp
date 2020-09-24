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
#include "CalypsoPoIllegalParameterException.h"
#include "CalypsoPoPinException.h"
#include "CalypsoPoSecurityContextException.h"
#include "CalypsoPoSecurityDataException.h"
#include "CalypsoPoSessionBufferOverflowException.h"
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
  AbstractPoCommandBuilder<AbstractPoResponseParser>* builder)
: AbstractApduResponseParser(response, builder) {}

AbstractPoCommandBuilder<AbstractPoResponseParser>*
    AbstractPoResponseParser::getBuilder() const
{
    return dynamic_cast<AbstractPoCommandBuilder<AbstractPoResponseParser>*>(
                AbstractApduResponseParser::getBuilder());
}

const KeypleSeCommandException AbstractPoResponseParser::buildCommandException(
    const std::type_info& exceptionClass,
    const std::string& message,
    const std::shared_ptr<SeCommand> commandRef,
    const int statusCode) const
{
    std::shared_ptr<CalypsoPoCommand> command =
        std::dynamic_pointer_cast<CalypsoPoCommand>(commandRef);

    if (exceptionClass == typeid(CalypsoPoAccessForbiddenException)) {
        return std::move(
                   CalypsoPoAccessForbiddenException(message,
                                                     command,
                                                     statusCode));
    } else if (exceptionClass == typeid(CalypsoPoDataAccessException)) {
        return std::move(
                   CalypsoPoDataAccessException(message, command, statusCode));
    } else if (exceptionClass == typeid(CalypsoPoDataOutOfBoundsException)) {
        return std::move(
                   CalypsoPoDataOutOfBoundsException(message,
                                                     command,
                                                     statusCode));
    } else if (exceptionClass == typeid(CalypsoPoIllegalArgumentException)) {
        return std::move(
                   CalypsoPoIllegalArgumentException(message, command));
    } else if (exceptionClass == typeid(CalypsoPoIllegalParameterException)) {
        return std::move(
                   CalypsoPoIllegalParameterException(message,
                                                      command,
                                                      statusCode));
    } else if (exceptionClass == typeid(CalypsoPoPinException)) {
        return std::move(
                   CalypsoPoPinException(message, command, statusCode));
    } else if (exceptionClass == typeid(CalypsoPoSecurityContextException)) {
        return std::move(
                   CalypsoPoSecurityContextException(message,
                                                     command,
                                                     statusCode));
    } else if (exceptionClass == typeid(CalypsoPoSecurityDataException)) {
        return std::move(
                   CalypsoPoSecurityDataException(message,
                                                  command,
                                                  statusCode));
    } else if (exceptionClass ==
                   typeid(CalypsoPoSessionBufferOverflowException)) {
        return std::move(
                   CalypsoPoSessionBufferOverflowException(message,
                                                           command,
                                                           statusCode));
    } else if (exceptionClass == typeid(CalypsoPoTerminatedException)) {
        return std::move(
                   CalypsoPoTerminatedException(message, command, statusCode));
    } else {
        return std::move(
                   CalypsoPoUnknownStatusException(message,
                                                   command,
                                                   statusCode));
    }
}

void AbstractPoResponseParser::checkStatus() const
{
    try {
        AbstractApduResponseParser::checkStatus();
    } catch (const KeypleSeCommandException& e) {
        throw e;
    }
}

}
}
}
}
