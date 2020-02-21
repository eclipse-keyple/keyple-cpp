/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#include "SamBuilderParser.h"
#include "AbstractSamCommandBuilder.h"
#include "AbstractSamResponseParser.h"

namespace keyple {
namespace calypso {
namespace command {
namespace sam {

using namespace keyple::calypso::command;

SamBuilderParser::SamBuilderParser(
    std::shared_ptr<AbstractSamCommandBuilder> samCommandBuilder)
: samCommandBuilder(samCommandBuilder)
{
}

std::shared_ptr<AbstractSamCommandBuilder> SamBuilderParser::getCommandBuilder()
{
    return samCommandBuilder;
}

std::shared_ptr<AbstractSamResponseParser> SamBuilderParser::getResponseParser()
{
    return samResponseParser;
}

void SamBuilderParser::setResponseParser(
    std::shared_ptr<AbstractSamResponseParser> poResponseParser)
{
    this->samResponseParser = poResponseParser;
}

}
}
}
}
