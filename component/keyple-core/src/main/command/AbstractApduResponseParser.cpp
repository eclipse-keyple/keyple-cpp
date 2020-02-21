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

namespace keyple {
namespace core {
namespace command {

using namespace keyple::core::seproxy::message;

std::unordered_map<int, std::shared_ptr<
    AbstractApduResponseParser::StatusProperties>>
        AbstractApduResponseParser::STATUS_TABLE;

AbstractApduResponseParser::StaticConstructor::StaticConstructor()
{
    std::unordered_map<int, std::shared_ptr<StatusProperties>> m;
    m.emplace(0x9000, std::make_shared<StatusProperties>(true, "Success"));
    STATUS_TABLE = m;
}

AbstractApduResponseParser::StaticConstructor
    AbstractApduResponseParser::staticConstructor;

std::unordered_map<int, std::shared_ptr<
    AbstractApduResponseParser::StatusProperties>>
        AbstractApduResponseParser::getStatusTable() const
{
    return STATUS_TABLE;
}

AbstractApduResponseParser::AbstractApduResponseParser(
    std::shared_ptr<ApduResponse> response)
{
    this->response = response;
    initialized = true;
}

AbstractApduResponseParser::AbstractApduResponseParser()
{
    initialized = false;
}

void AbstractApduResponseParser::setApduResponse(
    std::shared_ptr<ApduResponse> response)
{
    this->response = response;
    initialized = true;
}

bool AbstractApduResponseParser::isInitialized()
{
    return initialized;
}

const std::shared_ptr<ApduResponse>
AbstractApduResponseParser::getApduResponse() const
{
    if (!initialized) {
        throw IllegalStateException("The parser has not been initialized.");
    }

    return response;
}

int AbstractApduResponseParser::getStatusCode() const
{
    return response->getStatusCode();
}

std::shared_ptr<AbstractApduResponseParser::StatusProperties>
    AbstractApduResponseParser::getPropertiesForStatusCode() const
{
    return getStatusTable()[getStatusCode()];
}

bool AbstractApduResponseParser::isSuccessful() const
{
    if (!initialized) {
        throw IllegalStateException("The parser has not been initialized.");
    }

    std::shared_ptr<StatusProperties> props = getPropertiesForStatusCode();
    return props != nullptr && props->isSuccessful();
}

std::string AbstractApduResponseParser::getStatusInformation()
{
    std::shared_ptr<StatusProperties> props = getPropertiesForStatusCode();
    return props != nullptr ? props->getInformation() : "";
}

AbstractApduResponseParser::StatusProperties::StatusProperties(
  bool successful, const std::string& information)
: successful(successful), information(information)
{
}

bool AbstractApduResponseParser::StatusProperties::isSuccessful()
{
    return successful;
}

std::string AbstractApduResponseParser::StatusProperties::getInformation()
{
    return information;
}

}
}
}
