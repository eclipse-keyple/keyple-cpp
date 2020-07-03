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

#include "AbstractApduResponseParser.h"

#include "KeypleSeCommandException.h"
#include "KeypleSeCommandUnknownStatusException.h"

namespace keyple {
namespace core {
namespace command {

using namespace keyple::core::command::exception;
using namespace keyple::core::seproxy::message;

using StatusProperties = AbstractApduResponseParser::StatusProperties;

std::map<int, std::shared_ptr<StatusProperties>>
    AbstractApduResponseParser::STATUS_TABLE;

/* ABSTRACT APDU RESPONSE PARSER ---------------------------------------------*/

AbstractApduResponseParser::StaticConstructor::StaticConstructor()
{
    STATUS_TABLE.emplace(0x9000, std::make_shared<StatusProperties>("Success"));
}

AbstractApduResponseParser::StaticConstructor
    AbstractApduResponseParser::staticConstructor;

std::map<int, std::shared_ptr<StatusProperties>>&
    AbstractApduResponseParser::getStatusTable() const
{
    return STATUS_TABLE;
}

AbstractApduResponseParser::AbstractApduResponseParser(
  const std::shared_ptr<ApduResponse>& response,
  const std::shared_ptr<AbstractApduCommandBuilder> builder)
: mResponse(response), mBuilder(builder)
{
}

void AbstractApduResponseParser::setApduResponse(
  const std::shared_ptr<ApduResponse>& response)
{
    mResponse = response;
}

const std::shared_ptr<ApduResponse>
    AbstractApduResponseParser::getApduResponse() const
{
    return mResponse;
}

const std::shared_ptr<StatusProperties>
    AbstractApduResponseParser::getStatusCodeProperties() const
{
    return getStatusTable()[mResponse->getStatusCode()];
}

bool AbstractApduResponseParser::isSuccessful() const
{
    std::shared_ptr<StatusProperties> props = getStatusCodeProperties();

    return props != nullptr && props->isSuccessful();
}

std::string AbstractApduResponseParser::getStatusInformation() const
{
    std::shared_ptr<StatusProperties> props = getStatusCodeProperties();

    return props != nullptr ? props->getInformation() : "";
}

void AbstractApduResponseParser::checkStatus()
{
    std::shared_ptr<StatusProperties> props = getStatusCodeProperties();

    if (props != nullptr && props->isSuccessful())
        return;

    /* Status code is not referenced, or not successful */

    /* Exception class */
    std::shared_ptr<KeypleSeCommandException> exceptionClass =
            props != nullptr ? props->getExceptionClass() : nullptr;

    /* Message */
    const std::string& message = props != nullptr ? props->getInformation()
        : "Unknown status";

    /* Command reference */
    std::shared_ptr<SeCommand> commandRef = getCommandRef();

    /* status code */
    int statusCode = mResponse->getStatusCode();

    /* Throw the exception */
    throw *buildCommandException(exceptionClass, message, commandRef,
                                 std::make_shared<Integer>(statusCode)).get();
}

const std::shared_ptr<SeCommand> AbstractApduResponseParser::getCommandRef()
    const
{
    return mBuilder != nullptr ? mBuilder->getCommandRef() : nullptr;
}

const std::shared_ptr<AbstractApduCommandBuilder>
    AbstractApduResponseParser::getBuilder() const
{
    return mBuilder;
}

const std::shared_ptr<KeypleSeCommandException>
    AbstractApduResponseParser::buildCommandException(
        const std::shared_ptr<KeypleSeCommandException> exceptionClass,
        const std::string& message,
        const std::shared_ptr<SeCommand> commandRef,
        const std::shared_ptr<Integer> statusCode)
{
    (void)exceptionClass;

    return std::make_shared<KeypleSeCommandUnknownStatusException>(
               message, commandRef, statusCode);
}

/* STATUS PROPERTIES -------------------------------------------------------- */

StatusProperties::StatusProperties(
  const std::string& information)
: mSuccessful(true), mInformation(information), mExceptionClass(nullptr) {}

StatusProperties::StatusProperties(
  const std::string& information,
  const std::shared_ptr<KeypleSeCommandException> exceptionClass)
: mSuccessful(exceptionClass == nullptr), mInformation(information),
  mExceptionClass(exceptionClass) {}

bool StatusProperties::isSuccessful() const
{
    return mSuccessful;
}

const std::string& StatusProperties::getInformation() const
{
    return mInformation;
}

const std::shared_ptr<KeypleSeCommandException>
    StatusProperties::getExceptionClass() const
{
    return mExceptionClass;
}

}
}
}
