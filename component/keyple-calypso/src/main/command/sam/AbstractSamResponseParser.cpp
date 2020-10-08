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
#include "CalypsoPoCommand.h"
#include "CalypsoSamAccessForbiddenException.h"
#include "CalypsoSamCounterOverflowException.h"
#include "CalypsoSamDataAccessException.h"
#include "CalypsoSamIllegalArgumentException.h"
#include "CalypsoSamIllegalParameterException.h"
#include "CalypsoSamIncorrectInputDataException.h"
#include "CalypsoSamSecurityDataException.h"
#include "CalypsoSamUnknownStatusException.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace keyple::calypso::command::po;
using namespace keyple::calypso::command::sam::exception;
using namespace keyple::core::command;
using namespace keyple::core::seproxy::message;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

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
    }, {
        0x9000,
        std::make_shared<StatusProperties>("Success")
    }
};

AbstractSamResponseParser::AbstractSamResponseParser(
  const std::shared_ptr<ApduResponse> response,
  AbstractSamCommandBuilder<AbstractSamResponseParser>* builder)
: AbstractApduResponseParser(response, builder) {}

const std::map<int, std::shared_ptr<StatusProperties>>&
    AbstractSamResponseParser::getStatusTable() const
{
    return STATUS_TABLE;
}

AbstractApduCommandBuilder* AbstractSamResponseParser::getBuilder() const
{
    return AbstractApduResponseParser::getBuilder();
}

const KeypleSeCommandException AbstractSamResponseParser::buildCommandException(
    const std::type_info& exceptionClass,
    const std::string& message,
    const std::shared_ptr<SeCommand> commandRef,
    const int statusCode) const
{
    std::shared_ptr<CalypsoPoCommand> command =
        std::dynamic_pointer_cast<CalypsoPoCommand>(commandRef);

    if (exceptionClass == typeid(CalypsoSamAccessForbiddenException)) {
        return std::move(
                   CalypsoSamAccessForbiddenException(
                       message,
                       std::dynamic_pointer_cast<CalypsoSamCommand>(command),
                       statusCode));
    } else if (exceptionClass == typeid(CalypsoSamCounterOverflowException)) {
        return std::move(
                   CalypsoSamCounterOverflowException(
                       message,
                       std::dynamic_pointer_cast<CalypsoSamCommand>(command),
                       statusCode));
    } else if (exceptionClass == typeid(CalypsoSamDataAccessException)) {
        return std::move(
                   CalypsoSamDataAccessException(
                       message,
                       std::dynamic_pointer_cast<CalypsoSamCommand>(command),
                       statusCode));
    } else if (exceptionClass == typeid(CalypsoSamIllegalArgumentException)) {
        return std::move(
                   CalypsoSamIllegalArgumentException(
                       message,
                       std::dynamic_pointer_cast<CalypsoSamCommand>(command)));
    } else if (exceptionClass == typeid(CalypsoSamIllegalParameterException)) {
        return std::move(
                   CalypsoSamIllegalParameterException(
                       message,
                       std::dynamic_pointer_cast<CalypsoSamCommand>(command),
                       statusCode));
    } else if (exceptionClass == typeid(CalypsoSamIncorrectInputDataException)) {
        return std::move(
                   CalypsoSamIncorrectInputDataException(
                       message,
                       std::dynamic_pointer_cast<CalypsoSamCommand>(command),
                       statusCode));
    } else if (exceptionClass == typeid(CalypsoSamSecurityDataException)) {
        return std::move(
                   CalypsoSamSecurityDataException(
                       message,
                       std::dynamic_pointer_cast<CalypsoSamCommand>(command),
                       statusCode));
    } else {
        return std::move(
                   CalypsoSamUnknownStatusException(
                       message,
                       std::dynamic_pointer_cast<CalypsoSamCommand>(command),
                       statusCode));
    }
}

void AbstractSamResponseParser::checkStatus() const
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
